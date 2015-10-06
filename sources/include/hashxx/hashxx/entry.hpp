#pragma once

#include <atomic>

namespace hashxx {

const size_t default_container_size = 100000;

template<typename T>
struct entry {
	using value_type = T;

	size_t				index = 0;
	value_type			data;

	std::atomic<bool>	activate{false};
	std::atomic<bool>	removed{false};
};

template<typename T>
using entry_ptr = std::unique_ptr<T>;

template<
	typename T,
	typename ...Args
>
inline entry_ptr<T> make_entry(Args&& ...args)
{ return std::make_unique<T>(std::forward<Args>(args)...); }

}	// namespace hashxx