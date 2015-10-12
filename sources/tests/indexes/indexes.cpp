#define BOOST_TEST_MODULE indexes
#include <hashxx/unit_test.hpp>
#include <hashxx/system_config.hpp>

#include <hashxx/index/index_field.hpp>

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

using my_index1 = mem_index_type<
	my_struct,
	uint64_t,
	&my_struct::value1
>;

using my_index2 = mem_index_type<
	my_struct,
	uint32_t,
	&my_struct::value2
>;

BOOST_FIXTURE_TEST_SUITE(indexes_test, indexes_fixture);

BOOST_AUTO_TEST_CASE(create_index_and_fill_it)
{
	my_index1 index;

	index.update_index(1, 2, 3);
	BOOST_CHECK_EQUAL(index.size(), 1);
	BOOST_CHECK(index.check_index(2));
}

BOOST_AUTO_TEST_CASE(index_with_object)
{
	my_index1 index;

	my_index1::entry_type entry;
	entry.data.value1 = 3;

	index.update_index(&entry);
	BOOST_CHECK_EQUAL(index.size(), 1);	
	BOOST_CHECK(index.check_index(3));
}

BOOST_AUTO_TEST_CASE(get_index_from_entry)
{
	my_index1 index;

	my_index1::entry_type entry;
	entry.data.value1 = 15;

	BOOST_CHECK_EQUAL(my_index1::get(&entry), 15);
}

BOOST_AUTO_TEST_SUITE_END();
