
#include <directory.h>

#include <memory.h>
#include <dirent.h>
#include <direct.h>
#include <cwalk.h>
#include <debug.h>

error_code_t dir_make_good(char* dir, char* out_good_dir)
{
	if(out_good_dir == NULL)
		return ERROR_UNKOWN;
	size_t len = strlen(dir);
	char buffer[len + 1];
	memcpy(buffer, dir, len);
	buffer[len] = 0;
	size_t final_len = 0;
	char previous_char = buffer[0];
	for(size_t i = 0; i < len; i++)
	{
		if(buffer[i] == '\\')
			buffer[i] = '/';
		if((buffer[i] == '/') && (buffer[i] == previous_char))
			continue;
		out_good_dir[final_len] = buffer[i];
		final_len++;
		previous_char = buffer[i];
	}
	if(out_good_dir[final_len - 1] != '/')
		out_good_dir[final_len] = '/'; 
	else out_good_dir[final_len] = 0;
	out_good_dir[final_len + 1] = 0;
	return SUCCESS;
}

static error_code_t __dir_create_v1(char* dir)
{
	if(_mkdir(dir) != 0)
		return ERROR_FAILED_TO_CREATE_DIRECTORY;
	return SUCCESS;	
}

/*Don't try to understand it*/
static error_code_t __dir_create_v2(char* dir)
{
	char* _dir = dir;
	bool exists = false;
	while(!dir_exists(dir, &exists) && !exists)
	{ 
		char* end = strchr(_dir, '/');
		if((end == NULL) || (*end == 0))
			return SUCCESS;
		end -= 1;
		char* start = dir;
		uint64_t len = (uint64_t)end - (uint64_t)start + 1;
		char buffer[len + 1]; 
		memcpy(buffer, start, len);
		buffer[len] = 0;
		_dir = end + 2;
		if(!dir_exists(buffer, &exists) && !exists)
		{
			error_code_t error_code = __dir_create_v1(buffer); 
			if(error_code)
				return error_code;
		}
	}
	return SUCCESS;
}

error_code_t dir_create(char* dir)
{
	return __dir_create_v2(dir);
}

error_code_t dir_exists(char* dir, bool* out_is_exists)
{
	DIR* _dir = opendir(dir);
	if(_dir != NULL)
	{
		closedir(_dir);
		*out_is_exists = true;
		return SUCCESS;
	}
	else
	{
		closedir(_dir);
		*out_is_exists = false; 
		return SUCCESS;
	}
}

error_code_t foreach_file_in_dir(char* dir, char* search_pattern, void (*func)(struct _finddata_t* find_data, void* args), void* args)
{
	char _dir[strlen(dir) + 2]; 
	dir_make_good(dir, _dir);
	char __search_pattern[strlen(_dir) + strlen(search_pattern) + 1];
	strcpy(__search_pattern, _dir); 
	strcat(__search_pattern, search_pattern);

	struct _finddata_t file_data; 
	int file_handle; 
	int saved_handle;
	file_handle = _findfirst(__search_pattern, &file_data); 
	saved_handle = file_handle;
	if(file_handle == -1L)
		return ERROR_FILE_NOT_FOUND;
	do
	{
		func(&file_data, args);
	} while(!_findnext(file_handle, &file_data));
	_findclose(saved_handle);
	return SUCCESS;
}