#define BOOST_TEST_MODULE container_impl
#include <hashxx/unit_test.hpp>
#include <hashxx/system_config.hpp>

#include <hashxx/container_impl.hpp>

using namespace hashxx;

struct my_struct {
	uint64_t value1;
	uint64_t value2;
};

struct container_fixture
{};

using container_type = container_impl<my_struct>;


BOOST_FIXTURE_TEST_SUITE(container_impl_test, container_fixture);

BOOST_AUTO_TEST_CASE(create_container_and_get_1_entry)
{
	container_type c1{1000};

	auto v1 = c1.available_entry();

	BOOST_CHECK_EQUAL(c1.capacity(), 1000);
	BOOST_CHECK_EQUAL(c1.size(), 999);
	BOOST_CHECK(v1 != nullptr);

}

BOOST_AUTO_TEST_CASE(get_and_retrieve_entry)
{
	container_type c1{100};

	auto v1 = c1.available_entry();
	BOOST_CHECK_EQUAL(c1.size(), 99);
	c1.purge_removed(v1);
	BOOST_CHECK_EQUAL(c1.size(), 100);
}


BOOST_AUTO_TEST_SUITE_END();