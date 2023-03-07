
#include <image.h>

#include <stb/stb_image.h>
#include <stb/stb_image_resize.h>
#include <stb/stb_image_write.h>

#include <string.h>
#include <cwalk.h>


const char* image_extensions[] =  { ".png", ".jpg", ".bmp" };
const uint32_t NUM_IMAGE_EXTENSIONS = 3;

bool is_image_extension(const char* extension)
{
	for(uint32_t i = 0; i < NUM_IMAGE_EXTENSIONS; i++)
		if(!strcmp(extension, image_extensions[i]))
			return true;
	return false;
}

error_code_t image_get_width(image_t* image, image_size_t* out_width)
{
	*out_width = image->width;
	return SUCCESS;
}
error_code_t image_get_height(image_t* image, image_size_t* out_height)
{
	*out_height = image->height;
	return SUCCESS;
}
error_code_t image_get_data(image_t* image, uint8_t** out_bytes)
{
	*out_bytes = image->data;
	return SUCCESS;
}
error_code_t image_get_num_channels(image_t* image, image_size_t* out_num_channels)
{
	*out_num_channels = image->channels;
	return SUCCESS;
}
error_code_t image_get_stride(image_t* image, image_size_t* out_stride)
{
	*out_stride = image->stride;
	return SUCCESS;
}
error_code_t image_get_type(image_t* image, image_type_t* out_image_type)
{
	*out_image_type = image->type;
	return SUCCESS;
}

error_code_t image_init(image_t* image)
{
	image->data = NULL;
	image->width = 0;
	image->height = 0;
	image->channels = 0;
	image->stride = 0;
	image->type = 0;
	return SUCCESS;
}

error_code_t image_save(image_t* image, char* file_name)
{
	switch(image->type)
	{
		case IMAGE_TYPE_PNG:
			return stbi_write_png(file_name, image->width, image->height, image->channels, image->data, 0) ? SUCCESS : ERROR_FAILED_TO_WRITE;
		case IMAGE_TYPE_JPG:
			return stbi_write_jpg(file_name, image->width, image->height, image->channels, image->data, 100) ? SUCCESS : ERROR_FAILED_TO_WRITE;
		default: 
			return ERROR_FAILED_TO_WRITE | ERROR_UNSUPPORTED_EXTENSION;
	}
}

error_code_t image_log(image_t* image)
{
	printf(	"------------------------\n"
			"Width: %lu\n"
			"Height: %lu\n"
			"Channels: %lu\n"
			"Stride: %lu\n"
			"Type: %s\n"
			"------------------------\n",
			image->width,
			image->height,
			image->channels,
			image->stride,
			(image->type > NUM_IMAGE_EXTENSIONS) ? "unkown" : image_extensions[image->type - 1]
		);
	return SUCCESS;
}

error_code_t image_load(image_t* image, char* file_name)
{
	const char* extension = strrchr(file_name, '.'); 
	if(extension != NULL)
	{
		for(image_type_t i = 1; i <= NUM_IMAGE_EXTENSIONS; i++)
			if(!strcmp(extension, image_extensions[i - 1]))
		 		image->type = i; 
	}
	else return ERROR_UNSUPPORTED_EXTENSION;
	image->data = stbi_load(file_name, &(image->width), &(image->height), &(image->channels), 0);
	if(image->data == NULL)
		return ERROR_FAILED_TO_LOAD;
	image->stride = 0;	
	return SUCCESS;
}

error_code_t image_create_resized(image_t* image, image_size_t width, image_size_t height, image_t* out_resized_image)
{
	error_code_t error_code = image_create(out_resized_image, width, height, image->channels, image->type);
	if(error_code)
		return error_code;
	int result = stbir_resize_uint8(image->data, image->width, image->height, 0, out_resized_image->data, width, height, 0, image->channels);
	if(!result)
		return ERROR_FAILED_TO_RESIZE;
	return SUCCESS;
}

error_code_t image_create(image_t* image, image_size_t width, image_size_t height, image_size_t channels, image_type_t image_type)
{
	image->data = (uint8_t*)malloc(sizeof(uint8_t) * width * height * channels); 
	if(image->data == NULL)
		return ERROR_MEMORY_ALLOCATION_FAILURE;
	image->width = width; 
	image->height = height; 
	image->channels = channels;
	image->stride = 0;
	image->type = image_type;
	return SUCCESS;
}

error_code_t image_free(image_t* image)
{
	if(image->data != NULL)
		free(image->data);
	return SUCCESS;;
}