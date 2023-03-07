

#ifndef __CMD_METADATA_H__
#define	__CMD_METADATA_H__
#include <texture_resize.h>
#include <cmd_arger.h>

static CmdArgerEnumDesc operation_mode_enum_desc[] = 
{
	{
		.name = "dir", 
		.info = "input argument will be a directory containing image files",
		.value = OPERATION_MODE_DIR
	},
	{
		.name = "dirr", 
		.info = "input argument will be a directory containing image files and folders, operation will be recursive",
		.value = OPERATION_MODE_DIR_RECURSIVE
	},
	{
		.name = "files", 
		.info = "input arguments will be file names, operation will be performed only on those files",
		.value = OPERATION_MODE_FILES
	},
	{
		.name = "file", 
		.info = "input argment will be a single file name",
		.value = OPERATION_MODE_SINGLE_FILE
	}
};

static CmdArgerEnumDesc resize_mode_enum_desc[] = 
{
	{
		.name = "half",
		.info = "1/2th of the original image", 
		.value = RESIZE_MODE_HALF
	},
	{
		.name = "quad", 
		.info = "1/4th of the original image",
		.value = RESIZE_MODE_QUAD
	},
	{
		.name = "oct",
		.info = "1/8th of the original image",
		.value = RESIZE_MODE_OCT
	},
	{
		.name = "mul2",
		.info = "multiple of 2 of the original image",
		.value = RESIZE_MODE_2_MULTIPLE
	},
	{
		.name = "mul4",
		.info = "multiple of 4 of the original image",
		.value = RESIZE_MODE_4_MULTIPLE
	},
	{
		.name = "mul8",
		.info = "multiple of 8 of the original image",
		.value = RESIZE_MODE_8_MULTIPLE
	},
	{
		.name = "halfmul2",
		.info = "1/2th and multiple of 2 of the original image",
		.value = RESIZE_MODE_HALF | RESIZE_MODE_2_MULTIPLE
	},
	{
		.name = "halfmul4",
		.info = "1/2th and multiple of 4 of the original image",
		.value = RESIZE_MODE_HALF | RESIZE_MODE_4_MULTIPLE
	},
	{
		.name = "halfmul8",
		.info = "1/2th and multiple of 4 of the original image",
		.value = RESIZE_MODE_HALF | RESIZE_MODE_8_MULTIPLE
	},
	{
		.name = "quadmul2",
		.info = "1/4th and multiple of 2 of the original image",
		.value = RESIZE_MODE_QUAD | RESIZE_MODE_2_MULTIPLE
	},
	{
		.name = "quadmul4",
		.info = "1/4th and multiple of 4 of the original image",
		.value = RESIZE_MODE_QUAD | RESIZE_MODE_4_MULTIPLE
	},
	{
		.name = "quadmul8",
		.info = "1/4th and multiple of 8 of the original image",
		.value = RESIZE_MODE_QUAD | RESIZE_MODE_8_MULTIPLE
	},
	{
		.name = "octmul2",
		.info = "1/8th and multiple of 2 of the original image",
		.value = RESIZE_MODE_OCT | RESIZE_MODE_2_MULTIPLE
	},
	{
		.name = "octmul4",
		.info = "1/8th and multiple of 4 of the original image",
		.value = RESIZE_MODE_OCT | RESIZE_MODE_4_MULTIPLE
	},
	{
		.name = "octmul8",
		.info = "1/8th and multiple of 8 of the original image",
		.value = RESIZE_MODE_OCT | RESIZE_MODE_8_MULTIPLE
	}
};


#endif/*__CMD_METADATA_H__*/