#include <cmdinput.h>

#include <exception.h>
#include <cmd_metadata.h>
#include <cwalk.h>
#include <image.h>
#include <directory.h>
#include <debug.h>

extern const char* app_version_and_name;

/*Does nothing, can be removed*/
error_code_t input_ensure(input_t* input)
{
	return SUCCESS;
}

error_code_t input_init(input_t* input)
{
	input->input = NULL; 
	input->output = NULL; 
	input->input_count = 0; 
	input->output_count = 0;
	input->resize_mode = 0; 
	input->operation_mode = 0;
	return SUCCESS;
}

error_code_t input_free(input_t* input)
{
	if(input == NULL)
		return ERROR_NULL_REFERENCE;
	switch(input->operation_mode)
	{
		case OPERATION_MODE_DIR_RECURSIVE:
		case OPERATION_MODE_SINGLE_FILE:
		case OPERATION_MODE_DIR:
			if((input->input_count != 1) && (input->output_count != 1))
				return ERROR_INVALID_INPUT_OR_OUTPUT_COUNT;
			if(input->input != NULL)
				free(input->input); 
			if(input->output != NULL)
				free(input->output);
			break;
		case OPERATION_MODE_FILES: 
			if(input->input != NULL)
			for(size_t i = 0; i < input->input_count; i++)
				if(((char**)input->input)[i] != NULL)
					free(((char**)input->input)[i]);
			if(input->input != NULL)
				free(input->input);
			if(input->output != NULL)
				free(input->output);
			break;
	}
	memset(input, 0, sizeof(input_t));
	return SUCCESS;
}

static error_code_t build_input_data(resize_mode_t resize_mode, operation_mode_t operation_mode, char** tokens, uint64_t token_count, input_t* out_input);
error_code_t input_parse(int argc, char** argv, input_t* out_input_data)
{

	resize_mode_t resize_mode = RESIZE_MODE_HALF;
	operation_mode_t operation_mode = OPERATION_MODE_SINGLE_FILE;; 

	CmdArgerDesc optional_argument_descriptions[] = 
	{
		cmd_arger_desc_enum(&resize_mode, "rmode", "selects the resize mode", resize_mode_enum_desc, sizeof(resize_mode_enum_desc) / sizeof(*resize_mode_enum_desc)),
		cmd_arger_desc_enum(&operation_mode, "omode", "selects the operation mode", operation_mode_enum_desc, sizeof(operation_mode_enum_desc) / sizeof(*operation_mode_enum_desc)),
	};

	char* cursor = NULL;
	CmdArgerDesc required_argument_descriptions[] = 
	{
		cmd_arger_desc_string(&cursor, "inputs and output", "inputs and output")
	};

	uint64_t optional_argument_count = sizeof(optional_argument_descriptions) / sizeof(*optional_argument_descriptions);
	uint64_t required_argument_count = sizeof(required_argument_descriptions) / sizeof(*required_argument_descriptions);
	cmd_arger_parse(optional_argument_descriptions, optional_argument_count, required_argument_descriptions, required_argument_count, argc, argv, (char*)app_version_and_name, cmd_arger_false);

	char** tokens = NULL;
	uint64_t token_count = argc;
	for(uint64_t i = 1; i < argc; i++)
	{
		if(argv[i] == cursor)
		{
			tokens = &argv[i]; 
			token_count -= i;
			break;
		}
	}
	error_code_t error_code = build_input_data(resize_mode, operation_mode, tokens, token_count, out_input_data);
	handle_error(error_code, NULL);
	return SUCCESS;
}

// typedef struct callback_add_file_to_input_data_t { input_t* input; size_t index;  } callback_add_file_to_input_data_t;
// static void callback_add_file_to_input(struct _finddata_t* finddata, void* args);
// static void callback_count_number_of_files(struct _finddata_t* finddata, void* args);

static error_code_t build_input_data(resize_mode_t resize_mode, operation_mode_t operation_mode, char** tokens, uint64_t token_count, input_t* out_input)
{
	if(token_count <= 0)
		return ERROR_NOT_ENOUGH_ARGUMENTS;

	out_input->input = NULL; 
	out_input->output = NULL; 
	out_input->input_count = 0; 
	out_input->output_count = 0; 
	out_input->resize_mode = resize_mode;
	out_input->operation_mode = operation_mode;

	cwk_path_set_style(CWK_STYLE_UNIX);
	switch(operation_mode)
	{
		case OPERATION_MODE_SINGLE_FILE: 
		{
			DEBUG_PUTS("OPERATION_MODE_SINGLE_FILE");
			size_t len = strlen(tokens[0]);
			out_input->input = malloc(sizeof(char) * (len + 1));
			out_input->input_count = 1;
			cwk_path_normalize(tokens[0], out_input->input, len + 1);
			DEBUG_PRINT("Input file path found: %s ", (const char*)out_input->input);
			const char* extension;
			size_t extension_len;
			bool has_extension = cwk_path_get_extension((char*)out_input->input, (const char**)&extension, &extension_len);
			if(!has_extension || !is_image_extension(extension))
				return ERROR_INVALID_INPUT_PATH;
			DEBUG_PRINT("Extension Found: %s", (const char*)extension);
			if(token_count  == 1)
			{
				size_t len2 = strlen(".resized") + strlen(out_input->input);
				char buffer[len2 + 1];
				strcpy(buffer, out_input->input);

				char new_extension[strlen(".resized") + strlen(extension) + 1];
				strcpy(new_extension, ".resized"); 
				strcat(new_extension, extension); 

				out_input->output = malloc(sizeof(char) * (len2 + 1));
				cwk_path_change_extension(buffer, new_extension, out_input->output, len2 + 1);
				DEBUG_PRINT("Output file path not found; using default file path %s ", (const char*)out_input->output);
			}
			else
			{
				has_extension = cwk_path_get_extension(tokens[1], (const char**)&extension, &extension_len); 
				if(!has_extension)
					return ERROR_INVALID_OUTPUT_PATH;
				out_input->output = malloc(sizeof(char) + (strlen(tokens[1]) + 1));
				cwk_path_normalize(tokens[1], out_input->output, strlen(tokens[1]) + 1);
				DEBUG_PRINT("Output file path found: %s ", (const char*)out_input->output);
			}
			out_input->output_count = 1;
			break;
		}
		case OPERATION_MODE_FILES:
		{
			DEBUG_PUTS("OPERATION_MODE_FILES");
			if(token_count < 2)
				return ERROR_NOT_ENOUGH_ARGUMENTS;
			out_input->input_count = (token_count - 1) << 1;
			out_input->input = malloc(sizeof(char*) * (out_input->input_count));
			DEBUG_PUTS("Files Found: ");
			for(size_t i = 0, j = 0; i < (token_count - 1); i++, j += 2)
			{
				bool has_extension;
				size_t extension_len;
				const char* extension; 
				has_extension = cwk_path_get_extension(tokens[i], (const char**)&extension, &extension_len);
			 	if(!has_extension || !is_image_extension(extension))
					return ERROR_INVALID_INPUT_PATH;
				((char**)out_input->input)[j] = (char*)malloc(sizeof(char) * (strlen(tokens[i]) + 1));
				cwk_path_normalize(tokens[i], ((char**)out_input->input)[j], strlen(tokens[i]) + 1);
				char* file_name = ((char**)out_input->input)[j];
				char buffer[strlen(file_name) + 1]; 
				strcpy(buffer, file_name);
				size_t len = strlen(buffer); 
				for(size_t k = 0; k < len; k++)
					if(buffer[k] == '\\')
						buffer[k] = '/';

				char* out_file_name_only = strrchr(buffer, '/'); 
				if((out_file_name_only == NULL) || (*out_file_name_only == 0))
					out_file_name_only = file_name;

				((char**)out_input->input)[j + 1] = (char*)malloc(sizeof(char) * (strlen(out_file_name_only) + 1));
				strcpy(((char**)out_input->input)[j + 1], out_file_name_only);
				DEBUG_PRINT("File no %lu, Input: %s, Output: %s ", i + 1, file_name, out_file_name_only);
			}
			char buffer[strlen(tokens[token_count - 1]) + 1];
			out_input->output_count = 1;
			cwk_path_normalize(tokens[token_count - 1], buffer, strlen(tokens[token_count - 1]) + 1);
			out_input->output = malloc(sizeof(char) * (strlen(buffer) + 2));
			dir_make_good(buffer, out_input->output);
			break;
		}
		case OPERATION_MODE_DIR_RECURSIVE:
		case OPERATION_MODE_DIR:
		{
			/*----------------   DON'T UNCOMMENT THIS!   ----------------------------*/
			// char* input_dir = tokens[0];
			// bool found_image_files = false;
			// size_t total_file_count = 0;
			// callback_add_file_to_input_data_t data =  { out_input, 1 };
			// for(size_t i = 0; i < NUM_IMAGE_EXTENSIONS; i++)
			// {
			// 	size_t file_count = 0; 
			// 	char search_pattern[strlen(image_extensions[i]) + 2]; 
			// 	strcpy(search_pattern, "*"); 
			// 	strcat(search_pattern, image_extensions[i]);
			// 	error_code_t _error_code = foreach_file_in_dir(input_dir, search_pattern, callback_count_number_of_files, (void*)(&file_count));
			// 	if(!_error_code)
			// 		found_image_files = true;
			// 	if(!_error_code)
			// 		total_file_count += file_count;
			// }
			// if(!found_image_files)
			// 	return ERROR_FILE_NOT_FOUND;
			// out_input->input = malloc(sizeof(char*) * (total_file_count + 1));
			// out_input->input_count = total_file_count + 1;
			// ((char**)out_input->input)[0] = (char*)malloc(strlen(input_dir) + 1);
			// cwk_path_normalize(input_dir, ((char**)out_input->input)[0], strlen(input_dir) + 1); 
			// for(size_t i = 0; i < NUM_IMAGE_EXTENSIONS; i++)
			// {
			// 	char search_pattern[strlen(image_extensions[i]) + 2]; 
			// 	strcpy(search_pattern, "*"); 
			// 	strcat(search_pattern, image_extensions[i]);
			// 	foreach_file_in_dir(input_dir, search_pattern, callback_add_file_to_input, (void*)(&data));
			// }
			DEBUG_PUTS((out_input->operation_mode == OPERATION_MODE_DIR) ? "OPERATION_MODE_DIR" : "OPERATION_MODE_DIR_RECURSIVE");
			out_input->input_count = 1; 
			out_input->input = malloc(sizeof(char) * (strlen(tokens[0]) + 2)); 
			char buffer[strlen(tokens[0]) + 1];
			cwk_path_normalize(tokens[0], buffer, strlen(tokens[0]) + 1);
			dir_make_good(buffer, out_input->input);
			DEBUG_PRINT("Input directory found: %s ", (const char*)out_input->input);
			if(token_count == 2)
			{
				char __buffer[strlen(tokens[1]) + 1];
				cwk_path_normalize(tokens[1], __buffer, strlen(tokens[1]) + 1);
				out_input->output = malloc(sizeof(char) * (strlen(__buffer) + 2));
				dir_make_good(__buffer, out_input->output);
				DEBUG_PRINT("Output directory found: %s ", (const char*)out_input->output);
			}
			else
			{
				out_input->output = malloc(sizeof(char) * (strlen((char*)out_input->input) + strlen("_Resized")));
				memcpy(out_input->output, (char*)out_input->input, strlen((char*)out_input->input) - 1); 
				((char*)out_input->output)[strlen((char*)out_input->input) - 1] = 0;
				strcat(out_input->output, "_Resized");
				DEBUG_PRINT("Output directory not found; using default directory: %s ", (const char*)out_input->output);
			}
			out_input->output_count = 1;
			break;
		}	 
		default: 
			return ERROR_INVALID_OPERATION_MODE;
	}
	DEBUG_PUTS("Arguments parsed successfully");
	return SUCCESS;
}

// static void callback_add_file_to_input(struct _finddata_t* finddata, void* args)
// {
// 	callback_add_file_to_input_data_t* data = (callback_add_file_to_input_data_t*)args; 
// 	input_t* input = data->input;
// 	const char* file_name = finddata->name;
// 	size_t len = strlen(file_name);
// 	((char**)input->input)[data->index] = (char*)malloc(sizeof(char) * (len + 1)); 
// 	strcpy(((char**)input->input)[data->index], file_name); 
// 	(data->index) += 1;
// }

// static void callback_count_number_of_files(struct _finddata_t* finddata, void* args)
// {
// 	(*(size_t*)args) += 1;
// }