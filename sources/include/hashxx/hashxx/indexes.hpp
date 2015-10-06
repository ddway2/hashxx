#pragma once

#include <tuple>
#include <utility>

#include <hashxx/entry.hpp>
#include <hashxx/index/index_field.hpp>

namespace hashxx {
namespace detail {

template<
	typename Tuple,
	typename F,
	size_t ...Indices
>
void for_each_impl(Tuple&& tuple, F&& func, std::index_sequence<Indices...>)
{
	using fake_array = int[];
	(void)fake_array{1, 
		(func(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...
	};
}

template<
	typename Tuple,
	typename F
>
void for_each(Tuple&& tuple, F&& func)
{
	constexpr size_t N = std::tuple_size< std::remove_reference_t<Tuple> >::value;
	for_each_impl(
		std::forward<Tuple>(tuple),
		std::forward<F>(func),
		std::make_index_sequence<N>{}
	);
}

}	// namespace detail

template<
	typename T,
	typename ...Indexes
>
class indexes_wrapper_type {
public:
	using indexes_type = std::tuple<Indexes...>;
	using indexes_value_type = std::tuple<typename Indexes::index_value_type...>;
	using entry_type = entry<T>;
	using entry_ptr = entry_type*;


public:
	indexes_wrapper_type()
	: indexes_wrapper_type(default_container_size)
	{}

	indexes_wrapper_type(size_t index_count)
	: indexes_(Indexes{index_count}...) 
	{}

	template<size_t I>
	inline typename std::tuple_element<I, indexes_type>::type& get()
	{ return std::get<I>(indexes_); }

	static indexes_value_type get_values(entry_ptr entry)
	{
		return std::make_tuple(Indexes::get(entry)...);
	}

	inline void clear_indexes()
	{
		detail::for_each(indexes_, [](auto& index) {
			index.clear();
		});
	}

	inline void update_new_index(entry_ptr entry)
	{
		size_t pos = entry->index;
		detail::for_each(indexes_, [&](auto& index) {
			index.update_index(pos, entry);
		});
	}

	inline size_t size() const
	{ return std::tuple_size<indexes_type>::value; }

private:
	indexes_type indexes_;
};

template<
	typename T,
	typename ...Indexes
>
using index_by = indexes_wrapper_type<T, Indexes...>;

}	// namespace hashxx
