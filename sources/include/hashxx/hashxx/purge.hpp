#pragma once

#include <hashxx/container_impl.hpp>
#include <moodycamel/concurrent_queue.hpp>

namespace hashxx {
	
template<typename T>
class container_purge
{
public:
	using container_impl_type = container_impl<T>;

	using entry_type = entry<T>;
	using entry_ptr = entry_type*;

	using queue_container = moodycamel::concurrent_queue<entry_ptr>;
	using queue_container_ptr = std::unique_ptr<queue_container>;
public:
	container_purge(container_impl_type& impl)
	: impl_(impl)
	{
		purge_queue_ = std::make_unique<queue_container>(impl_.capacity());
	}

	inline void erase_entry(entry_ptr entry)
	{
		if (!entry->removed.load()) {
			entry->removed = true;
			entry->activate = false;

			purge_size_++;
			purge_queue_->enqueue(entry);
		}
	}

	template<typename Callback>
	void purge_entries(Callback call)
	{
		entry_ptr entries[1024];
		while (size_t count = purge_queue_->try_dequeue_bulk(entries, 1024) > 0) {
			for (size_t i = 0 ; i < count ; ++i) {
				call(entries[i]->data);
				entries[i]->data.~T();
				entries[i]->removed = false;
				impl_.purge_removed(entries[i]);
				purge_size_--;
			}
		}
	}

	void purge_entries()
	{
		entry_ptr entries[1024];
		while (size_t count = purge_queue_->try_dequeue_bulk(entries, 1024) > 0) {
			for (size_t i = 0 ; i < count ; ++i) {
				entries[i]->data.~T();
				entries[i]->removed = false;
				impl_.purge_removed(entries[i]);
				purge_size_--;
			}
		}
	}

	inline size_t size() const
	{ return purge_size_.load(); }

private:
	container_impl_type&		impl_;

	std::atomic<size_t>			purge_size_{0};
	queue_container_ptr			purge_queue_;
};

}	// namespace hashxx
