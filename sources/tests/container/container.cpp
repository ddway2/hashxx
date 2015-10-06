#define BOOST_TEST_MODULE test_container
#include <hashxx/unit_test.hpp>
#include <hashxx/system_config.hpp>

#include <hashxx/container.hpp>
#include <hashxx/index/index_field.hpp>

using namespace hashxx;

struct my_struct {
	uint64_t		value1;
	uint32_t		value2;

	std::string		data;
};



struct container_fixture
{

};

using container_type = container<
	my_struct,
	index_by<
		my_struct,
		mem_index_type<my_struct, uint64_t, &my_struct::value1>,
		mem_index_type<my_struct, uint32_t, &my_struct::value2>
	>
>;

BOOST_FIXTURE_TEST_SUITE(container_test, container_fixture);

BOOST_AUTO_TEST_CASE(instanciate_container)
{
	container_type c1{1000};
	BOOST_CHECK_EQUAL(c1.size(), 0);
}

BOOST_AUTO_TEST_CASE(insert_one_value)
{
	container_type c1{100};
	my_struct m1;
	m1.value1 = 10;
	m1.value2 = 25;
	c1.insert(m1);
	BOOST_CHECK_EQUAL(c1.size(), 1);
}


BOOST_AUTO_TEST_SUITE_END();