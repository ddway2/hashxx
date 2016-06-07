#pragma once

#include <cstdlib>

#include <moodycamel/concurrent_queue.hpp>

#include <hashxx/entry.hpp>
#include <hashxx/compiler.hpp>

namespace hashxx {

template<typename T>
class container_impl
{
public:
	using value_type = T;
	using entry_type = entry<T>;
	using entry_ptr = entry_type*;
	using array_entry_type = entry_type*;

	using queue_container = moodycamel::concurrent_queue<entry_ptr>;
	using queue_container_ptr = std::unique_ptr<queue_container>;

	constexpr static size_t value_size = sizeof(T);
public:
	container_impl()
	: container_size_(default_container_size)
	{ init(); }

	explicit container_impl(size_t container_size)
	: container_size_(container_size)
	{ init(); }

	~container_impl()
	{ release(); }

	/// Available entries
	inline entry_ptr available_entry(const value_type& v)
	{
		entry_ptr entry = nullptr;
		if (unlike(!available_queue_->try_dequeue(entry))) {
			throw std::runtime_error("available_entry: no available entry");
		}
		available_size_--;
		new (entry->data) value_type();
		*(entry->data) = v;
		entry->activate = true;
		return entry;
	}

	/// Available entry (move semantic)
	inline entry_ptr available_entry(value_type&& v)
	{
		entry_ptr entry = nullptr;
		if (unlike(!available_queue_->try_dequeue(entry))) {
			throw std::runtime_error("available_entry: no available entry");
		}
		available_size_--;
		new (entry->data) value_type();
		*(entry->data) = std::move(v);
		entry->activate = true;
		return entry;
	}

	/// Emplace entry
	template<typename ...Args>
	entry_ptr emplace_entry(Args&& ...args)
	{
		entry_ptr entry = nullptr;
		if (unlike(!available_queue_->try_dequeue(entry))) {
			throw std::runtime_error("emplace_entry: no available entry");
		}
		available_size_--;
		new (entry->data) value_type(std::forward<Args>(args)...);
		entry->activate = true;
		return entry;
	}

	/// Purge entries
	inline void purge_removed(entry_ptr entry) noexcept
	{
		available_size_++;
		if (unlike(!available_queue_->try_enqueue(entry))) {
			throw std::runtime_error("purge_removed: try enqueue failed");
		}
	}
    
    /// Purge entries in bulk mode
    inline void purge_removed_bulk(entry_ptr* it, size_t len) noexcept
    {
        available_size_ += len;
        if (unlike(!available_queue_->try_enqueue_bulk(it, len))) {
        	throw std::runtime_error("purge_removed_bulk: try enqueue failed");
        }
    }

	/// for_each
	template<typename Function>
	inline void for_each(Function&& f)
	{
		for (size_t i = 0 ; i < container_size_ ; ++i) {
			if (container_[i].activate.load()) {
				f(&(container_[i]));
			}
		}
	}

	/// At function
	inline entry_ptr at(size_t i)
	{ return &(container_[i]); }

	/// Available size in container
	inline size_t available_size() const
	{ return available_size_.load(); }

	/// Max capacity
	inline size_t capacity() const
	{ return container_size_; }

private:
	void init() {
		container_ = new entry_type[container_size_];
		available_queue_ = std::make_unique<queue_container>(container_size_);

		for (size_t i = 0 ; i < container_size_ ; ++i) {
			container_[i].index = i;
			available_queue_->enqueue(&(container_[i]));
		}
		available_size_ = container_size_;

	}

	void release()
	{ delete [] container_; }

private:
	size_t 					container_size_{0};
	array_entry_type		container_{nullptr};

	std::atomic<size_t>		available_size_{0};
	queue_container_ptr		available_queue_;
};

}	// namespace hashxx
