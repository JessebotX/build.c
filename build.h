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
  DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
  OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef BUILD_H
#define BUILD_H

#define BUILD_VERSION_MAJOR 0
#define BUILD_VERSION_MINOR 1
#define BUILD_VERSION_PATCH 1

#if !BUILD_OS_OTHER
   #if defined(_WIN32) || defined(_WIN64)
      #ifndef BUILD_OS_WINDOWS
         #define BUILD_OS_WINDOWS 1
      #endif
   #else
      #error OS unsupported by default (disable error by setting BUILD_OS_OTHER=1)
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
   #include <stdarg.h>

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

#if BUILD_C_VERSION >= 202311L || BUILD_CPP_VERSION >= 201103L
   #define BUILD__EMPTY_VALUE {}
#else
   #define BUILD__EMPTY_VALUE {0}
#endif

#define BUILD__MAYBE_UNUSED(name) ((void)(name))

#if BUILD_OS_WINDOWS
   #define WIN32_LEAN_AND_MEAN
   #include <Windows.h>
#endif

/**
 * Dynamically-sized null-terminated sequence of bytes (char[]).
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
 * Dynamically-sized null-terminated array containing null-terminated sequences of bytes (char[][]).
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
 * Collection of source files and compiler settings.
 */
typedef struct Build_CompileTarget Build_CompileTarget;
struct Build_CompileTarget {
   char* compiler;
   Build_StringList compile_flags;
   Build_StringList linker_flags;
   Build_StringList source_files;
};
#ifndef BUILD_UNSTRIP_PREFIX
   #define CompileTarget Build_CompileTarget
#endif

#if BUILD_CPP_VERSION
extern "C" {
#endif

/**
 * Create directory at PATH, including missing parent directories.
 */
BUILD_DEF int build_directory_new(const char* path);
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define directory_new build_directory_new
#endif

/**
 * Create a new dynamically-sized null-terminated sequence of bytes
 * from an existing null-terminated string DATA. DATA can be null to
 * create an empty StringBuffer.
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
 * Release all memory of S.
 */
BUILD_DEF void build_string_buffer_delete(Build_StringBuffer* s);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_delete build_string_buffer_delete
#endif

/**
 * Clear contents in S.
 */
BUILD_DEF void build_string_buffer_clear(Build_StringBuffer* s);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_clear build_string_buffer_clear
#endif

/**
 * Append a null-terminated string ARG to a dynamically-sized
 * null-terminated string S.
 */
BUILD_DEF Build_StringBuffer build_string_buffer_append(Build_StringBuffer* s, const char* arg);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_buffer_append build_string_buffer_append
#endif

/**
 * Append string ARG of byte length ARG_COUNT to a dynamically-sized
 * null-terminated string S.
 */
BUILD_DEF Build_StringBuffer build_string_buffer_append_count(Build_StringBuffer* s, const char* arg, int arg_count);
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

#ifndef BUILD_NO_STDINC
/**
 * Create a null-terminated array of null-terminated sequence of bytes
 * (char[][]) from variable number of arguments. The last argument
 * must be NULL.
 */
BUILD_DEF Build_StringList build_string_list_new_varargs(const char* item1, ...);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_new_varargs build_string_list_new_varargs
#endif
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
 * Release all memory in L, including its individual items.
 */
BUILD_DEF void build_string_list_delete_all(Build_StringList* l);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_delete_all build_string_list_delete_all
#endif

/**
 * Release memory of all individual items in L, but keep L's own memory.
 */
BUILD_DEF void build_string_list_delete_items(Build_StringList* l);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_delete_items build_string_list_delete_items
#endif

/**
 * Append a null-terminated string ARG to L.
 */
BUILD_DEF Build_StringList build_string_list_append_string(Build_StringList* l, const char* arg);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_append_string build_string_list_append_string
#endif

#ifndef BUILD_NO_STDINC
/**
 * Append a variable number of null-terminated string arguments to L.
 * The last argument must be NULL.
 */
BUILD_DEF Build_StringList build_string_list_append_string_varargs(Build_StringList* l, ...);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_append_string_varargs build_string_list_append_string_varargs
#endif
#endif

/**
 * Append string ARG of byte length ARG_COUNT to L.
 */
BUILD_DEF Build_StringList build_string_list_append_string_count(Build_StringList* l, const char* arg, int arg_count);
#ifndef BUILD_UNSTRIP_PREFIX
   #define string_list_append_string_count build_string_list_append_string_count
#endif

/**
 * Append ARG to L.
 */
BUILD_DEF Build_StringList build_string_list_append_list(Build_StringList* l, const Build_StringList* arg);
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

/**
 * Create object file from TARGET.
 */
BUILD_DEF int build_target_compile_object(const Build_CompileTarget* target, const char* obj_stem);
#ifndef BUILD_UNSTRIP_PREFIX
   #define target_compile_object build_target_compile_object
#endif

/**
 * Create executable from TARGET.
 */
BUILD_DEF int build_target_compile_executable(const Build_CompileTarget* target, const char* exe_stem);
#ifndef BUILD_UNSTRIP_PREFIX
   #define target_compile_executable build_target_compile_executable
#endif

BUILD_INTERNAL_DEF void* build__memcpy(void* destination, const void* source, int n);
BUILD_INTERNAL_DEF char* build__strdup(const char* s);
BUILD_INTERNAL_DEF char* build__strndup(const char* s, int n);
BUILD_INTERNAL_DEF int build__strlen(const char* s);

#if BUILD_OS_WINDOWS
BUILD_INTERNAL_DEF int build__windows_directory_new(const char* path);
BUILD_INTERNAL_DEF void build__windows_command_list_join(Build_StringList* list, Build_StringBuffer* buf);
BUILD_INTERNAL_DEF int build__windows_process_execute(Build_StringList* cmd);
#endif

#if BUILD_CPP_VERSION
} /* extern "C" */
#endif

#endif /* BUILD_H */

#ifdef BUILD_IMPLEMENTATION

BUILD_DEF int build_directory_new(const char* path)
{
   int result = 0;
#if BUILD_OS_WINDOWS
   result = build__windows_directory_new(path);
#else
   result = 0;
#endif
   return result;
}

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

   BUILD_ASSERT(count >= 0 && count < capacity && "invalid bounds (0 <= count < capacity)");

   result.data = (char*)BUILD_MEM_ALLOC(sizeof(*result.data) * capacity);
   BUILD_ASSERT(result.data && "allocation failure");

   result.capacity = capacity;

   build__memcpy(result.data, data, count);
   result.data[count] = '\0';
   result.count = count;

   return result;
}

BUILD_DEF void build_string_buffer_delete(Build_StringBuffer* s)
{
   if (!s || !s->data) {
      return;
   }

   BUILD_MEM_FREE(s->data, sizeof(*s->data) * s->capacity);
   s->data = NULL;
   s->count = 0;
   s->capacity = 0;
}

BUILD_DEF void build_string_buffer_clear(Build_StringBuffer* s)
{
   if (!s || !s->data) {
      return;
   }

   while (s->count-- > 0) {
      s->data[s->count] = 0;
   }
   s->count = 0;
}

BUILD_DEF Build_StringBuffer build_string_buffer_append(Build_StringBuffer* s, const char* arg)
{
   return build_string_buffer_append_count(s, arg, build__strlen(arg));
}

BUILD_DEF Build_StringBuffer build_string_buffer_append_count(Build_StringBuffer* s, const char* arg, int arg_count)
{
   int new_count;

   if (!s) {
      return build_string_buffer_new_count(arg, arg_count);
   }

   new_count = s->count + arg_count;
   if (s->capacity == 0) {
      int new_capacity = new_count + 1;

      s->data = (char*)BUILD_MEM_ALLOC(sizeof(*s->data) * new_capacity);
      BUILD_ASSERT(s->data && "allocation failure");

      s->capacity = new_capacity;
   } else if (new_count >= s->capacity) {
      int new_capacity = s->capacity * 2;
      int old_size = sizeof(*s->data) * s->capacity;
      int new_size;
      char* tmp;

      BUILD__MAYBE_UNUSED(old_size);

      while (new_capacity <= new_count) {
         new_capacity *= 2;
      }
      new_size = sizeof(*s->data) * new_capacity;

      tmp = (char*)BUILD_MEM_REALLOC(s->data, new_size, old_size);
      BUILD_ASSERT(tmp && "allocation failure");
      s->data = tmp;
      s->capacity = new_capacity;
   }

   build__memcpy(s->data + s->count, arg, arg_count);
   s->count = new_count;
   s->data[new_count] = '\0';

   return *s;
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

#ifndef BUILD_NO_STDINC
BUILD_DEF Build_StringList build_string_list_new_varargs(const char* item1, ...)
{
   va_list args;
   const char* arg;
   Build_StringList result = build_string_list_new(NULL);

   if (!item1) {
      return result;
   }

   build_string_list_append_string(&result, item1);

   va_start(args, item1);
   while((arg = va_arg(args, const char*))) {
      build_string_list_append_string(&result, arg);
   }
   va_end(args);

   return result;
}
#endif

BUILD_DEF Build_StringList build_string_list_new_count(const char* const* data, int count)
{
   return build_string_list_new_capacity(data, count, count + 1);
}

BUILD_DEF Build_StringList build_string_list_new_capacity(const char* const* data, int count, int capacity)
{
   Build_StringList result = BUILD__EMPTY_VALUE;

   BUILD_ASSERT(count >= 0 && count < capacity && "invalid bounds (0 <= count < capacity)");

   result.data = (char**)BUILD_MEM_ALLOC(sizeof(*result.data) * capacity);
   BUILD_ASSERT(result.data && "allocation failure");
   result.capacity = capacity;

   for (result.count = 0; result.count < count; result.count++) {
      char* tmp = build__strdup(data[result.count]);
      result.data[result.count] = tmp;
   }
   result.data[result.count] = NULL;

   return result;
}

BUILD_DEF void build_string_list_delete_all(Build_StringList* l)
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

BUILD_DEF void build_string_list_delete_items(Build_StringList* l)
{
   if (!l || !l->data) {
      return;
   }

   for (; l->count-- > 0;) {
      int i = l->count;
      BUILD__MAYBE_UNUSED(i);

      BUILD_MEM_FREE(l->data[i], sizeof(*l->data[i]) * (build__strlen(l->data[i]) + 1));
   }
   l->count = 0;
   l->data[l->count] = NULL;
}

BUILD_DEF Build_StringList build_string_list_append_string(Build_StringList* l, const char* arg)
{
   return build_string_list_append_string_count(l, arg, build__strlen(arg));
}

#ifndef BUILD_NO_STDINC
BUILD_DEF Build_StringList build_string_list_append_string_varargs(Build_StringList* l, ...)
{
   const char* arg;
   va_list args;
   Build_StringList result;

   va_start(args, l);
   while ((arg = va_arg(args, const char*))) {
      result = build_string_list_append_string(l, arg);
   }
   va_end(args);

   return result;
}
#endif

BUILD_DEF Build_StringList build_string_list_append_string_count(Build_StringList* l, const char* arg, int arg_count)
{
   int new_count;
   char* arg_clone = NULL;

   if (!l) {
      Build_StringList result = build_string_list_new(NULL);
      return build_string_list_append_string_count(&result, arg, arg_count);
   }

   new_count = l->count + 1;
   if (l->capacity == 0) {
      l->data = (char**)BUILD_MEM_ALLOC(sizeof(*l->data) * (new_count + 1));
      BUILD_ASSERT(l->data && "allocation failure");
   } else if (new_count >= l->capacity) {
      char** tmp;
      int new_capacity = l->capacity * 2;
      int old_size = sizeof(*l->data) * l->capacity;
      int new_size = sizeof(*l->data) * new_capacity;

      BUILD_ASSERT(old_size >= 0);
      BUILD_ASSERT(new_size >= 0 && new_size >= old_size);

      tmp = (char**)BUILD_MEM_REALLOC(l->data, new_size, old_size);
      BUILD_ASSERT(tmp && "allocation failure");

      l->data = tmp;
      l->capacity = new_capacity;
   }

   arg_clone = build__strndup(arg, arg_count);
   l->data[l->count++] = arg_clone;
   l->data[l->count] = NULL;

   return *l;
}

BUILD_DEF Build_StringList build_string_list_append_list(Build_StringList* l, const Build_StringList* args)
{
   int old_count;
   int new_count;
   int i;

   if (!l) {
      Build_StringList result = build_string_list_new(NULL);
      return build_string_list_append_list(&result, args);
   }

   if (!args || !args->data || args->count == 0) {
      return *l;
   }

   old_count = l->count;
   new_count = l->count + args->count;
   if (l->capacity == 0) {
      int new_capacity = new_count + 1;

      l->data = (char**)BUILD_MEM_ALLOC(sizeof(*l->data) * new_capacity);
      BUILD_ASSERT(l->data && "allocation failure");

      l->capacity = new_capacity;
   } else if (new_count >= l->capacity) {
      char** tmp;
      int new_capacity = l->capacity * 2;
      int old_size;
      int new_size;

      BUILD__MAYBE_UNUSED(old_size);

      while (new_capacity <= new_count) {
         new_capacity *= 2;
      }
      old_size = sizeof(*l->data) * l->capacity;
      new_size = sizeof(*l->data) * new_capacity;

      tmp = (char**)BUILD_MEM_REALLOC(l->data, new_size, old_size);
      BUILD_ASSERT(tmp && "allocation failure");

      l->data = tmp;
      l->capacity = new_capacity;
   }

   for (i = old_count; i < new_count; i++, l->count++) {
      char* arg_clone = build__strdup(args->data[i - old_count]);
      l->data[i] = arg_clone;
   }
   l->data[l->count] = NULL;

   return *l;
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

BUILD_DEF int build_target_compile_object(const Build_CompileTarget* target, const char* obj_stem)
{
   Build_StringList command_args;
   Build_StringBuffer obj_path;
   int obj_stem_count = build__strlen(obj_stem);
   int result = 0;

   BUILD_ASSERT(target);
   BUILD_ASSERT(obj_stem); /* TODO: default exe name (probably use parent dir?) */

   command_args = build_string_list_new(NULL);

   /* TODO: check if compiler is MSVC (create a macro defining compiler command format) */

   obj_path = build_string_buffer_new_capacity(obj_stem, obj_stem_count, obj_stem_count + sizeof(".o"));
   build_string_buffer_append(&obj_path, ".o");

   build_string_list_append_string(&command_args, target->compiler);

   build_string_list_append_list(&command_args, &target->compile_flags);
   build_string_list_append_list(&command_args, &target->linker_flags);
   build_string_list_append_list(&command_args, &target->source_files);

   build_string_list_append_string(&command_args, "-o");
   build_string_list_append_string_count(&command_args, obj_path.data, obj_path.count);

   if (!build_process_execute(&command_args)) {
      result = 0;
   } else {
      result = 1;
   }

   build_string_buffer_delete(&obj_path);
   build_string_list_delete_all(&command_args);
   return result;
}

BUILD_DEF int build_target_compile_executable(const Build_CompileTarget* target, const char* exe_stem)
{
   Build_StringList command_args;
   Build_StringBuffer exe_path;
   int exe_stem_count = build__strlen(exe_stem);
   int result = 0;

   BUILD_ASSERT(target);
   BUILD_ASSERT(exe_stem); /* TODO: default exe name (probably use parent dir?) */

   command_args = build_string_list_new(NULL);

   /* TODO: check if compiler is MSVC (create a macro defining compiler command format) */

   /* TODO: platform specific function to create exe_path */
   exe_path = build_string_buffer_new_capacity(exe_stem, exe_stem_count, exe_stem_count + sizeof(".exe"));
   build_string_buffer_append(&exe_path, ".exe");

   build_string_list_append_string(&command_args, target->compiler);

   build_string_list_append_list(&command_args, &target->compile_flags);
   build_string_list_append_list(&command_args, &target->linker_flags);
   build_string_list_append_list(&command_args, &target->source_files);

   build_string_list_append_string(&command_args, "-o");
   build_string_list_append_string_count(&command_args, exe_path.data, exe_path.count);

   if (!build_process_execute(&command_args)) {
      result = 0;
   } else {
      result = 1;
   }

   build_string_buffer_delete(&exe_path);
   build_string_list_delete_all(&command_args);
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
   BUILD_ASSERT(result && "allocation failure");

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
BUILD_INTERNAL_DEF int build__windows_directory_new(const char* path)
{
   int result = 1;
   int i = 0;
   char* path_cstr = build__strdup(path);
   int path_len = build__strlen(path);
   char temp_char = '\0';

   for (i = 0; i < path_len; i++) {
      if (path_cstr[i] == '/' || path_cstr[i] == '\\') {
         temp_char = path_cstr[i];
         path_cstr[i] = '\0';
         result = CreateDirectoryA(path_cstr, NULL);
         path_cstr[i] = temp_char;
      } else if (i == (path_len - 1)) {
         result = CreateDirectoryA(path_cstr, NULL);
      }

      if (!result) {
         if (GetLastError() != ERROR_ALREADY_EXISTS) {
            result = 0;
            break;
         } else {
            result = 1;
         }
      }
   }

   BUILD_MEM_FREE(path_cstr, sizeof(*path_cstr) * (path_len + 1));
   return result;
}

BUILD_INTERNAL_DEF int build__windows_process_execute(Build_StringList* args)
{
   STARTUPINFO startup = BUILD__EMPTY_VALUE;
   PROCESS_INFORMATION process = BUILD__EMPTY_VALUE;
   int result = 0;
   Build_StringBuffer command_line;

   BUILD_ASSERT(args);

   command_line = string_buffer_new(NULL);
   build__windows_command_list_join(args, &command_line);

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
      /* TODO: logging */
      goto ret;
   }

   CloseHandle(process.hThread);
   result = 1;

ret:
   string_buffer_delete(&command_line);
   return result;
}

BUILD_INTERNAL_DEF void build__windows_command_list_join(Build_StringList* list, Build_StringBuffer* buf)
{
   int i;

   BUILD_ASSERT(list && list->data && "uninitialized");
   BUILD_ASSERT(buf && buf->data && "uninitialized");

   for (i = 0; i < list->count; i++) {
      int len = build__strlen(list->data[i]);
      int backslashes = 0;
      int j = 0;

      if (list->data[i] == NULL) {
         break;
      }

      if (i > 0) {
         build_string_buffer_append(buf, " ");
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
            build_string_buffer_append_count(buf, list->data[i], len);
            continue;
         }
      }

      build_string_buffer_append(buf, "\"");

      for (j = 0; j < len; j++) {
         char c = list->data[i][j];
         if (c == '\\') {
            backslashes++;
         } else {
            if (c == '\"') {
               int k = 0;
               for (k = 0; k < backslashes + 1; k++) {
                  build_string_buffer_append(buf, "\\");
               }
            }
            backslashes = 0;
         }
         build_string_buffer_append_count(buf, &c, 1);
      }

      for (j = 0; j < backslashes; j++) {
         build_string_buffer_append(buf, "\\");
      }
      build_string_buffer_append(buf, "\"");
   }
}
#endif /* BUILD_OS_WINDOWS */

#endif /* BUILD_IMPLEMENTATION */
