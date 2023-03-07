
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <image.h>
#include <texture_resize.h>
#include <error_code.h>
#include <cmdinput.h>
#include <debug.h>

const char* app_version_and_name = "Texture-Resize-1.0";
extern size_t total_file_processed_count;

static error_code_t input_process(input_t* input)
{
	switch(input->operation_mode)
	{
		case OPERATION_MODE_SINGLE_FILE: 
		{
			error_code_t error_code = process_file((char*)input->input, "./", (char*)input->output, input->resize_mode);
			return error_code;
		}
		case OPERATION_MODE_FILES: 
		{
			size_t file_count = input->input_count >> 1; 
			error_code_t error_code = 0;
			for(size_t i = 0, j = 0; i < file_count; i++, j += 2)
				error_code |= process_file(((char**)input->input)[j], (char*)input->output, ((char**)input->input)[j + 1], input->resize_mode);
			return error_code;
		}
		case OPERATION_MODE_DIR: 
		{
			error_code_t error_code = process_files_in_dir((char*)input->input, (char*)input->output, input->resize_mode);
			return error_code;
		}
		case OPERATION_MODE_DIR_RECURSIVE: 
		{
			error_code_t error_code = processs_files_in_dir_recursive((char*)input->input, (char*)input->output, input->resize_mode);
			return error_code;
		}
		default:
			return ERROR_INVALID_OPERATION_MODE;
	}
	return SUCCESS;
}

static void display_small_help()
{
	const char* help =
	"Oops! Not enough arguments are provided\n"
	"Use --help to get more information!";
	puts(help);
}

int main(int argc, char** argv)
{
	if(argc <= 1)
	{
		display_small_help();
		exit(0);
	}
	input_t input; 
	handle_error(input_init(&input), NULL);
	handle_error(input_parse(argc, argv, &input), NULL);
	handle_error(input_ensure(&input), NULL);
	handle_error(input_process(&input), NULL);
	handle_error(input_free(&input), NULL);
	DEBUG_PRINT("\nTotal number of files processed: %lu", total_file_processed_count);
	return 0;
}

