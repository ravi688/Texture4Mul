
#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__

#include <stdbool.h>
#include <io.h>
#include <error_code.h>

/* 1. dir, the directory to be created.
 * 2. returns SUCCESS on success, otherwise ERROR_FAILED_TO_CREATE_DIRECTORY if that already exists or the dir is wrong.
 */
error_code_t dir_create(char* dir);


/* 1. dir, the directory to be checked for existance.
 * 2. returns SUCCESS always.
 */
error_code_t dir_exists(char* dir, bool* out_is_exists);


/* 1. out_good_dir must have enough size to hold 1 extra value than that in dir.
 * 2. converts every '\\' occurance with '/' character.
 * 3. trims the '/' or '\\' character if they are at the beginning of the dir. 
 * 4. replaces multiple and consecutive occurances of '/' or '\\' with a single '/' character.
 * 5. appends one extra character '/' at the end of the dir if it doesn't have.
 * 6. returns SUCCESS on success, otherwise ERROR_UNKOWN if dir == NULL.
 * Examples: 
 * 			1. ./Output////images/image --------> Output/images/image/
 *			2. .\\\\\Output\\\images////image --> Output/imges/image/
 */
error_code_t dir_make_good(char* dir, char* out_good_dir);


/* 1. args, can be NULL.
 * 2. dir, directory to search for files following the search_pattern.
 * 3. func, callback function.
 * 4. args, argument to be passed into the callback function.
 * 5. returns SUCCESS on success, otherwise ERROR_FILE_NOT_FOUND if there are no files matching that search pattern.
 * Examples: 
 * 			1. dir must be in the form of Output/images/ or ./Output/images/, but not Output/images or ./Output/images
 * 			2. serach_pattern should be like *.png or *.bmp
 */
error_code_t foreach_file_in_dir(char* dir, char* search_pattern, void (*func)(struct _finddata_t* find_data, void* args), void* args);

#endif/*__DIRECTORY_H__*/