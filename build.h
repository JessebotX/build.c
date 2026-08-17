/* build.h

  * URL: <https://github.com/JessebotX/build.h>

  Portable single-file C89+/C++ library for writing project build
  recipes.

Quickstart
----------

  my_program.c:

      #include <stdio.h>
      #define Hello_world puts

      int main(void) {
         Hello_world("printf");
         return 0;
      }


  build.c (written in C99+, but build.h supports C89)

      #define BUILD_IMPLEMENTATION
      #include "build.h"

      int main(int argc, char** argv) {
         Artifact artifact = (Artifact){
            .compiler = "clang",
            .compile_options = strlist_from_args("-O0", "-g3", NULL),
            .link_options = strlist_from_args("-lm", NULL),
            .source_files = strlist_from_args("my_program1.c", "my_program2.c", NULL),
         };
         artifact_new_executable(&artifact, "my_program");

         return 0;
      }

  (Windows) Run the following commands to build and execute my_program:

      clang build.c -o build.exe
      build.exe
      my_program.exe

License
-------

  * SPDX-License-Identifier: 0BSD

  Copyright (c) 2026 Jesse <jessebot.git@gmail.com>

  Permission to use, copy, modify, and/or distribute this software for
  any purpose with or without fee is hereby granted.

  THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL
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
#define BUILD_VERSION_MINOR 2
#define BUILD_VERSION_PATCH 0

#ifndef BUILD_DEF
   #define BUILD_DEF
#endif

#ifndef BUILD_INTERNAL
   #define BUILD_INTERNAL static
#endif

#ifndef BUILD_OS_OTHER
   #if defined(_WIN32) || defined(_WIN64)
      #define BUILD_OS_WINDOWS 1
   #elif defined(__linux__)
      #define BUILD_OS_LINUX 1
      #error TODO: Linux support is currently WIP
   #else
      #error OS not supported (disable error by defining BUILD_OS_OTHER)
   #endif
#endif

#if defined(BUILD_OS_WINDOWS)
   /* NOTE: bug with tcc, cant find GetFileSizeEx */
   #define WIN32_LEAN_AND_MEAN
   #include <Windows.h>
#elif defined(BUILD_OS_LINUX)
   #include <sys/stat.h>
   #include <sys/types.h>
   #include <unistd.h>
#endif

#ifndef BUILD_DISABLE_STDINC
   #include <stdarg.h>

   #ifndef BUILD_ASSERT
      #include <assert.h>
      #define BUILD_ASSERT(condition) assert((condition))
   #endif

   #if !defined(BUILD_MEM_ALLOC) || !defined(BUILD_MEM_REALLOC) || !defined(BUILD_MEM_FREE)
      #ifdef BUILD_OS_WINDOWS
         #ifndef BUILD_MEM_ALLOC
            #define BUILD_MEM_ALLOC(n) \
               HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (n))
         #endif

         #ifndef BUILD_MEM_REALLOC
            #define BUILD_MEM_REALLOC(ptr, n, n_old) \
               HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (ptr), (n))
         #endif

         #ifndef BUILD_MEM_FREE
            #define BUILD_MEM_FREE(ptr, n) \
               HeapFree(GetProcessHeap(), 0, (void*)(ptr))
         #endif
      #else
         #include <stdlib.h>

         #ifndef BUILD_MEM_ALLOC
            #define BUILD_MEM_ALLOC(n) malloc((n))
         #endif

         #ifndef BUILD_MEM_REALLOC
            #define BUILD_MEM_REALLOC(ptr, n, n_old) realloc((ptr), (n))
         #endif

         #ifndef BUILD_MEM_FREE
            #define BUILD_MEM_FREE(ptr, n) free((void*)(ptr))
         #endif
      #endif
   #endif
#endif

#if __STDC_VERSION__ >= 202311L
   #define BUILD__EMPTY_VALUE {}
#else
   #define BUILD__EMPTY_VALUE {0}
#endif

#ifndef NULL
   #if __STDC_VERSION__ >= 202311L
      #define NULL nullptr
   #else
      #define NULL ((void*)0)
   #endif
#endif

/**
 * A dynamically-sized null-terminated string (array of bytes).
 */
typedef struct Build_StrBuf Build_StrBuf;
struct Build_StrBuf {
   char* bytes;
   int len;
   int cap;
};
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define StrBuf Build_StrBuf
#endif

/**
 * A dynamically-sized null-terminated array of null-terminated strings
 * (i.e. char[][]).
 */
typedef struct Build_StrList Build_StrList;
struct Build_StrList {
   const char** bytes;
   int len;
   int cap;
};
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define StrList Build_StrList
#endif

/**
 * Options and source files to make a build artifact.
 */
typedef struct Build_Artifact Build_Artifact;
struct Build_Artifact {
   const char* compiler;
   const char* linker; /* default: use compiler */
   const char** compile_command_format; /* default: "#cc", "#cflags", "-c", "#in", "-o", "#out", NULL */
   const char** link_command_format; /* default: "#cc", "#ldflags", "#in", "-o", "#out", NULL */
   Build_StrList compile_options;
   Build_StrList link_options;
   Build_StrList source_files;
};
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define Artifact Build_Artifact
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create a new executable from ARTIFACT with the name of EXE_NAME.
 *
 * NOTE: On Windows, the .exe file extension will automatically be
 * appended onto the name.
 */
BUILD_DEF int build_artifact_new_executable(Build_Artifact* artifact, const char* exe_name);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define artifact_new_executable build_artifact_new_executable
#endif

/**
 * Execute an external process on its own separate thread. Returns a
 * non-zero value on success, 0 on failure.
 */
BUILD_DEF int build_process_execute(const Build_StrList* args);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define process_execute build_process_execute
#endif

/**
 * Execute an external process on its own separate thread. Returns a
 * non-zero value on success, 0 on failure. Use existing COMMAND_BUF
 * to construct command as string if necessary.
 */
BUILD_DEF int build_process_execute_b(const Build_StrList* args, Build_StrBuf* command_buf);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define process_execute_b build_process_execute_b
#endif

/**
 * Execute an external process on its own separate thread. Returns a
 * non-zero value on success, 0 on failure.
 */
BUILD_DEF int build_process_execute_c(const char* args[]);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define process_execute_c build_process_execute_c
#endif

/**
 * Execute an external process on its own separate thread. Returns a
 * non-zero value on success, 0 on failure. Use existing COMMAND_BUF
 * to construct command as string if necessary.
 */
BUILD_DEF int build_process_execute_c_b(const char* args[], Build_StrBuf* command_buf);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define process_execute_c_b build_process_execute_c_b
#endif

/**
 * Read entire file at PATH.
 */
BUILD_DEF Build_StrBuf build_file_read_all(const char* path);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define file_read_all build_file_read_all
#endif

/**
 * Read entire file at PATH into BUF. Returns the number of bytes read
 * into BUF, and returns a number < 0 on error.
 */
BUILD_DEF int build_file_read_all_b(const char* path, Build_StrBuf* buf);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define file_read_all_b build_file_read_all_b
#endif

/**
 * Create a new directory at PATH, and creates parent path elements if
 * necessary. If PATH is not absolute, it will be relative to the
 * current directory.
 */
BUILD_DEF int build_directory_new(const char* path);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define directory_new build_directory_new
#endif

/**
 * Set current directory to PATH.
 */
BUILD_DEF int build_directory_set(const char* path);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define directory_set build_directory_set
#endif

/**
 * Execute an external process on its own separate thread. LEN is the
 * number of arguments provided (excluding the null terminator).
 * Returns a non-zero value on success, 0 on failure.
 */
BUILD_DEF int build_process_execute_c_len(const char* args[], int len);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define process_execute_c_len build_process_execute_c_len
#endif

/**
 * Create a new dynamically-sized null-terminated string from a
 * null-terminated sequence of bytes S.
 */
BUILD_DEF Build_StrBuf build_strbuf_from_c(const char* s);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strbuf_from_c build_strbuf_from_c
#endif

/**
 * Create a new dynamically-sized null-terminated string from a
 * sequence of bytes S that is LEN bytes in length.
 */
BUILD_DEF Build_StrBuf build_strbuf_from_c_len(const char* s, int len);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strbuf_from_c_len build_strbuf_from_c_len
#endif

/**
 * Create a new dynamically-sized null-terminated string from a string
 * literal.
 */
#define BUILD_STRBUF_FROM_LITERAL(l, arg) build_strbuf_from_c_len((l), (arg), sizeof((arg)) - 1)
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define STRBUF_FROM_LITERAL BUILD_STRBUF_FROM_LITERAL
#endif

/**
 * Create a new dynamically-sized null-terminated string from a
 * sequence of bytes S that is LEN bytes in length. Reserve CAP
 * amount of bytes for future modifications.
 */
BUILD_DEF Build_StrBuf build_strbuf_from_c_reserve(const char* s, int len, int cap);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strbuf_from_c_reserve build_strbuf_from_c_reserve
#endif

/**
 * Resize capacity of S to NEW_CAP. NEW_CAP must be greater than the len of S.
 */
BUILD_DEF void build_strbuf_resize(Build_StrBuf* s, int new_cap);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strbuf_from_c_reserve build_strbuf_from_c_reserve
#endif

/**
 * Clear contents of S.
 */
BUILD_DEF void build_strbuf_clear(Build_StrBuf* s);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strbuf_clear build_strbuf_clear
#endif

/**
 * Delete contents and deallocate all memory of S.
 */
BUILD_DEF void build_strbuf_free(Build_StrBuf* s);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strbuf_free build_strbuf_free
#endif

/**
 * Append null-terminated string ARG to S
 */
BUILD_DEF void build_strbuf_append_c(Build_StrBuf* s, const char* arg);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strbuf_append_c build_strbuf_append_c
#endif

/**
 * Append string ARG of byte length LEN to S
 */
BUILD_DEF void build_strbuf_append_c_len(Build_StrBuf* s, const char* arg, int len);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strbuf_append_c_len build_strbuf_append_c_len
#endif

/**
 * Append string literal ARG to S.
 */
#define BUILD_STRBUF_APPEND_LITERAL(l, arg) build_strbuf_append_c_len((l), (arg), sizeof((arg)) - 1)
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define STRBUF_APPEND_LITERAL BUILD_STRBUF_APPEND_LITERAL
#endif

#ifndef BUILD_DISABLE_STDINC
/**
 * Create a null-terminated array of null-terminated strings from
 * multiple null-terminated string arguments. The last argument must be
 * null.
 */
BUILD_DEF Build_StrList build_strlist_from_args(const char* arg_0, ...);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strlist_from_args build_strlist_from_args
#endif
#endif /* BUILD_DISABLE_STDINC */

/**
 * Create a null-terminated array of null-terminated strings from an
 * existing null-terminated array L.
 */
BUILD_DEF Build_StrList build_strlist_from_c(const char* l[]);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strlist_from_c build_strlist_from_c
#endif

/**
 * Create a null-terminated array of null-terminated strings from an
 * existing array L containing LEN amount of null-terminated strings.
 */
BUILD_DEF Build_StrList build_strlist_from_c_len(const char* l[], int len);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strlist_from_c_len build_strlist_from_c_len
#endif

/**
 * Create a null-terminated array of null-terminated strings from an
 * existing array L containing LEN amount of null-terminated strings.
 * Reserve CAP amount of space for null-terminated strings for future
 * modifications.
 */
BUILD_DEF Build_StrList build_strlist_from_c_reserve(const char* l[], int len, int cap);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strlist_from_c_reserve build_strlist_from_c_reserve
#endif

/**
 * Clear all contents in L without freeing its memory capacity.
 */
BUILD_DEF void build_strlist_clear(Build_StrList* l);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strlist_clear build_strlist_clear
#endif

/**
 * Delete and deallocate the buffer and all the items within it in L.
 */
BUILD_DEF void build_strlist_free(Build_StrList* l);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strlist_free build_strlist_free
#endif

/**
 * Append null-terminated string ARG to L.
 */
BUILD_DEF void build_strlist_append_c(Build_StrList* l, const char* arg);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strlist_append_c build_strlist_append_c
#endif

/**
 * Append string ARG of byte length LEN to L.
 */
BUILD_DEF void build_strlist_append_c_len(Build_StrList* l, const char* arg, int len);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strlist_append_c_len build_strlist_append_c_len
#endif

/**
 * Append string literal ARG to L.
 */
#define BUILD_STRLIST_APPEND_LITERAL(l, arg) build_strlist_append_c_len((l), (arg), sizeof((arg)) - 1)
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define STRLIST_APPEND_LITERAL BUILD_STRLIST_APPEND_LITERAL
#endif

#ifndef BUILD_DISABLE_STDINC
/**
 * Append multiple null-terminated strings to L. Final argument must
 * be null.
 */
BUILD_DEF void build_strlist_append_args(Build_StrList* l, const char* arg0, ...);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strlist_append_args build_strlist_append_args
#endif
#endif /* BUILD_DISABLE_STDINC */

/**
 * Append all items in ARG to L.
 */
BUILD_DEF void build_strlist_append_l(Build_StrList* l, const Build_StrList* arg);
#ifndef BUILD_DISABLE_SHORT_NAMES
   #define strlist_append_l build_strlist_append_l
#endif

#ifdef BUILD_OS_WINDOWS
BUILD_INTERNAL int build__directory_set_windows(const char* path);
BUILD_INTERNAL int build__process_execute_windows(const char* args[], Build_StrBuf* buf);
BUILD_INTERNAL int build__directory_new_windows(const char* path);
BUILD_INTERNAL Build_StrBuf build__file_read_all_windows(const char* path);
#endif

BUILD_INTERNAL void* build__memcpy(void* destination, const void* source, int n);
BUILD_INTERNAL int build__strlen(const char* s);
BUILD_INTERNAL char* build__strdup(const char* s);
BUILD_INTERNAL char* build__strndup(const char* s, int n);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BUILD_H */

#ifdef BUILD_IMPLEMENTATION

BUILD_DEF int build_artifact_new_executable(Build_Artifact* artifact, const char* exe_name)
{
   int result = 0;
   int i;
   Build_StrList command_args = BUILD__EMPTY_VALUE;
   Build_StrList objects = BUILD__EMPTY_VALUE;
   Build_StrBuf output_path = BUILD__EMPTY_VALUE;
   Build_StrBuf command_buf = BUILD__EMPTY_VALUE;
   BUILD_ASSERT(artifact && "missing artifact");
   BUILD_ASSERT(exe_name && "missing exe name"); /* TODO: default to root dir name */

   /* TODO: utitize *_command_format strings */

   /* #cc# #cflags# */
   build_strlist_append_c(&command_args, artifact->compiler);
   build_strlist_append_l(&command_args, &artifact->compile_options);

   if (artifact->source_files.len > 0) {
      int source_index;
      int output_index;

      /* ... -c #in -o #out */
      BUILD_STRLIST_APPEND_LITERAL(&command_args, "-c");
      BUILD_STRLIST_APPEND_LITERAL(&command_args, "placeholder"); /* placeholder index for source file */
      source_index = command_args.len - 1;
      BUILD_STRLIST_APPEND_LITERAL(&command_args, "-o");
      BUILD_STRLIST_APPEND_LITERAL(&command_args, "placeholder"); /* placeholder index for output file */
      output_index = command_args.len - 1;

      for (i = 0; i < artifact->source_files.len; i++) {
         /* source path */
         BUILD_MEM_FREE(command_args.bytes[source_index], (build__strlen(command_args.bytes[source_index]) + 1) * sizeof(*command_args.bytes));
         command_args.bytes[source_index] = build__strdup(artifact->source_files.bytes[i]);

         /* output object path */
         build_strbuf_clear(&output_path);
         BUILD_MEM_FREE(command_args.bytes[output_index], (build__strlen(command_args.bytes[output_index]) + 1) * sizeof(*command_args.bytes));
         build_strbuf_append_c(&output_path, artifact->source_files.bytes[i]);
         BUILD_STRBUF_APPEND_LITERAL(&output_path, ".o");
         command_args.bytes[output_index] = build__strndup(output_path.bytes, output_path.len);

         /* run command */
         result = build_process_execute_b(&command_args, &command_buf);
         BUILD_ASSERT(result && "running process failed");

         /* add to object list for linking */
         build_strlist_append_c_len(&objects, output_path.bytes, output_path.len);
      }
   }

   build_strbuf_clear(&output_path);
   build_strlist_clear(&command_args);

   build_strbuf_append_c(&output_path, exe_name);
#ifdef BUILD_OS_WINDOWS
   BUILD_STRBUF_APPEND_LITERAL(&output_path, ".exe");
#endif

   if (!artifact->linker) {
      artifact->linker = artifact->compiler;
   }

   build_strlist_append_c(&command_args, artifact->linker);
   build_strlist_append_l(&command_args, &artifact->link_options);
   build_strlist_append_l(&command_args, &objects);
   BUILD_STRLIST_APPEND_LITERAL(&command_args, "-o");
   build_strlist_append_c(&command_args, output_path.bytes);

   result = build_process_execute_b(&command_args, &command_buf);

   build_strbuf_free(&command_buf);
   build_strbuf_free(&output_path);
   build_strlist_free(&objects);
   build_strlist_free(&command_args);

   return result;
}

BUILD_DEF int build_process_execute(const Build_StrList* args)
{
   Build_StrBuf command_buf = BUILD__EMPTY_VALUE;
   return build_process_execute_c_b(args->bytes, &command_buf);
}

BUILD_DEF int build_process_execute_b(const Build_StrList* args, Build_StrBuf* command_buf)
{
   BUILD_ASSERT(command_buf && "missing strbuf");
   if (!args) {
      return 0;
   }
   build_strbuf_clear(command_buf);
   return build_process_execute_c_b(args->bytes, command_buf);
}

BUILD_DEF int build_process_execute_c(const char* args[])
{
   int result = 0;
   Build_StrBuf command_buf = BUILD__EMPTY_VALUE;

   result = build_process_execute_c_b(args, &command_buf);
   return result;
}

BUILD_DEF int build_process_execute_c_b(const char* args[], Build_StrBuf* command_buf)
{
   int result = 0;
   BUILD_ASSERT(command_buf && "missing strbuf");

   build_strbuf_clear(command_buf);
#ifdef BUILD_OS_WINDOWS
   {
      result = build__process_execute_windows(args, command_buf);
   }
#else
   result = 0;
#endif
   return result;
}

BUILD_DEF int build_process_execute_c_len(const char* args[], int len)
{
   int result = 0;
   Build_StrList l = build_strlist_from_c_len(args, len);

   result = build_process_execute_c(l.bytes);
   build_strlist_free(&l);
   return result;
}

BUILD_DEF Build_StrBuf build_file_read_all(const char* path)
{
   Build_StrBuf result = BUILD__EMPTY_VALUE;

   if (build_file_read_all_b(path, &result) < 0) {
      build_strbuf_free(&result);
   }
   return result;
}

BUILD_DEF int build_file_read_all_b(const char* path, Build_StrBuf* buf)
{
   int result = -1;
   HANDLE file;
   LARGE_INTEGER file_size;
   DWORD bytes_read = 0;
   BUILD_ASSERT(buf && "missing strbuf");

   file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
   if (file == INVALID_HANDLE_VALUE) {
      goto ret;
   }

   if (!GetFileSizeEx(file, &file_size)) {
      goto ret_cleanup;
   }

   build_strbuf_resize(buf, file_size.QuadPart + 1);
   if (!ReadFile(file, buf->bytes, (DWORD)file_size.QuadPart, &bytes_read, NULL)) {
      buf->len = bytes_read;
      goto ret_cleanup;
   }
   buf->len = bytes_read;

   result = bytes_read;
ret_cleanup:
   CloseHandle(file);
ret:
   return bytes_read;
}

BUILD_DEF int build_directory_new(const char* path)
{
   int result = 0;
#ifdef BUILD_OS_WINDOWS
   result = build__directory_new_windows(path);
#else
   result = 0;
#endif
   return result;
}

BUILD_DEF int build_directory_set(const char* path)
{
   int result = 0;
#ifdef BUILD_OS_WINDOWS
   result = build__directory_set_windows(path);
#else
   result = 0;
#endif
   return result;
}

BUILD_DEF Build_StrBuf build_strbuf_from_c(const char* s)
{
   return build_strbuf_from_c_len(s, build__strlen(s));
}

BUILD_DEF Build_StrBuf build_strbuf_from_c_len(const char* s, int len)
{
   return build_strbuf_from_c_reserve(s, len, len + 1);
}

BUILD_DEF Build_StrBuf build_strbuf_from_c_reserve(const char* s, int len, int cap)
{
   Build_StrBuf result;
   BUILD_ASSERT(len >= 0 && len < cap);

   result.bytes = (char*)BUILD_MEM_ALLOC(cap * sizeof(*result.bytes));
   BUILD_ASSERT(result.bytes && "failed to allocate memory");
   result.cap = cap;
   build__memcpy(result.bytes, s, len);
   result.len = len;
   result.bytes[result.len] = '\0';
   return result;
}

BUILD_DEF void build_strbuf_resize(Build_StrBuf* s, int new_cap)
{
   BUILD_ASSERT(s && "missing strbuf");
   BUILD_ASSERT(new_cap > 0 && new_cap > s->len);

   if (s->cap == 0) {
      s->bytes = (char*)BUILD_MEM_ALLOC(new_cap * sizeof(*s->bytes));
      BUILD_ASSERT(s->bytes && "failed to allocate memory");
   } else {
      s->bytes = (char*)BUILD_MEM_REALLOC(s->bytes, new_cap * sizeof(*s->bytes), s->cap * sizeof(*s->bytes));
      BUILD_ASSERT(s->bytes && "failed to reallocate memory");
   }

   s->cap = new_cap;
}

BUILD_DEF void build_strbuf_clear(Build_StrBuf* s)
{
   BUILD_ASSERT(s && "missing strbuf");

   while (s->len-- > 0) {
      s->bytes[s->len] = 0;
   }
   s->len = 0;
}

BUILD_DEF void build_strbuf_free(Build_StrBuf* s)
{
   BUILD_ASSERT(s && "missing strbuf");

   if (s->bytes) {
      BUILD_MEM_FREE(s->bytes, s->cap * sizeof(*s->bytes));
      s->bytes = NULL;
   }
   s->len = 0;
   s->cap = 0;
}

BUILD_DEF void build_strbuf_append_c(Build_StrBuf* s, const char* arg)
{
   build_strbuf_append_c_len(s, arg, build__strlen(arg));
}

BUILD_DEF void build_strbuf_append_c_len(Build_StrBuf* s, const char* arg, int len)
{
   int new_len;
   BUILD_ASSERT(s && "missing strbuf");

   new_len = s->len + len;
   if (s->cap == 0) {
      s->bytes = (char*)BUILD_MEM_ALLOC((new_len + 1) * sizeof(*s->bytes));
      BUILD_ASSERT(s->bytes && "failed to allocate memory");
      s->cap = new_len + 1;
   } else if (new_len >= s->cap) {
      int new_cap = s->cap * 2;
      while (new_cap <= new_len) {
         new_cap *= 2;
         BUILD_ASSERT(new_cap > 0);
      }

      s->bytes = (char*)BUILD_MEM_REALLOC(s->bytes, new_cap * sizeof(*s->bytes), s->cap * sizeof(*s->bytes));
      BUILD_ASSERT(s->bytes);
      s->cap = new_cap;
   }

   memcpy(s->bytes + s->len, arg, len);
   s->len = new_len;
   s->bytes[s->len] = '\0';
}


#ifndef BUILD_DISABLE_STDINC
BUILD_DEF Build_StrList build_strlist_from_args(const char* arg_0, ...)
{
   va_list args;
   const char* arg;
   Build_StrList result = BUILD__EMPTY_VALUE;

   if (!arg_0) {
      return build_strlist_from_c(NULL);
   }

   build_strlist_append_c(&result, arg_0);

   va_start(args, arg_0);
   while ((arg = va_arg(args, const char*))) {
      build_strlist_append_c(&result, arg);
   }
   va_end(args);

   return result;
}
#endif /* BUILD_DISABLE_STDINC */

BUILD_DEF Build_StrList build_strlist_from_c(const char* l[])
{
   int len = 0;
   if (l) {
      while (l[len]) {
         len++;
      }
   }
   return build_strlist_from_c_len(l, len);
}

BUILD_DEF Build_StrList build_strlist_from_c_len(const char* l[], int len)
{
   return build_strlist_from_c_reserve(l, len, len + 1);
}

BUILD_DEF Build_StrList build_strlist_from_c_reserve(const char* l[], int len, int cap)
{
   Build_StrList result;
   BUILD_ASSERT(len >= 0 && len < cap);

   result.bytes = (const char**)BUILD_MEM_ALLOC(cap * sizeof(*result.bytes));
   BUILD_ASSERT(result.bytes && "failed to allocate memory");
   result.cap = cap;
   for (result.len = 0; result.len < len; result.len++) {
      result.bytes[result.len] = build__strdup(l[result.len]);
   }
   result.bytes[result.len] = NULL;
   return result;
}

BUILD_DEF void build_strlist_clear(Build_StrList* l)
{
   BUILD_ASSERT(l && "missing strlist");

   while (l->len-- > 0) {
      int i = l->len;
      BUILD_MEM_FREE(l->bytes[i], (build__strlen(l->bytes[i]) + 1) * sizeof(*l->bytes[i]));
   }
   l->len = 0;
}

BUILD_DEF void build_strlist_free(Build_StrList* l)
{
   int i;
   BUILD_ASSERT(l && "missing strlist");

   if (l->bytes) {
      for (i = 0; i < l->len; i++) {
         BUILD_MEM_FREE(l->bytes[i], (build__strlen(l->bytes[i]) + 1) * sizeof(*l->bytes[i]));
      }
      BUILD_MEM_FREE(l->bytes, l->cap * sizeof(*l->bytes));
   }
   l->bytes = NULL;
   l->len = 0;
   l->cap = 0;
}

BUILD_DEF void build_strlist_append_c(Build_StrList* l, const char* arg)
{
   build_strlist_append_c_len(l, arg, build__strlen(arg));
}

BUILD_DEF void build_strlist_append_c_len(Build_StrList* l, const char* arg, int len)
{
   char* clone;
   BUILD_ASSERT(l && "missing strlist");

   if (l->cap == 0) {
      l->bytes = (const char**)BUILD_MEM_ALLOC(2 * sizeof(*l->bytes));
      BUILD_ASSERT(l->bytes && "failed to allocate memory");
      l->len = 0;
      l->cap = 2;
      l->bytes[0] = NULL;
   } else if (l->len + 1 >= l->cap) {
      BUILD_ASSERT(l->cap * 2 > l->len + 1 && "overflow");
      l->bytes = (const char**)BUILD_MEM_REALLOC(l->bytes, (l->cap * 2) * sizeof(*l->bytes), l->cap * sizeof(*l->bytes));
      BUILD_ASSERT(l->bytes && "failed to reallocate memory");
      l->cap *= 2;
   }
   clone = build__strndup(arg, len);
   l->bytes[l->len++] = clone;
   l->bytes[l->len] = NULL;
}

#ifndef BUILD_DISABLE_STDINC
BUILD_DEF void build_strlist_append_args(Build_StrList* l, const char* arg_0, ...)
{
   va_list args;
   const char* arg;
   BUILD_ASSERT(l && "missing strlist");

   if (!arg_0) {
      return;
   }

   build_strlist_append_c(l, arg_0);

   va_start(args, arg_0);
   while ((arg = va_arg(args, const char*))) {
      build_strlist_append_c(l, arg);
   }
   va_end(args);
}
#endif /* BUILD_DISABLE_STDINC */

/**
 * Append all items in ARG to L.
 */
BUILD_DEF void build_strlist_append_l(Build_StrList* l, const Build_StrList* arg)
{
   int i;
   int new_len;

   if (!l) {
      return;
   }

   if (!arg || arg->cap == 0) {
      return;
   }

   /* preallocate memory if necessary */
   new_len = l->len + arg->len;
   if (l->cap == 0) {
      int new_cap = new_len + 1;
      l->bytes = (const char**)BUILD_MEM_ALLOC(new_cap * sizeof(*l->bytes));
      BUILD_ASSERT(l->bytes && "failed to allocate memory");
      l->cap = new_cap;
   } else if (l->cap <= new_len) {
      int new_cap = l->cap * 2;
      while (new_cap <= new_len) {
         new_cap *= 2;
         BUILD_ASSERT(new_cap > l->cap && "overflow");
      }
      l->bytes = (const char**)BUILD_MEM_REALLOC(l->bytes, new_cap * sizeof(*l->bytes), l->cap * sizeof(*l->bytes));
      BUILD_ASSERT(l->bytes && "failed to reallocate memory");
      l->cap = new_cap;
   }

   for (i = 0; i < arg->len; i++) {
      build_strlist_append_c(l, arg->bytes[i]);
   }
}

#ifdef BUILD_OS_WINDOWS
BUILD_INTERNAL int build__directory_set_windows(const char* path)
{
   return SetCurrentDirectory(path);
}

BUILD_INTERNAL int build__process_execute_windows(const char* args[], Build_StrBuf* buf)
{
   int result = 0;
   int i;
   STARTUPINFO startup = BUILD__EMPTY_VALUE;
   PROCESS_INFORMATION process = BUILD__EMPTY_VALUE;
   BUILD_ASSERT(buf && "missing strbuf");

   if (!args) {
      goto ret;
   }

   ZeroMemory(&startup, sizeof(startup));
   ZeroMemory(&process, sizeof(process));
   build_strbuf_clear(buf);

   for (i = 0; args[i]; i++) { /* join and escape args */
      int len = build__strlen(args[i]);
      int backslashes = 0;
      int j = 0;

      if (i > 0) {
         build_strbuf_append_c(buf, " ");
      }

      if (len != 0) {
         for (j = 0; j < len; j++) {
            char it = args[i][j];
            if (it == ' ' || it == '\t' || it == '\f' || it == '\"') {
               break;
            }
         }

         if (j >= len) {
            build_strbuf_append_c_len(buf, args[i], len);
            continue;
         }
      }

      build_strbuf_append_c(buf, "\"");

      for (j = 0; j < len; j++) {
         char c = args[i][j];
         if (c == '\\') {
            backslashes++;
         } else {
            if (c == '\"') {
               int k;
               for (k = 0; k < backslashes + 1; k++) {
                  build_strbuf_append_c(buf, "\\");
               }
            }
            backslashes = 0;
         }
         build_strbuf_append_c_len(buf, &c, 1);
      }

      for (j = 0; j < backslashes; j++) {
         build_strbuf_append_c(buf, "\\");
      }
      build_strbuf_append_c(buf, "\"");
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

   if (!CreateProcessA(NULL, buf->bytes, NULL, NULL, TRUE, 0, NULL, NULL, &startup, &process)) {
      /* TODO: logging */
      goto ret;
   }

   WaitForSingleObject(process.hProcess, INFINITE);
   CloseHandle(process.hProcess);
   CloseHandle(process.hThread);
   result = 1;
ret:
   return result;
}

BUILD_INTERNAL int build__directory_new_windows(const char* path)
{
   int result = 1;
   int i = 0;
   int path_len = build__strlen(path);
   char* path_cstr = build__strndup(path, path_len);
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
         DWORD err = GetLastError();
         if (err != ERROR_ALREADY_EXISTS || err != ERROR_PATH_NOT_FOUND) {
            /* TODO: logging */
            result = 0;
            goto ret;
         }
      }
   }

ret:
   BUILD_MEM_FREE(path_cstr, (path_len + 1) * sizeof(*path_cstr));
   return result;
}
#endif

BUILD_INTERNAL void* build__memcpy(void* destination, const void* source, int n)
{
   int i;
   BUILD_ASSERT(n >= 0);

   for (i = 0; i < n; i++) {
      ((char*)destination)[i] = ((const char*)source)[i];
   }
   return destination;
}

BUILD_INTERNAL int build__strlen(const char* s)
{
   int len = 0;
   if (s) {
      while (*s++) {
         len++;
      }
   }
   return len;
}

BUILD_INTERNAL char* build__strdup(const char* s)
{
   return build__strndup(s, build__strlen(s));
}

BUILD_INTERNAL char* build__strndup(const char* s, int n)
{
   char* result = NULL;
   BUILD_ASSERT(n >= 0);

   result = (char*)BUILD_MEM_ALLOC((n + 1) * sizeof(*result));
   BUILD_ASSERT(result && "failed to allocate memory");
   build__memcpy(result, s, n);
   result[n] = '\0';
   return result;
}

#endif /* BUILD_IMPLEMENTATION */
