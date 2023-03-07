

#include <exception.h>
#include <stdio.h>
#include <stdlib.h>

define_exception(METHOD_NOT_IMPLEMENTED);
define_exception(ACCESS_TO_RESTRICTED_CODE);

void __throw(exception_t exception, uint64_t line_no, const char* method_name, const char* file_name)
{
	printf("Exception: %s, thrown at %lu, %s, %s\n", exception, line_no, method_name, file_name);
	exit(0);
}