#ifndef __TEXTURE_RESIZE_H__
#define __TEXTURE_RESIZE_H__


#include <stdint.h>

#include <image.h>
#include <error_code.h>

typedef uint32_t count_t;;
typedef uint64_t operation_mode_t;
typedef uint64_t resize_mode_t;

#define RESIZE_MODE_HALF 	 				((resize_mode_t)(1 << 0))
#define RESIZE_MODE_QUAD 	 				((resize_mode_t)(1 << 1))
#define RESIZE_MODE_OCT 					((resize_mode_t)(1 << 2))
#define RESIZE_MODE_2_MULTIPLE  			((resize_mode_t)(1 << 3))
#define RESIZE_MODE_4_MULTIPLE  			((resize_mode_t)(1 << 4))
#define RESIZE_MODE_8_MULTIPLE  			((resize_mode_t)(1 << 5))

#define OPERATION_MODE_DIR  				((operation_mode_t)(1 << 0)) 
#define OPERATION_MODE_DIR_RECURSIVE  		((operation_mode_t)(1 << 1))
#define OPERATION_MODE_FILES		 		((operation_mode_t)(1 << 2))
#define OPERATION_MODE_SINGLE_FILE 			((operation_mode_t)(1 << 3))

/*TODO: Implement this*/
error_code_t processs_files_in_dir_recursive(char* input_dir, char* output_dir, resize_mode_t resize_mode);

error_code_t get_size(image_size_t size, resize_mode_t resize_mode, image_size_t* new_size);
error_code_t process_files_in_dir(char* input_dir, char* output_dir, resize_mode_t resize_mode);
error_code_t process_file(char* file_name, char* output_dir, char* output_file_name, resize_mode_t resize_mode);

const char* resize_mode_to_string(resize_mode_t mode);
const char* operation_mode_to_string(operation_mode_t mode);

#endif/*__TEXTURE_RESIZE_H__*/