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
inline void for_each(Tuple&& tuple, F&& func)
{
	constexpr size_t N = std::tuple_size< std::remove_reference_t<Tuple> >::value;
	for_each_impl(
		std::forward<Tuple>(tuple),
		std::forward<F>(func),
		std::make_index_sequence<N>{}
	);
}

template<
	typename Tuple1,
	typename Tuple2,
	typename F,
	size_t ...Indices
>
void for_tuples_impl(Tuple1&& t1, Tuple2&& t2, F&& func, std::index_sequence<Indices...>)
{
	using fake_array = int[];
	(void)fake_array{1,
		(func(std::get<Indices>(std::forward<Tuple1>(t1)), std::get<Indices>(std::forward<Tuple2>(t2))), void(), int{})...
	};
}

template<
	typename Tuple1,
	typename Tuple2,
	typename F

>
inline void for_tuples(Tuple1&& t1, Tuple2&& t2, F&& func)
{
	constexpr size_t N = std::tuple_size< std::remove_reference_t<Tuple1> >::value;

	for_tuples_impl(
		std::forward<Tuple1>(t1),
		std::forward<Tuple2>(t2),
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

	indexes_wrapper_type(const indexes_wrapper_type&) = delete;
	indexes_wrapper_type& operator= (const indexes_wrapper_type& ) = delete;

	/// Get index tuples
	template<size_t I>
	inline typename std::tuple_element<I, indexes_type>::type& get()
	{ return std::get<I>(indexes_); }

	/// Create tuple of indexed value
	static indexes_value_type get_values(entry_ptr entry)
	{ return std::make_tuple(Indexes::get(entry)...); }

	/// Clear all index
	inline void clear_indexes()
	{
		detail::for_each(indexes_, [](auto& index) {
			index.clear();
		});
	}

	/// Update new entry
	inline void update_new_index(entry_ptr entry)
	{
		detail::for_each(indexes_, [&](auto& index) {
			index.update_index(entry);
		});
	}

	/// Update index
	inline void update_index(entry_ptr entry, const indexes_value_type& old_value)
	{
		detail::for_tuples(
			indexes_,
			old_value,
			[&](auto& index, const auto& old_value) {
				index.update_index(entry, old_value);
			}
		);
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
