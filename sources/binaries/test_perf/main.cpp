
#include <iostream>
#include <hashxx/container.hpp>

struct fake_bearer
{
	uint32_t		orig_id;
	uint32_t		dest_id;

	uint32_t		opc;
	uint32_t		dpc;

	inline uint64_t orig_spc_key() const
	{ return (uint64_t)opc << 32 | orig_id; }

	inline uint64_t dest_spc_key() const
	{ return (uint64_t)dpc << 32 | dest_id; }
};


using namespace hashxx;
using container_type = container<
	fake_bearer,
	index_by<
		mem_fn_index_type<fake_bearer, uint64_t, &fake_bearer::orig_spc_key>,
		mem_fn_index_type<fake_bearer, uint64_t, &fake_bearer::dest_spc_key>
	>
>;


int main(int argc, char** argv)
{
	container_type container;
	bool done = false;

	std::thread t_insert([&](){
		size_t count = 0;

		while (!done) {
			fake_bearer b;

			b.orig_id = count % 100000;
			b.dest_id = (count % 100000) + (count % 100);

			++count;
		}
	});

	std::thread t_purge([&](){

	});
}