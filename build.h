/* build.h -*- mode: c -*- https://github.com/JessebotX/build.c

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

#if __cplusplus
   #define BUILD_CPP_VERSION __cplusplus
#else
   #ifdef __STDC_VERSION__
      #define BUILD_C_VERSION __STDC_VERSION__
   #else
      #define BUILD_C_VERSION 1
   #endif
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

#if BUILD_OS_WINDOWS
   #define WIN32_LEAN_AND_MEAN
   #include <Windows.h>
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
   char* output_parent_and_stem;
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
 * Create a new dynamically-sized null-terminated sequence of bytes
 * from an existing null-terminated string DATA. DATA can be null to
 * create an emmpty StringBuffer.
 */
BUILD_DEF Build_StringBuffer build_string_buffer_new(const char* data);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_new build_string_buffer_new
#endif

/**
 * Create a new dynamically-sized null-terminated sequence of bytes
 * from an existing string DATA with a byte length of COUNT.
 */
BUILD_DEF Build_StringBuffer build_string_buffer_new_count(const char* data, int count);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_new_count build_string_buffer_new_count
#endif

/**
 * Create a new dynamically-sized null-terminated sequence of bytes
 * from an existing string DATA with a byte length of COUNT. Reserve
 * CAPACITY bytes for future appends.
 */
BUILD_DEF Build_StringBuffer build_string_buffer_new_capacity(const char* data, int count, int capacity);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_new_capacity build_string_buffer_new_capacity
#endif

/**
 * Release all memory of a StringBuffer.
 */
BUILD_DEF void build_string_buffer_free(Build_StringBuffer* s);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_free build_string_buffer_free
#endif

/**
 * Empty out StringBuffer.
 */
BUILD_DEF void build_string_buffer_clear(Build_StringBuffer* s);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_clear build_string_buffer_clear
#endif

/**
 * Append a null-terminated string ARG to a dynamically-sized
 * null-terminated string S. If memory needs to be reallocated, it may
 * fail and return 0.
 */
BUILD_DEF int build_string_buffer_append(Build_StringBuffer* s, const char* arg);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_append build_string_buffer_append
#endif

/**
 * Append string ARG of byte length ARG_COUNT to a dynamically-sized
 * null-terminated string S. If memory needs to be reallocated, it may
 * fail and return 0.
 */
BUILD_DEF int build_string_buffer_append_count(Build_StringBuffer* s, const char* arg, int arg_count);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_append_count build_string_buffer_append_count
#endif

/**
 * Create a null-terminated array of a null-terminated sequence of
 * bytes (char[][]) from an existing null-terminated array of
 * null-terminated strings. DATA can be null to create an empty
 * StringList.
 */
BUILD_DEF Build_StringList build_string_list_new(const char* const* data);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_new build_string_list_new
#endif

/**
 * Create a null-terminated array of a null-terminated sequence of
 * bytes (char[][]) from an existing array of null-terminated strings,
 * where DATA is of size COUNT.
 */
BUILD_DEF Build_StringList build_string_list_new_count(const char* const* data, int count);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_new_count build_string_list_new_count
#endif

/**
 * Create a null-terminated array of a null-terminated sequence of
 * bytes (char[][]) from an existing array of null-terminated strings,
 * where DATA is of size COUNT. Reserve CAPACITY for future appends.
 */
BUILD_DEF Build_StringList build_string_list_new_capacity(const char* const* data, int count, int capacity);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_new_capacity build_string_list_new_capacity
#endif

/**
 * Release memory of the entire StringList, including its individual items.
 */
BUILD_DEF void build_string_list_free_all(Build_StringList* l);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_free_all build_string_list_free_all
#endif

/**
 * Delete all individual items in the StringList.
 */
BUILD_DEF void build_string_list_free_items(Build_StringList* l);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_free_items build_string_list_free_items
#endif

/**
 * Append null-terminated string ARG to StringList L. If memory needs
 * to be reallocated, it may fail and return 0.
 */
BUILD_DEF int build_string_list_append_string(Build_StringList* l, const char* arg);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_append_string build_string_list_append_string
#endif

/**
 * Append string ARG of byte length ARG_COUNT to StringList L. If
 * memory needs to be reallocated, it may fail and return 0.
 */
BUILD_DEF int build_string_list_append_string_count(Build_StringList* l, const char* arg, int arg_count);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_append_string_count build_string_list_append_string_count
#endif

/**
 * Append StringList ARG to StringList L. If memory needs to be
 * reallocated, it may fail and return 0.
 */
BUILD_DEF int build_string_list_append_list(Build_StringList* l, const Build_StringList* arg);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_append_list build_string_list_append_list
#endif

/**
 * Execute a process from a list of arguments CMD.
 */
BUILD_DEF int build_process_execute(Build_StringList* cmd);
#ifndef BUILD_UNSTRIP_PREFIX
   #define process_execute build_process_execute
#endif

BUILD_INTERNAL_DEF void* build__memcpy(void* destination, const void* source, int n);
BUILD_INTERNAL_DEF char* build__strdup(const char* s);
BUILD_INTERNAL_DEF int build__strlen(const char* s);
BUILD_INTERNAL_DEF char* build__strndup(const char* s, int n);

#if BUILD_OS_WINDOWS
BUILD_INTERNAL_DEF int build__windows_command_list_join(Build_StringList* list, Build_StringBuffer* buf);
BUILD_INTERNAL_DEF int build__windows_process_execute(Build_StringList* cmd);
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

BUILD_DEF void build_string_buffer_clear(Build_StringBuffer* s)
{
   if (!s || s->capacity == 0) {
      return;
   }

   while (s->count-- > 0) {
      s->data[s->count] = 0;
   }
   s->count = 0;
}

BUILD_DEF int build_string_buffer_append(Build_StringBuffer* s, const char* arg)
{
   return build_string_buffer_append_count(s, arg, build__strlen(arg));
}

BUILD_DEF int build_string_buffer_append_count(Build_StringBuffer* s, const char* arg, int arg_count)
{
   int new_count;

   if (!s || s->capacity <= 0) {
      /* TODO: logging */
      return 0;
   }

   new_count = s->count + arg_count;
   if (new_count >= s->capacity) {
      int new_capacity = s->capacity * 2;
      int old_size = sizeof(*s->data) * s->capacity;
      int new_size;
      char* tmp;

      while (new_capacity <= new_count) {
         new_capacity *= 2;
      }
      new_size = sizeof(*s->data) * new_capacity;

      tmp = (char*)BUILD_MEM_REALLOC(s->data, new_size, old_size);
      if (!tmp) {
         /* TODO: logging */
         return 0;
      }
      s->data = tmp;
      s->capacity = new_capacity;
   }

   build__memcpy(s->data + s->count, arg, arg_count);
   s->count = new_count;
   s->data[new_count] = '\0';

   return 1;
}

/* TODO: non-null data doesn't seem to work correctly */
BUILD_DEF Build_StringList build_string_list_new(const char* const* data)
{
   int count = 0;
   if (data) {
      while (*data++) {
         count++;
      }
   }
   return build_string_list_new_count(data, count);
}

BUILD_DEF Build_StringList build_string_list_new_count(const char* const* data, int count)
{
   return build_string_list_new_capacity(data, count, count + 1);
}

BUILD_DEF Build_StringList build_string_list_new_capacity(const char* const* data, int count, int capacity)
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

BUILD_DEF void build_string_list_free_items(Build_StringList* l)
{
   if (!l || !l->data) {
      return;
   }

   for (; l->count-- > 0;) {
      int i = l->count;
      BUILD_MEM_FREE(l->data[i], sizeof(*l->data[i]) * (build__strlen(l->data[i]) + 1));
   }
   l->count = 0;
   l->data[l->count] = NULL;
}

BUILD_DEF int build_string_list_append_string(Build_StringList* l, const char* arg)
{
   return build_string_list_append_string_count(l, arg, build__strlen(arg));
}

BUILD_DEF int build_string_list_append_string_count(Build_StringList* l, const char* arg, int arg_count)
{

   int new_count;
   char* arg_clone = NULL;

   BUILD_ASSERT(l && l->data);
   BUILD_ASSERT(arg_count >= 0);

   new_count = l->count + 1;
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

   arg_clone = build__strndup(arg, arg_count);
   if (!arg_clone) {
      /* TODO: logging */
      return 0;
   }
   l->data[l->count++] = arg_clone;
   l->data[l->count] = NULL;

   return 1;
}

BUILD_DEF int build_string_list_append_list(Build_StringList* l, const Build_StringList* args)
{
   int old_count;
   int new_count;
   int i;

   BUILD_ASSERT(l && l->data && l->capacity > 0);
   BUILD_ASSERT(args->data && args->capacity > 0);

   old_count = l->count;
   new_count = l->count + args->count;
   if (new_count >= l->capacity) {
      char** tmp;
      int new_capacity = l->capacity * 2;
      int old_size;
      int new_size;
      while (new_capacity <= new_count) {
         new_capacity *= 2;
      }
      old_size = sizeof(*l->data) * l->capacity;
      new_size = sizeof(*l->data) * new_capacity;

      tmp = (char**)BUILD_MEM_REALLOC(l->data, new_size, old_size);
      if (!tmp) {
         /* TODO: logging */
         return 0;
      }
      l->data = tmp;
      l->capacity = new_capacity;
   }

   for (i = old_count; i < new_count; i++, l->count++) {
      char* arg_clone = build__strdup(args->data[i - old_count]);
      if (!arg_clone) {
         /* TODO: logging */
         return 0;
      }
      l->data[i] = arg_clone;
   }
   l->data[l->count] = NULL;

   return 1;
}

BUILD_DEF int build_process_execute(Build_StringList* cmd)
{
   int result = 0;
#if BUILD_OS_WINDOWS
   result = build__windows_process_execute(cmd);
#else
   result = 0;
#endif
   return result;
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
   if (s) {
      while (*s++) {
         len++;
      }
   }
   return len;
}

#if BUILD_OS_WINDOWS
BUILD_INTERNAL_DEF int build__windows_process_execute(Build_StringList* args)
{
   STARTUPINFO startup = BUILD__EMPTY_VALUE;
   PROCESS_INFORMATION process = BUILD__EMPTY_VALUE;
   BOOL ok = FALSE;
   int result = 0;
   Build_StringBuffer command_line;

   BUILD_ASSERT(args);

   command_line = string_buffer_new(NULL);
   if (!build__windows_command_list_join(args, &command_line)) {
      /* TODO: logging */
      goto ret;
   }

   startup.cb = sizeof(STARTUPINFO);
   startup.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
   if (startup.hStdOutput == INVALID_HANDLE_VALUE) {
      /* TODO: logging */
      goto ret;
   }
   startup.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
   if (startup.hStdInput == INVALID_HANDLE_VALUE) {
      /* TODO: logging */
      goto ret;
   }
   startup.hStdError = GetStdHandle(STD_ERROR_HANDLE);
   if (startup.hStdError == INVALID_HANDLE_VALUE) {
      /* TODO: logging */
      goto ret;
   }
   startup.dwFlags |= STARTF_USESTDHANDLES;

   if (!CreateProcessA(NULL, command_line.data, NULL, NULL, TRUE, 0, NULL, NULL, &startup, &process)) {
      goto ret;
   }

   CloseHandle(process.hThread);
   result = 1;

ret:
   string_buffer_free(&command_line);
   return result;
}

BUILD_INTERNAL_DEF int build__windows_command_list_join(Build_StringList* list, Build_StringBuffer* buf)
{
   int i;

   BUILD_ASSERT(list);
   BUILD_ASSERT(buf);

   for (i = 0; i < list->count; i++) {
      int len = build__strlen(list->data[i]);
      int backslashes = 0;
      int j = 0;

      if (list->data[i] == NULL) {
         break;
      }

      if (i > 0) {
         if (!build_string_buffer_append(buf, " ")) {
            /* TODO: logging */
            return 0;
         }
      }

      if (len != 0) { /* check if string really needs to be quoted/escaped */
         int k;
         for (k = 0; k < len; k++) {
            char it = list->data[i][k];
            if (it == ' ' || it == '\t' || it == '\f' || it == '\"') {
               break;
            }
         }

         if (k >= len) { /* string doesn't need to be quoted/escaped */
            if (!build_string_buffer_append_count(buf, list->data[i], len)) {
               /* TODO: logging */
               return 0;
            }
            continue;
         }
      }

      if (!build_string_buffer_append(buf, "\"")) {
         /* TODO: logging */
         return 0;
      }

      for (j = 0; j < len; j++) {
         char c = list->data[i][j];
         if (c == '\\') {
            backslashes++;
         } else {
            if (c == '\"') {
               int k = 0;
               for (k = 0; k < backslashes + 1; k++) {
                  if (!build_string_buffer_append(buf, "\\")) {
                     /* TODO: logging */
                     return 0;
                  }
               }
            }
            backslashes = 0;
         }
         if (!build_string_buffer_append_count(buf, &c, 1)) {
            /* TODO: logging */
            return 0;
         }
      }

      for (j = 0; j < backslashes; j++) {
         if (!build_string_buffer_append(buf, "\\")) {
            /* TODO: logging */
            return 0;
         }
      }
      if (!build_string_buffer_append(buf, "\"")) {
         /* TODO: logging */
         return 0;
      }
   }

   return 1;
}
#endif /* BUILD_OS_WINDOWS */

#endif /* BUILD_IMPLEMENTATION */
