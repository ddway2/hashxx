#pragma once

#include <unordered_map>

namespace hashxx {
	
template<typename T>
using index_type_impl = std::unordered_map<T, size_t>;

struct index_type_base {};

}	// namespace hashxx
