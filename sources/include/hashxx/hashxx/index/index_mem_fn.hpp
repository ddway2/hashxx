#pragma once

#include <hashxx/entry.hpp>
#include <hashxx/index/index.hpp>

#include <unordered_map>

namespace hashxx {

template<
	typename T,
	typename MemberType,
	MemberType (T::* Func)(void),
	template<typename> class IndexType = index_type_impl
>
class mem_fn_index_type final
: public index_type_base
{
public:
	using entry_type = entry<T>;
	using entry_ptr = entry_type*;
	using index_value_type = MemberType;

	//using index_type = index_type_impl<index_value_type>;
	using index_type = IndexType<index_value_type>;
public:
	mem_fn_index_type()
	:  index_{default_container_size},
	   capacity_{default_container_size}
	{}

	mem_fn_index_type(size_t index_count)
	: index_{index_count},
	  capacity_{index_count}
	{}

	virtual ~mem_fn_index_type() = default;

	inline void clear()
	{ index_.clear(); }

	static index_value_type get(entry_ptr ptr)
	{  return (*(ptr->data).*Func)(); }

	inline void update_index(size_t pos, const index_value_type& new_value, const index_value_type& old_value)
	{
		if (new_value != old_value) {
			index_[new_value] = pos;
		}
	}

	inline void update_index(entry_ptr entry, const index_value_type& old_value)
	{ 
		index_value_type value = (*(entry->data).*Func)();
		if (value != old_value) {
			index_[value] = entry->index;
		}
	}

	inline void update_index(entry_ptr entry)
	{
		index_value_type value = (*(entry->data).*Func)();
		index_[value] = entry->index;
	}

	inline bool find(const index_value_type& v, size_t& pos) const
	{
		auto found = index_.find(v);
		if (found != index_.end()) {
			pos = found->second;
			return true;
		}
		return false;
	}

	inline bool check_index(const index_value_type& index)
	{ return index_.count(index) > 0; }

	inline size_t size() const
	{ return index_.size(); }
	
	inline size_t capacity() const
	{ return capacity_; }

	inline index_type& impl()
	{ return index_; }

private:
	index_type 				index_;
	size_t					capacity_;
};

}	// namespace hashxx
