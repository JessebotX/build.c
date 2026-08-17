/* build.c

  Last updated: 2026-08-16
*/

#define BUILD_IMPLEMENTATION
#include "../../build.h"      // #include "/your/path/to/build.h" here

int main(void)
{
   Artifact artifact = (Artifact){
      .compiler = "clang",
      .compile_options = strlist_from_args("-O0", "-g3", NULL),
      .source_files = strlist_from_args("src/main.c", "src/base.c", NULL),
   };
   artifact_new_executable(&artifact, "demo");

   return 0;
}
