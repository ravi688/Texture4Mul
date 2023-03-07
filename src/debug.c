

#include <debug.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <buffer.h>

#if defined(DEBUG)
static BUFFER* __debug_buffer = NULL;

#define BEGIN_TRACE()\
	uint64_t trace_len = strlen(function_name) + strlen(file_name) + 20;\
	char trace_buffer[trace_len];\
	sprintf(trace_buffer, ", at %lu, %s, %s\n", line_no, function_name, file_name);\
	trace_len = strlen(trace_buffer);

#define END_TRACE()\
	for(uint64_t i = 0; i < trace_len; i++)\
		BUFpush(&trace_buffer[i]);

__attribute__((constructor)) static void init_buffer_on_construct() { __debug_init__(); }
__attribute__((destructor)) static void flush_buffer_on_destruct() {  __debug_flush__(); puts("\nDebug Buffer Details: ");  __debug_terminate__();  }

void __debug_exit(size_t line_no, const char* function_name, const char* file_name, int exit_code)
{
	__debug_flush__();
	__debug_terminate__();
	exit(0);
}

void __debug_puts(size_t line_no, const char* function_name, const char* file_name, char* string)
{
	BUFpush_binded(); 
	BUFbind(__debug_buffer); 
	if(BUFget_element_count() > 0)
		BUFpop(NULL); 
	BEGIN_TRACE();
	uint64_t length = strlen(string);
	for(uint64_t i = 0; i < length; i++)
		BUFpush(&string[i]); 
	END_TRACE();
	char null_char = 0; 
	BUFpush(&null_char);
	BUFpop_binded();
#ifdef DEBUG_UNBUFFERED
	__debug_flush__();
#endif
}

void __debug_print(size_t line_no, const char* function_name, const char* file_name, const char* format_string, ...)
{
	BUFpush_binded();
	BUFbind(__debug_buffer);
	if(BUFget_element_count() > 0)
		BUFpop(NULL);
	BEGIN_TRACE();
	uint64_t length = strlen(format_string) + 180;
	char str_buffer[length];
	va_list args;
	va_start(args, format_string);
	vsnprintf(str_buffer, length, format_string, args);
	va_end(args);
	length = strlen(str_buffer);
	for(uint64_t i = 0; i < length; i++)
		BUFpush(&str_buffer[i]);
	END_TRACE();
	char null_char = 0;
	BUFpush(&null_char);
	BUFpop_binded();
#ifdef DEBUG_UNBUFFERED
	__debug_flush__();
#endif
}

void __debug_init(size_t line_no, const char* function_name, const char* file_name)
{
	__debug_buffer = BUFcreate(NULL, sizeof(char), 0, 0);
}

void __debug_flush(size_t line_no, const char* function_name, const char* file_name)
{
	BUFpush_binded();
	BUFbind(__debug_buffer);
	if(BUFget_element_count() == 0)
	{
		BUFpop_binded(); 
		return;
	}
	char* debug_str = BUFget_ptr();
	printf("%s", debug_str);
	BUFclear(NULL);
	BUFpop_binded();
}

void __debug_terminate(size_t line_no, const char* function_name, const char* file_name)
{
	BUFpush_binded();
	BUFbind(__debug_buffer);
	BUFlog();
	BUFfree(); 
	BUFpop_binded();
}
#endif/*DEBUG mode*/