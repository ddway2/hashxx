#pragma once

#include <atomic>
#include <memory>

#include <hashxx/alignment_option.hpp>

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


}	// namespace hashxx
