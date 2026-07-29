/* -*- mode: c; -*- */

#ifndef BUILD_H_
#define BUILD_H_

#if !defined(BUILD_DEF)
   #define BUILD_DEF
#endif

#if !defined(BUILD_ASSERT)
   #include <assert.h>
   #define BUILD_ASSERT(condition) assert((condition))
#endif

#if !defined(BUILD_MEM_ALLOC) || !defined(BUILD_MEM_REALLOC) || !defined(BUILD_MEM_FREE)
   #include <stdlib.h>

   #if !defined(BUILD_MEM_ALLOC)
      #define BUILD_MEM_ALLOC malloc
   #endif

   #if !defined(BUILD_MEM_REALLOC)
      #define BUILD_MEM_REALLOC realloc
   #endif

   #if !defined(BUILD_MEM_FREE)
      #define BUILD_MEM_FREE free
   #endif
#endif

#if !defined(NULL)
   #define NULL ((void*)0)
#endif

#if defined(_WIN32) || defined(_WIN64)
   #if !defined(BUILD_OS_WINDOWS)
      #define BUILD_OS_WINDOWS 1
   #endif
#elif defined(__linux__)
   #if !defined(BUILD_OS_LINUX)
      #define BUILD_OS_LINUX 1
   #endif
#else
   #define BUILD_OS_UNSUPPORTED 1
#endif

/* TODO: if C23+ or C++ use {} instead */
#define BUILD__EMPTY_VALUE {0}

#if BUILD_OS_WINDOWS
   #define WIN32_LEAN_AND_MEAN
   #include <Windows.h>
#elif BUILD_OS_LINUX
   #include <sys/stat.h>
#else
   #error "OS unsupported"
#endif

typedef struct {
   char* name;
   char** argv;
   int argc;
} build_Program;

BUILD_DEF int build_directory_new(const char* path);
#if !defined(BUILD_PREFIX_DEFINITIONS)
   #define directory_new build_directory_new
#endif

BUILD_DEF int build_directory_delete(const char* path);
#if !defined(BUILD_PREFIX_DEFINITIONS)
   #define directory_delete build_directory_delete
#endif

BUILD_DEF int build_bytes_len(const char* s);
#if !defined(BUILD_PREFIX_DEFINITIONS)
   #define bytes_len build_bytes_len
#endif

BUILD_DEF char* build_bytes_clone(const char* s);
#if !defined(BUILD_PREFIX_DEFINITIONS)
   #define bytes_clone build_bytes_clone
#endif

#endif /* BUILD_H_ */

#ifdef BUILD_IMPLEMENTATION
#undef BUILD_IMPLEMENTATION

BUILD_DEF int build_directory_new(const char* path)
{
   int result = 0;
   int i = 0;
   char* path_cstr = build_bytes_clone(path);
   int path_len = build_bytes_len(path);
   char temp_char = '\0';

   for (i = 0; i < path_len; i++) {
      if (path_cstr[i] == '/' || path_cstr[i] == '\\') {
         temp_char = path_cstr[i];
         path_cstr[i] = '\0';
#if BUILD_OS_WINDOWS
         result = CreateDirectoryA(path_cstr, NULL);
#else
         result = mkdir(path_cstr, 0755);
         result = !result;
#endif
         path_cstr[i] = temp_char;
      } else if (i == (path_len - 1)) {
#if BUILD_OS_WINDOWS
         CreateDirectoryA(path_cstr, NULL);
#else
         result = mkdir(path_cstr, 0755);
         result = !result;
#endif
      }
   }

   BUILD_MEM_FREE(path_cstr);
   return result;
}

BUILD_DEF int build_directory_delete(const char* path)
{
   int result = 0;

   /* TODO implement recursively deleting directory contents */

   return result;
}

BUILD_DEF int build_bytes_len(const char* s)
{
   int len = 0;
   while (*s++) {
      len++;
   }
   return len;
}

BUILD_DEF char* build_bytes_clone(const char* s)
{
   int i = 0;
   int len = build_bytes_len(s);
   char* result = (char*)BUILD_MEM_ALLOC(sizeof(*s) * (len + 1));
   if (!result) {
      return NULL;
   }

   for (i = 0; i < len; i++) {
      result[i] = s[i];
   }
   result[len] = '\0';

   return result;
}

#endif /* BUILD_IMPLEMENTATION */
