#include "cbuild/build.h"

int main(int argc, char** argv) {
    REBUILD_BUILDER(argc, argv);

    path_t root         = extract_root(argc, argv);
    compiler_t gcc      = new_compiler(GCC, "/usr/bin/gcc");
    cache_data_t cache  = load_cache(&root, "./.build_cache");
    project_t project   = new_project(&root);

    index_project(&project);

    compile_project(&project, &cache, &gcc);

    make_executable(&project, &cache, &gcc, "a.out");
    // test_project(&project, &cache, &gcc, NULL); 

    save_cache(&cache);

    free_cache(&cache);
    free_compiler(&gcc);
    free_project(&project);
    return 0;
}
