#ifndef BUILD_H
#define BUILD_H

#include <dirent.h>
#include "../error.h"
#include "../data_structure/array/gar/gar.h"

#define REBUILD_BUILDER(argc, argv) \
    do {                                                                        \
    } while (0)
#define log_fatal(...) do {printf("[FATAL] " __VA_ARGS__); exit(1);} while(0)
#define log_info(...)      printf("[ INFO] " __VA_ARGS__)

typedef struct compiler     compiler_t;

typedef struct project      project_t;
typedef struct cache_data   cache_data_t;
typedef struct source_set   source_set_t;
typedef struct source       source_t;
typedef struct path         path_t;

struct path {
    size_t length;
    char* str;
};

typedef enum compiler_type {
    GCC,
    CLANG,
} compiler_type_t;

struct compiler {
    compiler_type_t type;
    char* executable;
    char** links;
};

typedef enum file_type {
    INVALID,
    IMPLEMENTATION,
    HEADER,
    TEST,
} file_type_t;

struct source {
    path_t path;
    char* name;
    file_type_t type;
    // <type> last_mofified;
};

struct source_set {
    source_t file;
    source_set_t* next;
};

struct cache_data {
    path_t path;
    source_set_t* files;
};

struct project {
    path_t root;
    source_set_t* files;
};

path_t          path_from_string(char*);
path_t          extract_root(int, char**);
char*           last_name_start(char*);
path_t          append_path(path_t*, char*);
char*           get_path(path_t*);
void            free_path(path_t*);

source_t        new_header(path_t*, struct dirent*);
source_t        new_implementation(path_t*, struct dirent*);
source_t        new_test(path_t*, struct dirent*);
source_set_t*   append_source(source_set_t*, source_t);
void            free_source(source_t*);
void            free_source_set(source_set_t*);

compiler_t      new_compiler(compiler_type_t, char*);
error_t         compile_file(compiler_t*, cache_data_t*, source_t*);
void            free_compiler(compiler_t*);

cache_data_t    load_cache(path_t*, char*);
cache_data_t    parse_cache(path_t*);
void            save_cache(cache_data_t*);
void            free_cache(cache_data_t*);

project_t       new_project(path_t*);
void            free_project(project_t*);
void            index_project(project_t*);
error_t         compile_project(project_t*, cache_data_t*, compiler_t*);
error_t         make_executable(project_t*, cache_data_t*, compiler_t*, char*);
error_t         test_project(project_t*, cache_data_t*, compiler_t*, path_t*);

#endif
