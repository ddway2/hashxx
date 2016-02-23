#pragma once

#include <tuple>
#include <iostream>
#include <typeinfo>

#include <hashxx/entry.hpp>
#include <hashxx/purge.hpp>
#include <hashxx/iterator.hpp>
#include <hashxx/indexes.hpp>

#include <hashxx/compiler.hpp>

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

	template<size_t I>
	class find_wrapper
	{
	public:
		using index_type = typename std::tuple_element<I, typename indexes_type::indexes_type>::type;
		using member_type = typename index_type::index_value_type;

	public:
		find_wrapper(container_impl_type& impl, indexes_type& indexes)
		: impl_(impl),
		  indexes_(indexes)
		{}

		inline iterator find(const member_type& v)
		{
			size_t pos = 0;
			if (indexes_.get<I>().find(v, pos)) {
				entry_ptr entry = impl_.at(pos);
				if (
						entry->activate.load() && 
						!entry->removed.load() &&
						index_type::get(entry) == v
					) {
					return iterator(entry);
				}
			} 
			return iterator();
		}

	private:
		container_impl_type& 	impl_;
		indexes_type&			indexes_;
	};


public:

	container()
	: container_purge_{container_impl_}
	{}

	explicit container(size_t container_size)
	: container_impl_{container_size},
	  container_purge_{container_impl_},
	  indexes_{container_size * 2}
	{}

	/// Data insertion
	iterator insert(const T& elem)
	{
		auto entry = container_impl_.available_entry();
		entry->data = elem;

		indexes_.update_new_index(entry);
		check_reindex();

		return iterator{entry};
	}

	template<typename ...Args>
	iterator emplace(Args&&... args)
	{
		auto entry = container_impl_.available_entry();
		entry->data = T(std::forward<Args>(args)...);

		indexes_.update_new_index(entry);
		check_reindex();

		return iterator{entry};
	}

	template<typename Callback>
	bool modify(iterator& it, Callback&& call)
	{
		bool result = false;
		if (like(it.is_valid())) {
			auto old_values = indexes_type::get_values(it.get());
			call(*it);
			indexes_.update_index(it.get(), old_values);
			result = true;
		}
		return result;
	}

	void erase(iterator it)
	{
		if (like(it.is_valid())) {
			if (!it.get()->removed.load()) {
				container_purge_->erase_entry(it.get());
			}
		}
	}

	template<typename Callback>
	inline bool purge_entries(Callback&& call)
	{ return container_purge_->purge_entries(std::forward<Callback>(call)); }

	inline bool purge_entries()
	{ return container_purge_->purge_entries(); }

	template<typename Callback>
	inline void for_each(Callback&& call)
	{ container_impl_.for_each(std::forward<Callback>(call)); }

	template<size_t I>
	inline find_wrapper<I> get()
	{ return find_wrapper<I>{container_impl_, indexes_}; }


	inline iterator end() const
	{ return iterator(); }

	inline size_t size() const
	{ return container_impl_.capacity() - container_impl_.size(); }

	inline size_t capacity() const
	{ return container_impl_.capacity(); }

	inline void force_reindex()
	{ reindex(); }

private:
	inline void reindex()
	{
		indexes_.clear_indexes();
		container_impl_.for_each([&](entry_ptr entry){
			indexes_.update_new_index(entry);
		});
	}

	inline void check_reindex()
	{
		if (insert_count_ >= container_impl_.capacity()) {
			insert_count_ = 0;
			reindex();
		}
		++insert_count_;
	}

private:
	container_impl_type		container_impl_;
	container_purge_type	container_purge_;
	indexes_type			indexes_;

	size_t					insert_count_ = 0;

};

}	// namespace hashxx
