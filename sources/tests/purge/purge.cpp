#define BOOST_TEST_MODULE purge
#include <hashxx/unit_test.hpp>
#include <hashxx/system_config.hpp>

#include <hashxx/index/index_field.hpp>
#include <hashxx/container_impl.hpp>
#include <hashxx/purge.hpp>

using namespace hashxx;

bool destroy_struct = false;

struct my_struct  {
	uint64_t		value1;
	uint32_t		value2;

	std::string		data;

	my_struct()
	{}

	~my_struct()
	{
		destroy_struct = true;
	}
};

struct purge_fixture
{
	my_struct m1;
};

using container_type = container_impl<my_struct>;
using purge_type = container_purge<my_struct>;

BOOST_FIXTURE_TEST_SUITE(purge_test, purge_fixture);

BOOST_AUTO_TEST_CASE(instanciate_purge)
{
	container_type c1;
	purge_type p1{c1};

	BOOST_CHECK_EQUAL(p1.size(), 0);
}

BOOST_AUTO_TEST_CASE(erase_entry_and_purge)
{
	container_type c1;
	purge_type p1{c1};

	auto v1 = c1.available_entry(m1);
	destroy_struct = false;
	p1.erase_entry(v1);

	BOOST_CHECK_MESSAGE(!destroy_struct, "Not yet destroy");
	BOOST_CHECK_EQUAL(p1.size(), 1);

	int check_count = 0;
	p1.purge_entries([&](auto& v){
		++check_count;
	});

	BOOST_CHECK_MESSAGE(destroy_struct, "Message destroy");

	BOOST_CHECK_EQUAL(check_count, 1);
	BOOST_CHECK_EQUAL(p1.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END();
