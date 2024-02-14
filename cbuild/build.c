#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "build.h"

path_t path_from_string(char* str) {
    size_t len = strlen(str);
    char* cpy = malloc(len + 1);
    if (cpy == NULL) {
        log_fatal("Could not allocate %lu bytes for new path.\n", len + 1);
    }

    strcpy(cpy, str);
    return (path_t) {.length = len, .str = cpy};
}

char* get_path(path_t* path) {
    char* str = malloc(path->length + 1);
    strcpy(str, path->str);
    return str;
}

path_t extract_root(int argc, char** argv) {
    /* TODO: argv[0] is not guaranteed to contain path to executable */
    size_t size;
    path_t path;
    char *str;

    str = last_name_start(argv[0]);

    if (str == NULL) {
        /* TODO: maybe this if statement can never be true? */
        log_info("Root path: \".\"\n");
        return path_from_string(".");
    }

    size = str - argv[0];
    path.length = size;
    path.str = malloc(size + 1);
    if (path.str == NULL) {
        log_fatal("Could not allocate %lu bytes for project root.\n", size + 1);
    }

    strncpy(path.str, argv[0], size);
    path.str[size] = '\0';

    log_info("Root path: \"%s\"\n", path.str);
    return path;
}

path_t append_path(path_t* path, char* name) {
    size_t path_length, name_length;
    path_t new_path;

    path_length = strlen(path->str);
    name_length = strlen(name);

    new_path.str = malloc(path_length + name_length + 2);

    for (size_t i = 0; i < path_length; i++) {
        new_path.str[i] = path->str[i];
    }

    new_path.str[path_length] = '/';

    for (size_t i = 0; i < name_length; i++) {
        new_path.str[path_length + 1 + i] = name[i];
    }

    new_path.length = path_length + 1 + name_length;
    new_path.str[path_length + 1 + name_length] = '\0';

    return new_path;
}

char* last_name_start(char* path) {
    return strrchr(path, '/');
}

void free_path(path_t* path) {
    free(path->str);
    path->str = NULL;
}

project_t new_project(path_t* root) {
    return (project_t) {
        .root = *root,
        .files = NULL,
    };
}

void free_project(project_t* project) {
    free_path(&project->root);
    free_source_set(project->files);
    project->files = NULL;
}

source_set_t* append_source(source_set_t* set, source_t file) {
    source_set_t* head = malloc(sizeof(source_set_t));
    head->next = set;
    head->file = file;
    return head;
}

source_t new_implementation(path_t* path, struct dirent* file) {
    size_t length = strlen(file->d_name);
    char* name = malloc(length + 1);
    strcpy(name, file->d_name);
    *strrchr(name, '.') = '\0';

    return (source_t) {
        .path = *path,
        .name = name,
        .type = IMPLEMENTATION,
    };
}

source_t new_header(path_t* path, struct dirent* file) {
    return (source_t) {
        .path = *path,
        .type = HEADER,
    };
}

source_t new_test(path_t* path, struct dirent* file) {
    return (source_t) {
        .path = *path,
        .type = TEST,
    };
}

void free_source_set(source_set_t* set) {
    if (set == NULL) {return;}
    free_source(&set->file);
    free_source_set(set->next);
    free(set);
}

void free_source(source_t* file) {
    free_path(&file->path);
    free(file->name);
    file->name = NULL;
    file->type = INVALID;
}

cache_data_t load_cache(path_t* root, char* cache_path) {
    char* str;
    path_t path, file_path;

    path = append_path(root, cache_path);
    str = get_path(&path);

    errno = 0;
    mkdir(str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    free(str);
    switch (errno) {
        case 0:
            log_info("Creating cache directory.\n");
        case EACCES:
            log_fatal("No access to create cache.\n");
        case EEXIST:
            log_info("Cache already exists.\n");
            break;
        case ENAMETOOLONG:
            log_fatal("Name of cache directory too long.\n");
        case EROFS:
            log_fatal("Cache cannot be created in a read-only file system.\n");
        default:
            log_fatal("Unreachable error ???\n");
    }
    

    file_path = append_path(&path, "cache.json");
    str = get_path(&file_path);
    if (!access(str, R_OK | W_OK)) {
        free(str);
        return parse_cache(&file_path);
    }
    free(str);

    return (cache_data_t) {
        .path = path,
        .files = NULL,
    };
}

cache_data_t parse_cache(path_t* path) {
    log_fatal("parse_cache: unimplemented.\n");
}

void save_cache(cache_data_t* cache) {
    log_fatal("save_cache: unimplemented.\n");
}

void free_cache(cache_data_t* cache) {
    free_path(&cache->path);
    free_source_set(cache->files);
}

compiler_t new_compiler(compiler_type_t type, char* executable) {
    size_t len = strlen(executable);
    char* cpy = malloc(len + 1);
    if (cpy == NULL) {
        log_fatal("Could not allocate %lu bytes for compiler.", len + 1);
    }
    strcpy(cpy, executable);

    return (compiler_t) {
        .type = type,
        .executable = cpy,
        .links = NULL,
    };
}

void free_compiler(compiler_t* compiler) {
    free(compiler->executable);
    compiler->executable = NULL;
}

void index_recursive(project_t* project, path_t* path) {
    char *name;
    path_t new_path;
    DIR* dir;
    struct dirent* file;
    source_t source_file;
    
    name = get_path(path);
    dir = opendir(name);
    free(name);
    if (dir == NULL) {
        return;
    }

    while ((file = readdir(dir)) != NULL) {
        name = file->d_name;
        if (name[0] == '.') {continue;}

        name = strrchr(name, '.');
        if (name == NULL) {
            new_path = append_path(path, file->d_name);
            index_recursive(project, &new_path);
            free_path(&new_path);
            continue;
        }

        new_path = append_path(path, file->d_name);
        if        (strcmp(name, ".c") == 0) {
            source_file = new_implementation(&new_path, file);
        } else if (strcmp(name, ".h") == 0) {
            source_file = new_header(&new_path, file);
        } else {
            free_path(&new_path);
            continue;
        }

        project->files = append_source(project->files, source_file);
    }

    closedir(dir);
}

void index_project(project_t* project) {
    char* str;
    DIR* dir;

    str = get_path(&project->root);
    dir = opendir(str);
    if (dir == NULL) {
        closedir(dir);
        log_fatal("Could not open project root \"%s\"", str);
    }
    free(str);
    closedir(dir);

    index_recursive(project, &project->root);

    log_info("Indexed project from root.\n");
}

error_t make_executable(project_t* project, cache_data_t* cache, compiler_t* compiler, char* name) {
    compile_project(project, cache, compiler);
    log_fatal("make_executable: unimplemented.\n");
}

error_t compile_project(project_t* project, cache_data_t* cache, compiler_t* compiler) {
    error_t error, return_error = NO_ERROR;
    source_set_t* set;
    source_t file;
    char* command;

    set = project->files;
    for (file = set->file; set != NULL; set = set->next) {
        file = set->file;
        if (file.type == HEADER) {continue;}

        error = compile_file(compiler, cache, &file);
        if (error != NO_ERROR) {return_error = error;}
    }

    return return_error;
}

error_t compile_file(compiler_t* compiler, cache_data_t* cache, source_t* file) {
    int result;
    char cmd[500], *str, *temp, c;
    size_t i = 0;

    str = compiler->executable;
    while ((c = *str++) != '\0') {cmd[i++] = c;}
    cmd[i++] = ' ';

    cmd[i++] = '-'; cmd[i++] = 'o'; cmd[i++] = ' ';

    str = temp = get_path(&cache->path);
    while ((c = *str++) != '\0') {cmd[i++] = c;}
    free(temp);

    cmd[i++] = '/';
    str = file->name;
    while ((c = *str++) != '\0') {cmd[i++] = c;}
    cmd[i++] = '.'; cmd[i++] = 'o'; cmd[i++] = ' ';

    cmd[i++] = '-'; cmd[i++] = 'c'; cmd[i++] = ' ';
    str = temp = get_path(&file->path);
    while ((c = *str++) != '\0') {cmd[i++] = c;}
    free(temp);

    cmd[i++] = '\0';

    result = system(cmd);

    if (result) {
        return INTERNAL_ERROR;
    }
    return NO_ERROR;
}

error_t test_project(project_t* project, cache_data_t* cache, compiler_t* compiler, path_t* tests) {
    return NO_ERROR;
}
