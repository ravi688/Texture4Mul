

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <stdint.h>

#define THROW(exception) __throw(__exception_name(exception), __LINE__, __FUNCTION__ , __FILE__)


typedef char* exception_t;
#define __exception_name(exception_name) EXCEPTION_##exception_name
#define define_exception(exception_name) const exception_t __exception_name(exception_name) = #exception_name
#define declare_exception(exception_name) extern const exception_t __exception_name(exception_name)


declare_exception(METHOD_NOT_IMPLEMENTED);
declare_exception(ACCESS_TO_RESTRICTED_CODE);

void __throw(exception_t exception, uint64_t line_no, const char* method_name, const char* file_name);


#endif/*__EXCEPTION_H__*/