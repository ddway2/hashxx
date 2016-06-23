#define BOOST_TEST_MODULE test_iterator
#include <hashxx/unit_test.hpp>
#include <hashxx/system_config.hpp>

#include <hashxx/container.hpp>
#include <hashxx/index/index_field.hpp>
#include <hashxx/index/index_mem_fn.hpp>

using namespace hashxx;

struct my_struct {
	my_struct() = default;

	my_struct(uint64_t v1, uint32_t v2, const std::string& d)
	: value1{v1},
	  value2{v2},
	  data{d}
	{}

	inline uint32_t	index_fn()
	{ return value3; }

	uint64_t		value1;
	uint32_t		value2;
	uint32_t 		value3;

	std::string		data;
};

using container_type = container<
	my_struct,
	index_by<
		my_struct,
		mem_index_type<my_struct, uint64_t, &my_struct::value1>,
		mem_index_type<my_struct, uint32_t, &my_struct::value2>,
		mem_fn_index_type<my_struct, uint32_t, &my_struct::index_fn>
	>
>;

struct iterator_fixture {

};

BOOST_FIXTURE_TEST_SUITE(iterator_test, iterator_fixture);

BOOST_AUTO_TEST_CASE(create_iterator)
{
	container_type c1{100};
	my_struct m1;
	m1.value1 = 10;
	m1.value2 = 25;
	auto it = c1.insert(m1);

	BOOST_CHECK(it.is_valid());
}


BOOST_AUTO_TEST_CASE(const_iterator_check)
{
	container_type c1{100};
	my_struct m1;
	m1.value1 = 10;
	m1.value2 = 25;
	auto it = c1.insert(m1);

	auto cf = [](const container_type::iterator& it){
		BOOST_CHECK(it.is_valid());
	};

	cf(it);
}

BOOST_AUTO_TEST_SUITE_END();
