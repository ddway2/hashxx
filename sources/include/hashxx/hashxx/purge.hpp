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
	inline bool purge_entries(Callback&& call)
	{
		entry_ptr entries[purge_bulk_size_];
		bool processed = false;

		size_t count = purge_queue_->try_dequeue_bulk(entries, purge_bulk_size_);
		while (count > 0) {
			for (size_t i = 0 ; i < count ; ++i) {
				call(entries[i]->data);
				entries[i]->removed = false;
				purge_size_--;
			}
			processed = true;
            impl_.purge_removed_bulk(entries, count);
            purged_count_ += count;
            count = purge_queue_->try_dequeue_bulk(entries, purge_bulk_size_);
		}
		return processed;
	}

	inline bool purge_entries()
	{
		entry_ptr entries[purge_bulk_size_];
		bool processed = false;

		size_t count = purge_queue_->try_dequeue_bulk(entries, purge_bulk_size_);
		while (count > 0) {
			for (size_t i = 0 ; i < count ; ++i) {
				entries[i]->data.~T();
				entries[i]->removed = false;
				purge_size_--;
			}
			processed = true;
            impl_.purge_removed_bulk(entries, count);
            purged_count_ += count;
            count = purge_queue_->try_dequeue_bulk(entries, purge_bulk_size_);
		}
		return processed;
	}

	inline size_t size() const
	{ return purge_size_.load(); }

	inline uint64_t purged_count() const
	{ return purged_count_.load(); }

	inline void set_purge_bulk(size_t bulk)
	{ purge_bulk_size_ = bulk; }

private:
	container_impl_type&		impl_;

	std::atomic<size_t>			purge_size_{0};
	std::atomic<uint64_t>		purged_count_{0};

    size_t                      purge_bulk_size_ = 1024;
	queue_container_ptr			purge_queue_;
};

}	// namespace hashxx
