
#ifndef __ERROR_CODE_H__
#define	__ERROR_CODE_H__

#include <stdint.h>

typedef uint64_t error_code_t;

#define SUCCESS 						 	((error_code_t)0)
#define ERROR_FAILED_TO_LOAD			 	((error_code_t)(1 << 0))
#define ERROR_FAILED_TO_RESIZE			 	((error_code_t)(1 << 1))
#define ERROR_INVALID_RESIZE_MODE			((error_code_t)(1 << 2)) 
#define ERROR_FAILED_TO_ALLOCATE_MEMORY	 	((error_code_t)(1 << 3))
#define ERROR_UNSUPPORTED_EXTENSION 	 	((error_code_t)(1 << 4))
#define ERROR_FAILED_TO_WRITE 			 	((error_code_t)(1 << 5))
#define ERROR_MEMORY_ALLOCATION_FAILURE	 	((error_code_t)(1 << 6)) 
#define ERROR_UNKOWN 					 	((error_code_t)(1 << 7))
#define ERROR_INVALID_ARGUMENTS 		 	((error_code_t)(1 << 8))
#define ERROR_INVALID_OPERATION_MODE 	 	((error_code_t)(1 << 9))
#define ERROR_FILE_NOT_FOUND 			 	((error_code_t)(1 << 10))
#define ERROR_INVALID_DIR 				 	((error_code_t)(1 << 11))
#define ERROR_FAILED_TO_CREATE_DIRECTORY 	((error_code_t)(1 << 12))
#define ERROR_NOT_ENOUGH_ARGUMENTS 		 	((error_code_t)(1 << 13))
#define ERROR_INVALID_OUTPUT_PATH  		 	((error_code_t)(1 << 14))
#define ERROR_INVALID_INPUT_PATH 			((error_code_t)(1 << 15))
#define ERROR_INVALID_INPUT_OR_OUTPUT_COUNT ((error_code_t)(1 << 16))
#define ERROR_NULL_REFERENCE 				((error_code_t)(1 << 17))

#define handle_error(error_code, args) __handle_error(error_code, args, __LINE__, __FUNCTION__, __FILE__)

void __handle_error(error_code_t error_code, void* args, uint64_t line_no, const char* function_name, const char* file_name);
#endif/*__ERROR_CODE_H__*/