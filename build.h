/* build.h -*- mode: c -*-

Single header-only C89+ library for writing build recipes.

# LICENSE

SPDX-License-Identifier: 0BSD

Copyright (C) 2026 by Jesse <jessebot.git@gmail.com>

Permission to use, copy, modify, and/or distribute this software for
any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef BUILD_H
#define BUILD_H

#define BUILD_VERSION_MAJOR 0
#define BUILD_VERSION_MINOR 1
#define BUILD_VERSION_PATCH 0

#ifndef NULL
   #define NULL ((void*)0)
#endif

#ifndef BUILD_DEF
   #define BUILD_DEF
#endif

#ifndef BUILD_INTERNAL_DEF
   #define BUILD_INTERNAL_DEF static
#endif

#ifndef BUILD_NO_STDINC
   #ifndef BUILD_ASSERT
      #include <assert.h>
      #define BUILD_ASSERT(condition) assert((condition))
   #endif

   #if !defined(BUILD_MEM_ALLOC) || !defined(BUILD_MEM_REALLOC) || !defined(BUILD_MEM_FREE)
      #include <stdlib.h>

      #ifndef BUILD_MEM_ALLOC
         #define BUILD_MEM_ALLOC malloc
      #endif

      #ifndef BUILD_MEM_REALLOC
         #define BUILD_MEM_REALLOC realloc
      #endif

      #ifndef BUILD_MEM_FREE
         #define BUILD_MEM_FREE free
      #endif
   #endif
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
   #if !defined(BUILD_OS_UNSUPPORTED)
      #error "OS unsupported"
   #endif
#endif

/* TODO: if C23+ or C++ use {} instead */
#define BUILD__EMPTY_VALUE {0}

#if BUILD_OS_WINDOWS
   #define WIN32_LEAN_AND_MEAN
   #include <Windows.h>
#elif BUILD_OS_LINUX
   #include <sys/types.h>
   #include <sys/stat.h>
   #include <unistd.h>
#endif

/**
 * Dynamically-sized null-terminated array of null-terminated sequence of bytes.
 */
typedef struct Build_StringList Build_StringList;
struct Build_StringList {
   char** data;
   int count;
   int capacity;
};
#ifndef BUILD_UNSTRIP_PREFIX
   #define StringList Build_StringList
#endif

/**
 * Null-terminated array of bytes.
 */
typedef struct Build_StringBuffer Build_StringBuffer;
struct Build_StringBuffer {
   char* data;
   int count;
   int capacity;
};
#ifndef BUILD_UNSTRIP_PREFIX
   #define StringBuffer Build_StringBuffer
#endif

/**
 * Options for compiling a collection of source files to produce an output object/executable.
 */
struct Build_CompileTarget {
   char* compiler;
   char* output_path;
   Build_StringList compile_flags;
   Build_StringList linker_flags;
   Build_StringList source_files;
};
#ifndef BUILD_UNSTRIP_PREFIX
   #define CompileTarget Build_CompileTarget
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Append a null-terminated string to a StringList. Initializes
 * StringList if empty. NOTE: The null-terminated string that will be
 * appended to the StringList will be deep-copied/cloned.
 */
BUILD_DEF int build_string_list_append(Build_StringList* l, const char* s);
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define string_list_append build_string_list_append
#endif

BUILD_INTERNAL_DEF int build__cstr_count(const char* s);
BUILD_INTERNAL_DEF char* build__cstr_clone(const char* s);

#if 0
BUILD_DEF void build_win32_join_and_quote_command_list(Build_StringList* list, Build_StringBuffer* buf);
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define win32_join_and_quote_command_list build_win32_join_and_quote_command_list
#endif
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BUILD_H_ */

#ifdef BUILD_IMPLEMENTATION

BUILD_DEF int build_string_list_append(Build_StringList* l, const char* s)
{
   char* item;

   BUILD_ASSERT(l);

   if (l->capacity == 0) {
      int new_capacity = 2;

      l->data = (char**)BUILD_MEM_ALLOC(sizeof(*l->data) * new_capacity);
      if (!l->data) {
         return 0;
      }
      l->capacity = new_capacity;
   } else if (l->capacity <= (l->count + 1)) {
      char** tmp;
      int new_capacity = l->capacity * 2;

      BUILD_ASSERT(new_capacity >= 1);
      while (new_capacity <= (l->count + 1)) {
         new_capacity *= 2;
      }

      tmp = (char**)BUILD_MEM_REALLOC(l->data, sizeof(*l->data) * new_capacity);
      if (!tmp) {
         return 0;
      }
      l->data = tmp;
      l->capacity = new_capacity;
   }
   l->data[l->count] = NULL;

   item = build__cstr_clone(s);
   if (!item) {
      return 0;
   }

   l->data[l->count++] = item;
   l->data[l->count] = NULL;

   return 1;
}

BUILD_INTERNAL_DEF int build__cstr_count(const char* s)
{
   int len = 0;
   while (*s++) {
      len++;
   }
   return len;
}

BUILD_INTERNAL_DEF char* build__cstr_clone(const char* s)
{
   char* result;
   int i;
   int count = build__cstr_count(s);

   if (!s) {
      return NULL;
   }

   result = (char*)BUILD_MEM_ALLOC(sizeof(*s) * (count + 1));
   if (!result) {
      return NULL;
   }

   for (i = 0; i < (count + 1); i++) {
      result[i] = s[i];
   }
   result[count] = '\0';

   return result;
}

#if 0
BUILD_DEF void build_win32_join_and_quote_command_list(Build_StringList* list, Build_StringBuffer* buf)
{
   int i = 0;

   BUILD_ASSERT(list);
   BUILD_ASSERT(buf);

   for (i = 0; i < list->count; i++) {
      int len = build_count_bytes(list->data[i]);
      int backslashes = 0;
      int j = 0;

      if (list->data[i] == NULL) {
         break;
      }

      if (i > 0) {
         build_append_to_string_buffer(buf, " ");
      }

      /* TODO: dont quote if u cant find one of the following chars:  { '\t', '\n', '\v', '\"' }
      if (len != 0 && strpbrk(list->data[i], " \t\n\v\"")) {
      } else { // ... everything below ...
       */

      build_append_to_string_buffer(buf, "\"");
      for (j = 0; j < len; j++) {
         char c = list->data[i][j];
         if (c == '\\') {
            backslashes++;
         } else {
            if (c == '\"') {
               int k = 0;
               for (k = 0; k < backslashes + 1; k++) {
                  build_append_to_string_buffer(buf, "\\");
               }
            }
            backslashes = 0;
         }
         build_append_to_string_buffer_with_count(buf, &c, 1);
      }

      for (j = 0; j < backslashes; j++) {
         build_append_to_string_buffer(buf, "\\");
      }
      build_append_to_string_buffer(buf, "\"");
   }
}
#endif

#endif /* BUILD_IMPLEMENTATION */
