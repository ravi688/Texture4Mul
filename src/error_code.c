
#include <error_code.h>
#include <stdio.h>

#ifndef TO_STRING
#define TO_STRING(v) #v
#endif

#ifndef PRINT_ERROR
#define PRINT_ERROR(v) printf("[Error] %s, at %lu, %s, %s\n",v, line_no, function_name, file_name)
#endif

void __handle_error(error_code_t error_code, void* args, uint64_t line_no, const char* function_name, const char* file_name)
{		
	if(!error_code)
		return;
	if(error_code & ERROR_FAILED_TO_LOAD)
		PRINT_ERROR("ERROR_FAILED_TO_LOAD");							 	
	if(error_code & ERROR_FAILED_TO_RESIZE)
		PRINT_ERROR("ERROR_FAILED_TO_RESIZE");				
	if(error_code & ERROR_INVALID_RESIZE_MODE)
		PRINT_ERROR("ERROR_INVALID_RESIZE_MODE");				
	if(error_code & ERROR_FAILED_TO_ALLOCATE_MEMORY)
	 	PRINT_ERROR("ERROR_FAILED_TO_ALLOCATE_MEMORY");				
	if(error_code & ERROR_UNSUPPORTED_EXTENSION)
	 	PRINT_ERROR("ERROR_UNSUPPORTED_EXTENSION");	
	if(error_code & ERROR_FAILED_TO_WRITE)
	 	PRINT_ERROR("ERROR_FAILED_TO_WRITE");	
	if(error_code & ERROR_MEMORY_ALLOCATION_FAILURE)
	 	PRINT_ERROR("ERROR_MEMORY_ALLOCATION_FAILURE");					
	if(error_code & ERROR_UNKOWN)
	 	PRINT_ERROR("ERROR_UNKOWN");	
	if(error_code & ERROR_INVALID_ARGUMENTS)
	 	PRINT_ERROR("ERROR_INVALID_ARGUMENTS");	 	
	if(error_code & ERROR_INVALID_OPERATION_MODE)
	 	PRINT_ERROR("ERROR_INVALID_OPERATION_MODE");		
	if(error_code & ERROR_FILE_NOT_FOUND)
	 	PRINT_ERROR("ERROR_FILE_NOT_FOUND");	
	if(error_code & ERROR_INVALID_DIR)
	 	PRINT_ERROR("ERROR_INVALID_DIR");	
	if(error_code & ERROR_FAILED_TO_CREATE_DIRECTORY)
	 	PRINT_ERROR("ERROR_FAILED_TO_CREATE_DIRECTORY");	
	if(error_code & ERROR_NOT_ENOUGH_ARGUMENTS)
	 	PRINT_ERROR("ERROR_NOT_ENOUGH_ARGUMENTS");	
	if(error_code & ERROR_INVALID_OUTPUT_PATH)
	 	PRINT_ERROR("ERROR_INVALID_OUTPUT_PATH");	
	if(error_code & ERROR_INVALID_INPUT_PATH)
	 	PRINT_ERROR("ERROR_INVALID_INPUT_PATH");
	 if(error_code & ERROR_INVALID_INPUT_OR_OUTPUT_COUNT)
	 	PRINT_ERROR("ERROR_INVALID_INPUT_OR_OUTPUT_COUNT");
	 if(error_code & ERROR_NULL_REFERENCE)
	 	PRINT_ERROR("ERROR_NULL_REFERENCE");
}