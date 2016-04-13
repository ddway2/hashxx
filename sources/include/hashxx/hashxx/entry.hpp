#pragma once

#include <atomic>
#include <memory>
#include <mutex>

#include <cstdlib>

#include <hashxx/alignment_option.hpp>

namespace hashxx {

const size_t default_container_size = 100000;

template<typename T>
struct entry {
	using value_type = T;

	constexpr static size_t value_size = sizeof(T);

	entry()
	: data((value_type*)std::malloc(value_size))
	{}

	~entry()
	{ std::free(data); }

	entry(const entry&) = delete;
	entry(entry&& ) = delete;

	entry& operator= (const entry& ) = delete;
	entry& operator= (entry&& ) = delete;

	size_t				index = 0;
	value_type*			data = nullptr;

	std::atomic<bool>	activate{false};
	std::atomic<bool>	removed{false};
};


}	// namespace hashxx
