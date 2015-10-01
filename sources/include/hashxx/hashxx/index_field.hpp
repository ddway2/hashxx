#pragma once

#include <hashxx/entry.hpp>

#include <unordered_map>

namespace hashxx {

const size_t	default_index_count = 300000;

using pos_value_type = size_t;

template<typename T>
using index_type_impl = std::unordered_map<T, pos_value_type>;

struct index_type_base
{
	virtual ~index_type_base() = default;
}

template<
	typename T,
	typename MemberAccessor,
	typename MemberType
>
class mem_index_type final
: public index_type_base
{
public:
	using entry_type = entry<T>;
	using index_accessor_type = T::* MemberAccessor;
	using index_value_type = MemberType;

	using index_type = index_type_impl<index_value_type>;
public:
	mem_index_type()
	: index_{default_index_count}
	{}

	mem_index_type(size_t index_count)
	: index_{index_count}
	{}

	mem_index_type(const mem_index_type& ) = delete;
	mem_index_type& operator=(const mem_index_type& ) = delete;

	virtual ~mem_index_type() = default;

	inline void clear()
	{ index_.clear(); }

	inline index_value_type get_index(const entry_type& obj)
	{ return (obj.data).*member_; }

	inline void update_index(pos_value_type pos, const index_value_type& new_value, const index_value_type& old_value)
	{
		if (new_value != old_value) {
			index_[new_value] = pos;
		}
	}

	inline void update_index(pos_value_type pos, const entry_type& obj, const index_value_type& old_value)
	{ 
		index_value_type value = (obj.data).*member_; 
		if (value != old_value) {
			index_[value] = pos;
		}
	}

	inline void update_index(pos_value_type pos, const entry_type& obj)
	{
		index_value_type value = (obj.data).*member_;
		index_[value] = pos;
	}

	inline size_t size() const
	{ return index_.size(); }
private:
	index_accessor_type		member_;
	index_type 				index_;
};




}	// namespace hashxx
