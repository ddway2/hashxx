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

	public:
		find_wrapper(container_impl_type& impl, indexes_type& indexes)
		: impl_(impl),
		  indexes_(indexes)
		{}

		template<typename Value>
		inline iterator find(const Value& v)
		{
			size_t pos = 0;
			if (indexes_.get<I>().find(v, pos)) {
				entry_ptr entry = impl_.at(pos);
				auto t = index_type::get(entry);
				std::cout << "index_type::get(entry): "<< demangle_type_name(typeid(t).name()) << std::endl;
				std::cout << "v: " << demangle_type_name(typeid(v).name()) << std::endl;


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

	template<size_t I>
	inline find_wrapper<I> get()
	{ return find_wrapper<I>{container_impl_, indexes_}; }


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
