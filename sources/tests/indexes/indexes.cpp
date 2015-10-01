#define BOOST_TEST_MODULE indexes
#include <hashxx/unit_test.hpp>
#include <hashxx/system_config.hpp>

#include <hashxx/index_field.hpp>

struct my_struct
{
	uint64_t		value1;
	uint32_t		value2;
};


struct indexes_fixture
{
	my_struct m1;
	my_struct m2;
};


//
// Define struct
//

using namespace hashxx;

using my_index = mem_index_type<
	my_struct,
	uint64_t,
	&my_struct::value1
>;

BOOST_FIXTURE_TEST_SUITE(indexes_test, indexes_fixture);

BOOST_AUTO_TEST_CASE(create_index)
{
	my_index index;

	index.update_index(1, 2, 3);
	BOOST_CHECK_EQUAL(index.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END();
