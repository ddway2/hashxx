
#include <iostream>
#include <string>
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
	size_t max_count = 10000000;
	if (argc > 1) {
		std::string value = argv[1];
		max_count = std::stoull(value);
	}
	std::cout << "Test with " << max_count << " iterations" << std::endl;

	container_type container{100000};
	std::atomic<size_t> insert_count{0};

	std::thread t_purge([&](){
		while (!done.load()) {
			container.purge_entries(1024);
		}
	});

	auto start = std::chrono::system_clock::now();
	size_t count = 0;
	while (!done.load()) {
		fake_bearer b;

		b.orig_id = count % 100000;
		b.dest_id = (count % 100000) + (count % 100);

		if (count % 10 == 0) {
			container.insert(b);
			++insert_count;
		} else {
			auto it = container.get<0>().find(count % 500);
			if (it != container.end()) {
				container.modify(it, [&](auto& v){
					v.orig_id = count % 100;
				});
			}
		}
		auto it = container.get<0>().find(count % 1000);
		if (it != container.end()) {
			container.modify(it, [&](auto& v){
				v.orig_id = count % 200;
			});
		}
		++count;

		if (count % 10 == 0) {
			auto found = container.get<0>().find(count % 1500);
			container.erase(found);
		}

		if (count > 10000000)
			break;
	}
	auto stop = std::chrono::system_clock::now();
	t_purge.join();

	auto delay = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	std::cout << "Duration (us): " << delay << std::endl;
	std::cout << "Msg/s: " << (max_count / ((float)delay / 1000000.0f)) << std::endl;
	std::cout << "Msg duration avg: " << (delay / max_count) << std::endl;
}

#if 0
=pod

=head1 NAME

 euuidgen - an expandium tool

=head1 DESCRIPTION

 Generate an euuid.

=head1 SYNOPSIS

 prog

=cut
#endif
