#pragma once

#include <hashxx/entry.hpp>
#include <hashxx/purge.hpp>
#include <hashxx/iterator.hpp>
#include <hashxx/indexes.hpp>

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
	using value_type = T;
	using self_type = container;
	using iterator = iterator_impl<self_type>;
	using container_impl_type = container_impl<T>;
	using container_purge_type = container_purge<T>;

	using entry_type = entry<T>;
	using entry_ptr = entry_type*;

	using indexes_type = Indexes;
public:

	container()
	: container_purge_{container_impl_}
	{}

	explicit container(size_t container_size)
	: container_impl_{container_size},
	  container_purge_{container_impl_},
	  indexes_{container_size}
	{}

	/// Data insertion
	iterator insert(const T& elem)
	{
		auto entry = container_impl_.available_entry();
		entry->data = elem;

		indexes_.update_new_index(entry);

		return iterator{entry};
	}

	template<typename ...Args>
	iterator emplace(Args&&... args)
	{
		auto entry = container_impl_.available_entry();
		entry->data = T(std::forward<Args>(args)...);

		indexes_.update_new_index(entry);

		return iterator{entry};
	}

	template<typename Callback>
	bool modify(iterator& it, Callback call)
	{
		bool result = false;
		if (it.is_valid()) {
			auto old_value = indexes_type::get_value(it.entry_);
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

	inline size_t size() const
	{ return container_impl_.capacity() - container_impl_.size(); }

	inline size_t capacity() const
	{ return container_impl_.capacity(); }

private:
	void reindex()
	{
		indexes_.clear_indexes();
		container_impl_.for_each([&](entry_ptr entry){
			indexes_.update_new_index(entry);
		});
	}

	void index_entry(entry_ptr entry)
	{
		// TODO: indexation
	}

private:
	container_impl_type		container_impl_;
	container_purge_type	container_purge_;
	indexes_type			indexes_;

};

}	// namespace hashxx
