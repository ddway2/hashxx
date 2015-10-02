#pragma once

#include <hashxx/entry.hpp>

namespace hashxx {
	
template<
	typename Container
>
class iterator {
public:
	using container_type = Container;
	using value_type = typename T::value_type;
	using entry_type = entry<value_type>;

	using self_type = iterator;
	using pointer = T*;
	using reference = T&;

public:

	explicit iterator(entry_type* entry)
	: entry_(entry)
	{}

	inline pointer operator->() 
	{ 
		T* result = nullptr;
		if (entry_) {
			result = &(entry_->data);
		} else {
			throw std::runtime_error("hashxx::iterator operator-> - cannot dereference null value");
		}
		return result; 
	}

	inline reference operator*()
	{
		T* result = nullptr;
		if (entry_) {
			result = &(entry_->data);
		} else {
			throw std::runtime_error("hash::iterator operator* - cannot dereference null value");
		}
		return *result;
	}



private:
	entry_type*		entry_{nullptr};
};

}	// namespace hashxx
