

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stddef.h>

#if !defined(DEBUG) && !defined(RELEASE)
#warning "Warning: build mode is not defined; using [DEBUG] build mode"
#define DEBUG
#endif/*if DEBUG and RELEASE are not defined either, using DEBUG mode by default*/

#if defined(DEBUG) && defined(RELEASE)
#warning "Warning: both the build mode [DEBUG] and [RELEASE] are defined; using the [DEBUG] mode"
#undef RELEASE
#endif/*if DEBUG and RELEASE are both defined, using DEBUG mode by default*/

#if defined(DEBUG)

#define __debug_print__(frmt_str, ...) __debug_print(__LINE__, __FUNCTION__, __FILE__, frmt_str, __VA_ARGS__)
#define __debug_puts__(str) __debug_puts(__LINE__, __FUNCTION__, __FILE__, str)
#define __debug_init__() __debug_init(__LINE__, __FUNCTION__, __FILE__)
#define __debug_flush__() __debug_flush(__LINE__, __FUNCTION__, __FILE__)
#define __debug_terminate__() __debug_terminate(__LINE__, __FUNCTION__, __FILE__)
#define __debug_exit__(exit_code) __debug_exit(__LINE__, __FUNCTION__, __FILE__, exit_code)

#define ASSERT(boolean, string, ...) do { if(!(boolean)) { printf("Assertion Failed: %s, at %lu, %s, %s\n", string, __LINE__, __FUNCTION__, __FILE__); __debug_exit(0); } } while(false)
#define DEBUG_BLOCK(x) {  x  }
#define DEBUG_INT(var_name) __debug_print__("%s: %lld\n", #var_name, var_name)
#define DEBUG_CHAR(var_name) __debug_print__("%s: %c\n", #var_name, var_name)
#define DEBUG_UINT(var_name) __debug_print__("%s: %llu\n", #var_name, var_name)
#define DEBUG_FLOAT(var_name) __debug_print__("%s: %f\n", #var_name, var_name)
#define DEBUG_STR(var_name) __debug_print__("%s: %s\n", #var_name, var_name)
#define DEBUG_BOOL(var_name) __debug_print__("%s: %s\n", #var_name, (var_name) ? "true" : "false")
#define DEBUG_PRINT(...) __debug_print__(__VA_ARGS__)
#define DEBUG_PUTS(string) __debug_puts__(string)

#define DEBUG_INIT() __debug_init__()
#define DEBUG_FLUSH() __debug_flush__()
#define DEBUG_TERMINATE() __debug_terminate__()

void __debug_puts(size_t line_no, const char* function_name, const char* file_name, char* string);
void __debug_print(size_t line_no, const char* function_name, const char* file_name, const char* frmt_str, ...);
void __debug_init(size_t line_no, const char* function_name, const char* file_name);
void __debug_flush(size_t line_no, const char* function_name, const char* file_name);
void __debug_terminate(size_t line_no, const char* function_name, const char* file_name);
void __debug_exit(size_t line_no, const char* function_name, const char* file_name, int);

#elif defined(RELEASE)
#define ASSERT(boolean, string, ...)
#define DEBUG_BLOCK(x)
#define DEBUG_INT(var_name)
#define DEBUG_CHAR(var_name)
#define DEBUG_UINT(var_name)
#define DEBUG_FLOAT(var_name)
#define DEBUG_STR(var_name)
#define DEBUG_BOOL(var_name)
#define DEBUG_PRINT(...)
#define DEBUG_PUTS(string)

#define DEBUG_INIT()
#define DEBUG_FLUSH()
#define DEBUG_TERMINATE()

#endif/*BUILD MODE*/

#endif/*__DEBUG_H__*/