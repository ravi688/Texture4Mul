

#ifndef __CMD_INPUT_H__
#define __CMD_INPUT_H__

#include <error_code.h>
#include <texture_resize.h>

typedef struct input_t
{
	/* input: Stores: 
	 * 1. char**, pointer to the name of files in the current directory if operation_mode = OPERATION_MODE_CURR_DIR_FILES
	 * 2. char*,  pointer to the input dir if operation_mode = OPERATION_MODE_DIR
	 * 3. char*,  pointer to the input dir if operation_mode = OPERATION_MODE_DIR_RECURSIVE
	 * 4. char*,  pointer to the path of a single file if operation_mode = OPERATION_MODE_SINGLE_FILE
	 */
	void* input;
	/* output: Stores: 
	 * 1. char*,  pointer to the output dir in the current directory if operation_mode = OPERATION_MODE_CURR_DIR_FILES
	 * 2. char*,  pointer to the output dir if operation_mode = OPERATION_MODE_DIR
	 * 3. char*,  pointer to the output dir if operation_mode = OPERATION_MODE_RECURSIVE
	 * 4. char*,  pointer to the outpout path of a single file if operation_mode = OPERATION_MODE_SINGLE_FILE
	 */
	void* output;

	size_t input_count;
	size_t output_count;
	resize_mode_t resize_mode;
	operation_mode_t operation_mode;
} input_t;

error_code_t input_init(input_t* input);
error_code_t input_free(input_t* input);
error_code_t input_ensure(input_t* input);
error_code_t input_parse(int argc, char** argv, input_t* input);

#endif/*__CMD_INPUT_H__*/