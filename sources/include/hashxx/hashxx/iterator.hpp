#pragma once

#include <hashxx/entry.hpp>
#include <hashxx/compiler.hpp>

namespace hashxx {
	
template<
	typename Container
>
class iterator_impl {
public:
	using container_type = Container;
	using value_type = typename Container::value_type;
	using entry_type = entry<value_type>;

	using self_type = iterator_impl;
	using pointer = value_type*;
	using reference = value_type&;

public:
	iterator_impl()
	: entry_{nullptr}
	{}

	explicit iterator_impl(entry_type* entry)
	: entry_(entry)
	{}

	iterator_impl(const iterator_impl& ) = default;
	iterator_impl(iterator_impl&& ) = default;
	iterator_impl& operator=(const iterator_impl& ) = default;
	iterator_impl& operator=(iterator_impl&& ) = default;

	inline pointer operator->() 
	{ 
		pointer result = nullptr;
		if (like(entry_)) {
			result = &(entry_->data);
		} else {
			throw std::runtime_error("hashxx::iterator_impl operator-> - cannot dereference null value");
		}
		return result; 
	}

	inline reference operator*()
	{
		pointer result = nullptr;
		if (like(entry_)) {
			result = &(entry_->data);
		} else {
			throw std::runtime_error("hash::iterator_impl operator* - cannot dereference null value");
		}
		return *result;
	}

	friend bool operator== (const iterator_impl& ls, const iterator_impl& rs)
	{ return ls.entry_ == rs.entry_; }

	friend bool operator!= (const iterator_impl& ls, const iterator_impl& rs)
	{ return ls.entry_ != rs.entry_; }

	inline bool is_valid() const
	{ return entry_ != nullptr; }

	inline entry_type* get()
	{ return entry_; }

private:
	entry_type*		entry_{nullptr};
};

}	// namespace hashxx
