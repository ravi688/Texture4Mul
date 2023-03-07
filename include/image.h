
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdint.h>
#include <stdbool.h>
#include <error_code.h>


extern const char* image_extensions[];
extern const uint32_t NUM_IMAGE_EXTENSIONS;

typedef uint32_t image_size_t;
typedef uint32_t image_type_t;

typedef struct image_t 
{
	uint8_t* data; 
	image_size_t width; 
	image_size_t height; 
	image_size_t channels;
	image_size_t stride;
	image_type_t type;
} image_t;

#define IMAGE_TYPE_PNG ((image_type_t)1)
#define IMAGE_TYPE_JPG ((image_type_t)2)

bool is_image_extension(const char* extension);

error_code_t image_init(image_t* image);
error_code_t image_save(image_t* image, char* file_name);
error_code_t image_load(image_t* image, char* file_name);
error_code_t image_create_resized(image_t* image, image_size_t width, image_size_t height, image_t* out_resized_image);
error_code_t image_create(image_t* image, image_size_t width, image_size_t height, image_size_t channels, image_type_t image_type);
error_code_t image_free(image_t* image);
error_code_t image_log(image_t* image);

error_code_t image_get_width(image_t* image, image_size_t* out_width);
error_code_t image_get_width(image_t* image, image_size_t* out_height);
error_code_t image_get_data(image_t* image, uint8_t** out_bytes);
error_code_t image_get_num_channels(image_t* image, image_size_t* out_num_channels);
error_code_t image_get_stride(image_t* image, image_size_t* out_stride);
error_code_t image_get_type(image_t* image, image_type_t* out_image_type);

#endif/*__IMAGE_H__*/