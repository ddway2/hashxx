#pragma once

#include <string>

#include <hashxx/config.h>

#ifdef UNIX
#define like(x)		__builtin_expect(!!(x), 1)
#define unlike(x)	__builtin_expect(!!(x), 0)
#else
#define like(x)		(x)
#define unlike(x)	(x)
#endif

namespace hashxx {

HASHXX_API std::string
demangle_type_name(const std::string& mangled);

// Forward Declaration
class info_container;

}	// namespace hashxx
