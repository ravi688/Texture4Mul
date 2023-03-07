#include <string.h>
#include <stdio.h>

#include <texture_resize.h>
#include <directory.h>
#include <exception.h>
#include <debug.h>

/*Some static declarations; Not be understood by the user*/
static void callback_process_dir_recursive(struct _finddata_t* find_data, void* args);
static bool is_file(struct _finddata_t* find_data);
static bool is_subdir(struct _finddata_t* find_data);
static void callback_process_file(struct _finddata_t* find_data, void* args);


/* Holdes the total number of files processed when any of the following functions are called:
 * 1. processs_files_in_dir_recursive
 * 2. process_files_in_dir
 * 3. process_file
 */
size_t total_file_processed_count = 0;

typedef struct _callback_data_t
{
	char* input_dir;
	char* output_dir;
	resize_mode_t resize_mode; 
} _callback_data_t;


/* Processes all the files in all the sub-directories of input_dir having image extensions and saves the same hierarchy into the output directory output_dir
 * input_dir: Input directory contains the images and sub-directories containing images
 * 			note: must be in the form of Input/ or ./Input/
 * output_dir: can be any output directory
 * 			note: must be in the form of Output/ or ./Output/output2/output1/
 * resize_mode: must be a valid resize mode
 * returns: any error_code value, use handle_error to get the error string.
 */
error_code_t processs_files_in_dir_recursive(char* input_dir, char* output_dir, resize_mode_t resize_mode)
{
	_callback_data_t data =  { input_dir, output_dir, resize_mode };
	error_code_t error_code = foreach_file_in_dir(input_dir, "*", callback_process_dir_recursive, (void*)(&data));	
	return error_code;
}

/* Processes each file having image extensions in input_dir and saves into output_dir
 * input_dir: Input Directory
 * 			  note: must be in the format Input/Images/images2/ or ./Input/Imges/images2/
 * output_dir: Output Directory
 * 			  note: must be in the format Output/Images/ or ./Output/Images
 * resize_mode: must be a valid resize mode.
 * returns: any error_code value, use handle_error to get the error string.
 */
error_code_t process_files_in_dir(char* input_dir, char* output_dir, resize_mode_t resize_mode)
{
	for(int i = 0; i < NUM_IMAGE_EXTENSIONS; i++)
	{
		//make search pattern for a particular extension -> for example .png will become *.png
		const char* file_extension = image_extensions[i];
		char search_pattern[strlen(file_extension) + 2]; 
		strcpy(search_pattern, "*"); 
		strcat(search_pattern, file_extension);
		//process each file -> load the image file, resize it and then save it to the output_dir
		_callback_data_t data =  { input_dir, output_dir, resize_mode };
		error_code_t error_code = foreach_file_in_dir(input_dir, search_pattern, callback_process_file, (void*)(&data));
		DEBUG_BLOCK
		(
			if(error_code & ERROR_FILE_NOT_FOUND)
				DEBUG_PRINT("Warning: There are no files with extension %s", image_extensions[i]);
		);
	}
	return SUCCESS;
}

/* Processes a single file and saves into the output_dir with output_file_name
 * file_name: input file path
 * output_dir: can be "./" but must be end with '/' like Output/
 * output_file_name: output file name
 * resize_mode: must be a valid resize mode.
 * returns: any error_code value, use handle_error to get the error string.
 */
error_code_t process_file(char* file_name, char* output_dir, char* output_file_name, resize_mode_t resize_mode)
{
	DEBUG_PRINT("Loading %s ", file_name);
	image_t image, resized_image;
	error_code_t error_code = image_init(&image); 
	error_code |= image_load(&image, file_name);
	if(error_code)
		return error_code;
	image_size_t width, height;
	error_code |= get_size(image.width, resize_mode, &width);
	error_code |= get_size(image.height, resize_mode, &height);
	if(error_code)
		return error_code;
	DEBUG_PRINT("Resizing %s from %lu X %lu to %lu X %lu ", file_name, image.width, image.height, width, height);
	error_code = image_create_resized(&image, width, height, &resized_image);
	if(error_code)
		return error_code;
	/*TODO: Move this block of code into input_ensure function*/
	/*begin-block*/
	bool is_dir_exists; 
	error_code = dir_exists(output_dir, &is_dir_exists);
	if(error_code)
		return error_code;
	if(!is_dir_exists)
	{
		DEBUG_PRINT("Directory %s doesn't exists; creating ", output_dir);
		error_code = dir_create(output_dir);
		if(error_code)
			return error_code;
	}
	/*end-block*/
	char final_path[strlen(output_dir) + strlen(output_file_name) + 1];
	strcpy(final_path, output_dir);
	strcat(final_path, output_file_name);
	DEBUG_PRINT("Saving resized %s to %s ", file_name, final_path);
	error_code =  image_save(&resized_image, final_path);
	image_free(&resized_image);
	image_free(&image);
	total_file_processed_count++;
	return error_code;	
}

/* filles the new_size with some appropriate modified value of size according to the resize_mode passed into this function.
 * size: size of the previous_image
 * resize_mode: resize mode
 * new_size: new resized value of previous_image based on the resize_mode.
 * returns: any error_code value, use handle_error to get the error string.
 */
error_code_t get_size(image_size_t size, resize_mode_t resize_mode, image_size_t* new_size)
{
	bool b1 = resize_mode & (RESIZE_MODE_HALF | RESIZE_MODE_OCT | RESIZE_MODE_QUAD);
	bool b2 = resize_mode & (RESIZE_MODE_2_MULTIPLE | RESIZE_MODE_4_MULTIPLE | RESIZE_MODE_8_MULTIPLE);
	if(!b1 && !b2)
		return ERROR_INVALID_RESIZE_MODE;

	if(resize_mode & RESIZE_MODE_HALF)
	 	size = size >> 1; 
	else if(resize_mode & RESIZE_MODE_QUAD)
		size = size >> 2; 
	else if(resize_mode & RESIZE_MODE_OCT)
		size = size >> 3; 

	if(resize_mode & RESIZE_MODE_2_MULTIPLE)
		size = (size >> 1) << 1; 
	else if(resize_mode & RESIZE_MODE_4_MULTIPLE)
		size = (size >> 2) << 2; 
	else if(resize_mode & RESIZE_MODE_8_MULTIPLE)
		size = (size >> 3) << 3;

	*new_size = size;
	return SUCCESS;
}

/*Obvious*/
const char* resize_mode_to_string(resize_mode_t mode)
{
	switch(mode)
	{
		case RESIZE_MODE_HALF : return "RESIZE_MODE_HALF";
		case RESIZE_MODE_QUAD : return "RESIZE_MODE_QUAD";
		case RESIZE_MODE_OCT : return "RESIZE_MODE_OCT";
		case RESIZE_MODE_2_MULTIPLE : return "RESIZE_MODE_2_MULTIPLE";
		case RESIZE_MODE_4_MULTIPLE : return "RESIZE_MODE_4_MULTIPLE";
		case RESIZE_MODE_8_MULTIPLE : return "RESIZE_MODE_8_MULTIPLE";

		case RESIZE_MODE_2_MULTIPLE | RESIZE_MODE_HALF : return "RESIZE_MODE_2_MULTIPLE | RESIZE_MODE_HALF";
		case RESIZE_MODE_4_MULTIPLE | RESIZE_MODE_HALF : return "RESIZE_MODE_4_MULTIPLE | RESIZE_MODE_HALF";
		case RESIZE_MODE_8_MULTIPLE | RESIZE_MODE_HALF : return "RESIZE_MODE_8_MULTIPLE | RESIZE_MODE_HALF";

		case RESIZE_MODE_2_MULTIPLE | RESIZE_MODE_QUAD : return "RESIZE_MODE_2_MULTIPLE | RESIZE_MODE_QUAD";
		case RESIZE_MODE_4_MULTIPLE | RESIZE_MODE_QUAD : return "RESIZE_MODE_4_MULTIPLE | RESIZE_MODE_QUAD";
		case RESIZE_MODE_8_MULTIPLE | RESIZE_MODE_QUAD : return "RESIZE_MODE_8_MULTIPLE | RESIZE_MODE_QUAD";

		case RESIZE_MODE_2_MULTIPLE | RESIZE_MODE_OCT : return "RESIZE_MODE_2_MULTIPLE | RESIZE_MODE_OCT";
		case RESIZE_MODE_4_MULTIPLE | RESIZE_MODE_OCT : return "RESIZE_MODE_4_MULTIPLE | RESIZE_MODE_OCT";
		case RESIZE_MODE_8_MULTIPLE | RESIZE_MODE_OCT : return "RESIZE_MODE_8_MULTIPLE | RESIZE_MODE_OCT";

		default: return "unkown";
	}
}

/*Obvious*/
const char* operation_mode_to_string(operation_mode_t mode)
{
	switch(mode)
	{
		case OPERATION_MODE_FILES: return "OPERATION_MODE_FILES";
		case OPERATION_MODE_SINGLE_FILE: return "OPERATION_MODE_SINGLE_FILE";
		case OPERATION_MODE_DIR : return "OPERATION_MODE_DIR";
		case OPERATION_MODE_DIR_RECURSIVE : return "OPERATION_MODE_DIR_RECURSIVE";
		default: return "unkown";
	}
}

static void callback_process_file(struct _finddata_t* find_data, void* args)
{
	_callback_data_t* data = (_callback_data_t*)args;
	char buffer[strlen(data->input_dir) + strlen(find_data->name) + 1];
	strcpy(buffer, data->input_dir); 
	strcat(buffer, find_data->name);
	error_code_t error_code = process_file(buffer, data->output_dir, find_data->name, data->resize_mode);
	if(error_code)
		handle_error(error_code, (void*)(find_data->name));		
}

/*returns true if the find_data contains attributes of a sub directory except . or ..*/
static bool is_subdir(struct _finddata_t* find_data)
{
	return (find_data->attrib & _A_SUBDIR) && strcmp(find_data->name, ".") && strcmp(find_data->name, "..");
}

/*returns true if the find_data contains attributes of a file (not a sub directory)*/
static bool is_file(struct _finddata_t* find_data)
{
	return !(find_data->attrib & _A_SUBDIR) && strcmp(find_data->name, ".") && strcmp(find_data->name, "..");
}

static void callback_process_dir_recursive(struct _finddata_t* find_data, void* args)
{
	_callback_data_t* data = (_callback_data_t*)args;
	if(is_subdir(find_data))
	{
		/*prepare the full path for the input dir*/
		char full_input_path[strlen(data->input_dir) + strlen(find_data->name) + 2];
		strcpy(full_input_path, data->input_dir); 
		strcat(full_input_path, find_data->name);
		{
			char buffer2[strlen(data->input_dir) + strlen(find_data->name) + 2]; 
			dir_make_good(full_input_path, buffer2); 
			strcpy(full_input_path, buffer2);
		}
		/*prepare the full path for the output dir*/
		char full_output_path[strlen(data->output_dir) + strlen(find_data->name) + 2]; 
		strcpy(full_output_path, data->output_dir); 
		strcat(full_output_path, find_data->name);
		{
			char buffer2[strlen(data->output_dir) + strlen(find_data->name) + 2]; 
			dir_make_good(full_output_path, buffer2); 
			strcpy(full_output_path, buffer2);
		}
		error_code_t error_code = processs_files_in_dir_recursive(full_input_path, full_output_path, data->resize_mode);
		if(error_code)
			handle_error(error_code, (void*)full_input_path);
	}
	else if(is_file(find_data))
		callback_process_file(find_data, args);
}