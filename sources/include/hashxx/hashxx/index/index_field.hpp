#pragma once

#include <hashxx/entry.hpp>

#include <unordered_map>

namespace hashxx {

template<typename T>
using index_type_impl = std::unordered_map<T, size_t>;

struct index_type_base {};

template<
	typename T,
	typename MemberType,
	MemberType T::* MemberAccessor
>
class mem_index_type final
: public index_type_base
{
public:
	using entry_type = entry<T>;
	using index_value_type = MemberType;

	using index_type = index_type_impl<index_value_type>;
public:
	mem_index_type()
	: index_{default_container_size}
	{}

	mem_index_type(size_t index_count)
	: index_{index_count}
	{}

	virtual ~mem_index_type() = default;

	inline void clear()
	{ index_.clear(); }

	inline index_value_type get(const entry_type& obj)
	{ return (obj.data).*MemberAccessor; }

	inline void update_index(size_t pos, const index_value_type& new_value, const index_value_type& old_value)
	{
		if (new_value != old_value) {
			index_[new_value] = pos;
		}
	}

	inline void update_index(size_t pos, const entry_type& obj, const index_value_type& old_value)
	{ 
		index_value_type value = (obj.data).*MemberAccessor; 
		if (value != old_value) {
			index_[value] = pos;
		}
	}

	inline void update_index(size_t pos, const entry_type& obj)
	{
		index_value_type value = (obj.data).*MemberAccessor;
		index_[value] = pos;
	}

	inline bool check_index(const index_value_type& index)
	{ return index_.count(index) > 0; }

	inline size_t size() const
	{ return index_.size(); }
	
private:
	index_type 				index_;
};




}	// namespace hashxx
