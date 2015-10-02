#pragma once

#include <hashxx/entry.hpp>
#include <hashxx/purge.hpp>
#include <hashxx/iterator.hpp>

namespace hashxx {

struct container_base
{

};

template<
	typename T,
	typename Indexes
>
class container final
: public container_base
{
public:
	using self_type = container;
	using iterator = iterator<self_type>;
	using container_impl_type = container_impl<T>;
	using container_purge_type = container_purge<T>;

public:

	container()
	: container_purge_{container_impl_}
	{}

	explicit container(size_t container_size)
	: container_impl_{container_size},
	  container_purge_{}
	{}

	/// Data insertion
	iterator insert(const T& elem);

	template<typename ...Args>
	iterator emplace(Args&&... args);

	template<typename Callback>
	bool modify(iterator& it, Callback call);

	void erase(iterator it);

	template<typename Tag>
	iterator find();

	iterator end();

private:
	container_impl_type		container_impl_;
	container_purge_type	container_purge_;

};

}	// namespace hashxx
