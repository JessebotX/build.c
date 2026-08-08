/* build.h -*- mode: c -*-

Single header-only C89+ library for writing build recipes.

# License

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

#if !BUILD_OS_OTHER
   #if defined(_WIN32) || defined(_WIN64)
      #ifndef BUILD_OS_WINDOWS
         #define BUILD_OS_WINDOWS 1
      #endif
   #else
      #error OS unsupported by default
   #endif
#else
   #define BUILD_OS_OTHER 1
#endif

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
   #endif

   #ifndef BUILD_MEM_ALLOC
      #define BUILD_MEM_ALLOC(n) malloc(n)
   #endif

   #ifndef BUILD_MEM_REALLOC
      #define BUILD_MEM_REALLOC(ptr, n, n_old) realloc(ptr, n)
   #endif

   #ifndef BUILD_MEM_FREE
      #define BUILD_MEM_FREE(ptr, n) free
   #endif
#endif

#if __STDC_VERSION__ >= 202311L || __cplusplus >= 201103L
   #define BUILD__EMPTY_VALUE {}
#else
   #define BUILD__EMPTY_VALUE {0}
#endif

typedef struct Build_StringBuffer Build_StringBuffer;
struct Build_StringBuffer {
   char* data;
   int count;
   int capacity;
};
#ifndef BUILD_UNSTRIP_PREFIX
   #define StringBuffer Build_StringBuffer
#endif

typedef struct Build_StringList Build_StringList;
struct Build_StringList {
   char** data;
   int count;
   int capacity;
};
#ifndef BUILD_UNSTRIP_PREFIX
   #define StringList Build_StringList
#endif

struct Build_CompileTarget {
   char* compiler;
   char* output_name;
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

BUILD_DEF Build_StringBuffer build_string_buffer_new(const char* data);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_new build_string_buffer_new
#endif

BUILD_DEF Build_StringBuffer build_string_buffer_new_count(const char* data, int count);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_new_count build_string_buffer_new_count
#endif

BUILD_DEF Build_StringBuffer build_string_buffer_new_capacity(const char* data, int count, int capacity);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_new_capacity build_string_buffer_new_capacity
#endif

BUILD_DEF void build_string_buffer_free(Build_StringBuffer* s);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_free build_string_buffer_free
#endif

BUILD_DEF Build_StringList build_string_list_new(const char** data);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_new build_string_list_new
#endif

BUILD_DEF Build_StringList build_string_list_new_count(const char** data, int count);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_new_count build_string_list_new_count
#endif

BUILD_DEF Build_StringList build_string_list_new_capacity(const char** data, int count, int capacity);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_new_capacity build_string_list_new_capacity
#endif

BUILD_DEF void build_string_list_free_all(Build_StringList* l);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_free_all build_string_list_free_all
#endif

BUILD_DEF int build_string_list_append(Build_StringList* l, const char* arg);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_append build_string_list_append
#endif

BUILD_DEF int build_command_execute(Build_StringList* l);
#ifndef BUILD_UNSTRIP_PREFIX
   #define command_execute build_command_execute
#endif

BUILD_INTERNAL_DEF void* build__memcpy(void* destination, const void* source, int n);
BUILD_INTERNAL_DEF char* build__strdup(const char* s);
BUILD_INTERNAL_DEF char* build__strndup(const char* s, int n);
BUILD_INTERNAL_DEF int build__strlen(const char* s);

#if BUILD_OS_WINDOWS
BUILD_INTERNAL_DEF int build__windows_command_execute(Build_StringList* l);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BUILD_H */

#ifdef BUILD_IMPLEMENTATION

BUILD_DEF Build_StringBuffer build_string_buffer_new(const char* data)
{
   return build_string_buffer_new_count(data, build__strlen(data));
}

BUILD_DEF Build_StringBuffer build_string_buffer_new_count(const char* data, int count)
{
   return build_string_buffer_new_capacity(data, count, count + 1);
}

BUILD_DEF Build_StringBuffer build_string_buffer_new_capacity(const char* data, int count, int capacity)
{
   Build_StringBuffer result = BUILD__EMPTY_VALUE;

   if (count < 0 || count >= capacity) {
      /* TODO: logging */
      return result;
   }

   result.data = (char*)BUILD_MEM_ALLOC(sizeof(*result.data) * capacity);
   if (!result.data) {
      /* TODO: logging */
      return result;
   }
   result.capacity = capacity;

   build__memcpy(result.data, data, count);
   result.data[count] = '\0';
   result.count = count;

   return result;
}

BUILD_DEF void build_string_buffer_free(Build_StringBuffer* s)
{
   if (!s) {
      /* TODO: logging */
      return;
   }

   BUILD_MEM_FREE(s->data, sizeof(*s->data) * s->capacity);
   s->data = NULL;
   s->count = 0;
   s->capacity = 0;
}

BUILD_DEF Build_StringList build_string_list_new(const char** data)
{
   int count = 0;
   while (data && *data++) {
      count++;
   }
   return build_string_list_new_count(data, count);
}

BUILD_DEF Build_StringList build_string_list_new_count(const char** data, int count)
{
   return build_string_list_new_capacity(data, count, count + 1);
}

BUILD_DEF Build_StringList build_string_list_new_capacity(const char** data, int count, int capacity)
{
   Build_StringList result = BUILD__EMPTY_VALUE;

   if (count < 0 || count >= capacity) {
      /* TODO: logging */
      return result;
   }

   result.data = (char**)BUILD_MEM_ALLOC(sizeof(*result.data) * capacity);
   if (!result.data) {
      /* TODO: logging */
      return result;
   }
   result.capacity = capacity;

   for (result.count = 0; result.count < count; result.count++) {
      char* tmp = build__strdup(data[result.count]);
      if (!tmp) {
         /* TODO: logging */
         return result;
      }
      result.data[result.count] = tmp;
   }
   result.data[result.count] = NULL;

   return result;
}

BUILD_DEF void build_string_list_free_all(Build_StringList* l)
{
   int i;

   if (!l || !l->data) {
      return;
   }

   for (i = 0; i < l->count; i++) {
      BUILD_MEM_FREE(l->data[i], sizeof(*l->data[i]) * (build__strlen(l->data[i]) + 1));
   }
   BUILD_MEM_FREE(l->data, sizeof(*l->data) * l->capacity);
   l->data = NULL;
   l->count = 0;
   l->capacity = 0;
}

BUILD_DEF int build_string_list_append(Build_StringList* l, const char* arg)
{
   int new_count = l->count + 1;
   char* arg_clone = NULL;

   if (new_count >= l->capacity) {
      char** tmp;
      int new_capacity = l->capacity * 2;
      int old_size = sizeof(*l->data) * l->capacity;
      int new_size = sizeof(*l->data) * new_capacity;

      BUILD_ASSERT(old_size >= 0);
      BUILD_ASSERT(new_size >= 0 && new_size >= old_size);

      tmp = (char**)BUILD_MEM_REALLOC(l->data, new_size, old_size);
      if (!tmp) {
         /* TODO: logging */
         return 0;
      }
      l->data = tmp;
      l->capacity = new_capacity;
   }

   arg_clone = build__strdup(arg);
   if (!arg_clone) {
      /* TODO: logging */
      return 0;
   }
   l->data[l->count++] = arg_clone;
   l->data[l->count] = NULL;

   return 1;
}

BUILD_DEF int build_command_execute(Build_StringList* l)
{
   int result = 0;
#if BUILD_OS_WINDOWS
   result = build__windows_command_execute(l);
#else
   result = 0;
#endif
   return result;
}

BUILD_INTERNAL_DEF int build__windows_command_execute(Build_StringList* l)
{
   return 0; /* TODO: join and quote list of strings and pass string to CreateProcessA */
}

BUILD_INTERNAL_DEF void* build__memcpy(void* destination, const void* source, int n)
{
   int i;
   for (i = 0; i < n; i++) {
      ((char*)destination)[i] = ((const char*)source)[i];
   }
   return destination;
}

BUILD_INTERNAL_DEF char* build__strdup(const char* s)
{
   return build__strndup(s, build__strlen(s));
}

BUILD_INTERNAL_DEF char* build__strndup(const char* s, int n)
{
   char* result = NULL;
   int s_count = (n >= 0) ? (n) : (1);

   if (!s) {
      return NULL;
   }

   result = (char*)BUILD_MEM_ALLOC(sizeof(*result) * (s_count + 1));
   if (!result) {
      return NULL;
   }
   build__memcpy(result, s, s_count);
   result[s_count] = '\0';
   return result;
}

BUILD_INTERNAL_DEF int build__strlen(const char* s)
{
   int len = 0;
   while (*s++) {
      len++;
   }
   return len;
}

#endif /* BUILD_IMPLEMENTATION */
