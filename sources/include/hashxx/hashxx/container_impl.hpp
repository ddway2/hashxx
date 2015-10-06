#pragma once

#include <moodycamel/spsc_queue.hpp>

#include <hashxx/entry.hpp>

namespace hashxx {

template<typename T>
class container_impl
{
public:
	using value_type = T;
	using entry_type = entry<T>;
	using entry_ptr = entry_type*;
	using array_entry_type = entry_ptr*;

	using queue_container = moodycamel::spsc_queue<entry_ptr>;
	using queue_container_ptr = std::unique_ptr<queue_container>;
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
	inline entry_ptr available_entry()
	{
		entry_ptr entry = nullptr;
		if (!available_queue_->try_dequeue(entry)) {
			throw std::runtime_error("no available entry");
		}
		available_size_--;
		entry->activate = true;
		return entry;
	}

	/// Purge entries
	inline void purge_removed(entry_ptr entry) noexcept
	{
		available_size_++;
		available_queue_->enqueue(entry);
	}

	/// for_each
	template<typename Function>
	inline void for_each(Function&& f)
	{
		for (size_t i = 0 ; i < container_size_ ; ++i) {
			if (container_[i]->activate.load()) {
				f(container_[i]);
			}
		}
	}

	inline size_t size() const
	{ return available_size_.load(); }

	inline size_t capacity() const
	{ return container_size_; }

private:
	void init() {
		container_ = new entry_ptr[container_size_];
		available_queue_ = std::make_unique<queue_container>(container_size_);

		for (size_t i = 0 ; i < container_size_ ; ++i) {
			container_[i] = new entry_type();
			container_[i]->index = i;
			available_queue_->enqueue(container_[i]);
		}
		available_size_ = container_size_;

	}

	void release() {
		for (size_t i = 0 ; i < container_size_ ; ++i) {
			delete container_[i];
			container_[i] = nullptr;
		}
		delete [] container_;
	}

private:
	size_t container_size_{0};
	array_entry_type	container_{nullptr};

	std::atomic<size_t>		available_size_{0};

	queue_container_ptr		available_queue_;
};

}	// namespace hashxx
