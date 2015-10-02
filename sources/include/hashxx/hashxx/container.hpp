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
	using iterator = iterator_impl<self_type>;
	using container_impl_type = container_impl<T>;
	using container_purge_type = container_purge<T>;

	using entry_type = entry<T>;
	using entry_ptr = entry_type*;
public:

	container()
	: container_purge_{container_impl_}
	{}

	explicit container(size_t container_size)
	: container_impl_{container_size},
	  container_purge_{container_impl_}
	{}

	/// Data insertion
	iterator insert(const T& elem)
	{
		auto entry = container_impl_.available_entry();
		entry.data = elem;

		index_new_entry(entry);

		return iterator{entry};
	}

	template<typename ...Args>
	iterator emplace(Args&&... args)
	{
		auto entry = container_impl_.available_entry();
		entry.data = T(std::forward<Args>(args)...);

		index_new_entry(entry);

		return iterator{entry};
	}

	template<typename Callback>
	bool modify(iterator& it, Callback call)
	{
		bool result = false;
		if (it.is_valid()) {
			// TODO: get old value index
			call(*it);
			// TODO: compute new index (if necessary)
			result = true;
		}
		return result;
	}

	void erase(iterator it)
	{
		if (like(it.is_valid())) {
			if (!it.entry_->removed.load()) {
				container_purge_->erase_entry(it.entry_);
			}
		}
	}

	template<typename Tag>
	iterator find()
	{
		return iterator();
	}

	inline iterator end() const
	{ return iterator(); }

private:
	void index_new_entry(entry_ptr entry)
	{
		// TODO: indexation
	}

	void index_entry(entry_ptr entry)
	{
		// TODO: indexation
	}

private:
	container_impl_type		container_impl_;
	container_purge_type	container_purge_;

};

}	// namespace hashxx
