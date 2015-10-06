#define BOOST_TEST_MODULE indexes_desc

#include <hashxx/unit_test.hpp>
#include <hashxx/system_config.hpp>

#include <string>

#include <hashxx/indexes.hpp>

struct my_struct {
	uint64_t value1;
	uint32_t value2;

	std::string data;	
};

struct indexes_desc_fixture {

};


using namespace hashxx;

using index1_type = index_by<
	my_struct,
	mem_index_type<my_struct, uint64_t, &my_struct::value1>,
	mem_index_type<my_struct, uint32_t, &my_struct::value2>
>;

BOOST_FIXTURE_TEST_SUITE(indexes_desc_test, indexes_desc_fixture);

BOOST_AUTO_TEST_CASE(instanciates_indexes_desc_with_one_index)
{
	index1_type index;
	BOOST_CHECK_EQUAL(index.size(), 2);
}


BOOST_AUTO_TEST_SUITE_END();
