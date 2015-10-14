
#include <iostream>
#include <hashxx/container.hpp>
#include <hashxx/index/index_field.hpp>
#include <hashxx/index/index_mem_fn.hpp>

struct fake_bearer
{
	uint32_t		orig_id;
	uint32_t		dest_id;

	uint32_t		opc;
	uint32_t		dpc;

	inline uint64_t orig_spc_key()
	{ return (uint64_t)opc << 32 | orig_id; }

	inline uint64_t dest_spc_key()
	{ return (uint64_t)dpc << 32 | dest_id; }
};


using namespace hashxx;
using container_type = container<
	fake_bearer,
	index_by<
		fake_bearer,
		mem_fn_index_type<fake_bearer, uint64_t, &fake_bearer::orig_spc_key>,
		mem_fn_index_type<fake_bearer, uint64_t, &fake_bearer::dest_spc_key>
	>
>;

std::atomic<bool> done{false};

int main(int argc, char** argv)
{
	container_type container;
	std::atomic<size_t> insert_count{0};

	std::thread t_insert([&](){
		size_t count = 0;
		while (!done.load()) {
			fake_bearer b;

			b.orig_id = count % 100000;
			b.dest_id = (count % 100000) + (count % 100);

			if (count % 30 == 0) {
				container.insert(b);
				++insert_count;
			} else {
				auto it = container.get<0>().find(count % 50000);
				if (it != container.end()) {
					container.modify(it, [](auto& v){
						v.orig_id = count % 100;
					});
				}
			}
			++count;
			
		}
	});

	std::thread t_purge([&](){
		while (!done.load()) {
			//TODO: purge entry
		}
	});

	while (!done.load()) {
		std::this_thread::sleep_for(std::chrono::seconds(20));
		std::cout << "insert_count: " << (insert_count.load() / 20) << " insert/s" << std::endl;
	}
}