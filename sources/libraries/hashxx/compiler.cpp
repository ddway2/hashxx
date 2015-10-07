#include <hashxx/compiler.hpp>

#include <stdexcept>

#ifdef UNIX
#include <unistd.h>
#include <cstdio>
#include <cxxabi.h>
#include <glob.h>
#endif

namespace hashxx {

std::string
demangle_type_name(const std::string& mangled)
{
#if defined(UNIX)
    char* buffer;
    int status;

    buffer = abi::__cxa_demangle(mangled.c_str(), 0, 0, &status);

    if (status == 0) {
        std::string n(buffer);
        free(buffer);

        return n;
    } else {
        return std::string("demangle failure");
    }
#elif defined(WINDOWS)
    return mangled;
#else
    return std::string("unsupported");
#endif
}

}	// namespace hashxx