/* -*- mode: c; -*- */

#ifndef BUILD_H_
#define BUILD_H_

#if !defined(NULL)
   #define NULL ((void*)0)
#endif

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

typedef struct Build_Program {
   char* name;
   char** argv;
   int argc;
} Build_Program;
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define Program Build_Program
#endif

typedef struct Build_StringList {
   char** data;
   int count;
   int capacity;
} Build_StringList;
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define StringList Build_StringList
#endif

typedef struct Build_StringBuffer {
   char* data;
   int count;
   int capacity;
} Build_StringBuffer;
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define StringBuffer Build_StringBuffer
#endif

BUILD_DEF char* build_append_to_string_buffer(Build_StringBuffer* buf, const char* arg);
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define append_to_string_buffer build_append_to_string_buffer
#endif

BUILD_DEF void build_free_string_buffer(Build_StringBuffer* buf);
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define free_string_buffer build_free_string_buffer
#endif

BUILD_DEF int build_append_to_string_list(Build_StringList* list, const char* arg);
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define append_to_string_list build_append_to_string_list
#endif

BUILD_DEF void build_free_string_list(Build_StringList* list);
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define free_string_list build_free_string_list
#endif

BUILD_DEF int build_set_current_directory(const char* path);
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define set_current_directory build_set_current_directory
#endif

BUILD_DEF int build_new_directory(const char* path);
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define new_directory build_new_directory
#endif

/* TODO: implement */
BUILD_DEF int build_delete_directory(const char* path);
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define delete_directory build_delete_directory
#endif

BUILD_DEF int build_count_bytes(const char* s);
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define count_bytes build_count_bytes
#endif

BUILD_DEF char* build_clone_bytes(const char* s);
#if !defined(BUILD_UNSTRIP_PREFIX)
   #define clone_bytes build_clone_bytes
#endif

#endif /* BUILD_H_ */

#ifdef BUILD_IMPLEMENTATION
#undef BUILD_IMPLEMENTATION

BUILD_DEF char* build_append_to_string_buffer(Build_StringBuffer* buf, const char* arg)
{
   int arg_count = count_bytes(arg);
   int new_count = 0;
   int i = 0;

   BUILD_ASSERT(buf);

   new_count = buf->count + arg_count;

   if (buf->capacity <= 0) {
      int new_capacity = arg_count + 1;
      buf->data = (char*)BUILD_MEM_ALLOC(sizeof(*buf->data) * new_capacity);
      if (!buf->data) {
         return NULL;
      }
      buf->capacity = new_capacity;
   } else if (buf->capacity <= new_count) {
      char* tmp = NULL;
      int new_capacity = buf->capacity * 2;
      while (new_capacity <= new_count) {
         new_capacity = new_capacity * 2;
      }
      tmp = (char*)BUILD_MEM_REALLOC(buf->data, sizeof(*buf->data) * new_capacity);
      if (!tmp) {
         return NULL;
      }
      buf->data = tmp;
      buf->capacity = new_capacity;
   }

   for (i = 0; i < arg_count; i++) {
      (buf->data + buf->count)[i] = arg[i];
   }
   buf->data[new_count] = '\0';
   buf->count = new_count;

   return buf->data;
}

BUILD_DEF void build_free_string_buffer(Build_StringBuffer* buf)
{
   if (!buf) {
      return;
   }

   BUILD_MEM_FREE(buf->data);
   buf->data = NULL;
   buf->count = 0;
   buf->capacity = 0;
}

BUILD_DEF int build_append_to_string_list(Build_StringList* list, const char* item)
{
   char* item_clone = NULL;

   BUILD_ASSERT(list);

   if (list->capacity == 0) {
      int new_capacity = 2;
      list->data = (char**)BUILD_MEM_ALLOC(sizeof(*list->data) * 2);
      if (!list->data) {
         /* TODO: logging */
         return 0;
      }
      list->capacity = new_capacity;
   } else if ((list->count + 1) > (list->capacity))  {
      int new_capacity = list->capacity * 2;
      char** tmp = (char**)BUILD_MEM_REALLOC(list->data, sizeof(*list->data) * new_capacity);
      if (!tmp) {
         /* TODO: logging */
         return 0;
      }
      list->capacity = new_capacity;
      list->data = tmp;
   }

   item_clone = build_clone_bytes(item);
   if (!item_clone) {
      /* TODO: logging */
      return 0;
   }

   list->data[list->count++] = item_clone;
   return 1;
}

BUILD_DEF void build_free_string_list(Build_StringList* list)
{
   int i = 0;

   BUILD_ASSERT(list);

   for (i = 0; i < list->capacity; i++) {
      BUILD_MEM_FREE(list->data[i]);
      list->data[i] = NULL;
   }
   BUILD_MEM_FREE(list->data);
   list->data = NULL;
   list->count = 0;
   list->capacity = 0;
}

/* TODO: log errors */
BUILD_DEF int build_new_directory(const char* path)
{
   int result = 1;
   int i = 0;
   char* path_cstr = build_clone_bytes(path);
   int path_len = build_count_bytes(path);
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
         result = CreateDirectoryA(path_cstr, NULL);
#else
         result = mkdir(path_cstr, 0755);
         result = !result;
#endif
      }

      if (!result) {
         /* TODO: ignore "directory already exist" errors, log the rest */
      }
   }

   BUILD_MEM_FREE(path_cstr);
   return result;
}

BUILD_DEF int build_delete_directory(const char* path)
{
   int result = 0;

   /* TODO implement recursively deleting directory contents */

   return result;
}

BUILD_DEF int build_count_bytes(const char* s)
{
   int len = 0;
   while (*s++) {
      len++;
   }
   return len;
}

BUILD_DEF char* build_clone_bytes(const char* s)
{
   int i = 0;
   int len = build_count_bytes(s);
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
