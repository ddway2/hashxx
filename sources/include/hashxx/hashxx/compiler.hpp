#pragma once

#ifdef UNIX
#define like(x)		__builtin_expect(!!(x), 1)
#define unlike(x)	__buildin_expect(!!(x), 0)
#else
#define like(x)		x
#define unlike(x)	x
#endif
