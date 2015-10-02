#define BOOST_TEST_MODULE test_container
#include <hashxx/unit_test.hpp>
#include <hashxx/system_config.hpp>

#include <hashxx/container.hpp>

using namespace hashxx;

struct my_struct {
	uint64_t		value1;
	uint32_t		value2;
};

struct fake_indexes {};

struct container_fixture
{

};

using container_type = container<
	my_struct,
	fake_indexes
>;

BOOST_FIXTURE_TEST_SUITE(container_test, container_fixture);

BOOST_AUTO_TEST_CASE(instanciate_container)
{
	container_type c1{1000};
}


BOOST_AUTO_TEST_SUITE_END();