#define BOOST_TEST_MODULE test_container
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

struct container_fixture
{
	bool done = false;
};

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

BOOST_AUTO_TEST_CASE(insert_and_find_one_element)
{
	container_type c1{100};
	my_struct m1;
	m1.value1 = 10;
	m1.value2 = 25;
	m1.data = "toto";
	c1.insert(m1);

	{
		auto it = c1.get<0>().find(10);
		BOOST_CHECK_MESSAGE(it != c1.end(), "Check iterator validity");
		BOOST_CHECK_EQUAL(it->data, "toto");
	}
}

BOOST_AUTO_TEST_CASE(insert_find_and_modify_one_element)
{
	container_type c1{100};
	my_struct m1;
	m1.value1 = 10;
	m1.value2 = 25;
	m1.data = "toto";
	c1.insert(m1);

	auto it = c1.get<0>().find(10);
	BOOST_CHECK_MESSAGE(it != c1.end(), "Check iterator validity for it");

	bool result = c1.modify(it, [](auto& v){
		v.value1 = 11;
	});

	BOOST_CHECK_MESSAGE(result, "Modify Done");

	auto found_10 = c1.get<0>().find(10);
	BOOST_CHECK_MESSAGE(found_10 == c1.end(), "Check iterator validity for found_10");

	auto found_11 = c1.get<0>().find(11);
	BOOST_CHECK_MESSAGE(found_11 != c1.end(), "Check iterator validity for found_11");
	BOOST_CHECK_EQUAL(found_11->data, "toto");

}

BOOST_AUTO_TEST_CASE(insert_find_one_element_by_mem_fn)
{
	container_type c1{100};
	my_struct m1;
	m1.value1 = 10;
	m1.value2 = 25;
	m1.value3 = 20;
	m1.data = "toto";
	c1.insert(m1);

	auto found_20 = c1.get<2>().find(20);
	BOOST_CHECK_MESSAGE(found_20 != c1.end(), "check iterator validity for found_20");
}

BOOST_AUTO_TEST_CASE(emplace_100_elements_and_random)
{
	container_type c1{1000};

	for (uint32_t i = 0 ; i < 100 ; ++i) {
		c1.emplace(i, i + 2000, "toto " + std::to_string(i));
	}
	BOOST_CHECK_MESSAGE(c1.size() == 100, "Emplace 100 elements");
}

BOOST_AUTO_TEST_CASE(emplace_100_element_and_loop_for_each)
{
	container_type c1{1000};

	for (uint32_t i = 0 ; i < 100 ; ++i) {
		c1.emplace(i, i + 2000, "toto " + std::to_string(i));
	}

	uint32_t count = 0;
	c1.for_each([&](auto it) {
		BOOST_CHECK_EQUAL(it->value1, count);
		count++;
	});
	BOOST_CHECK_MESSAGE(count == 100, "Check number of message");
}

BOOST_AUTO_TEST_CASE(insert_100_moveable_element_and_loop_foreach)
{
	container_type c1{1000};

	for (uint32_t i = 0 ; i < 100 ; ++i) {
		my_struct c{i, i + 2000, "toto " + std::to_string(i)};
		c1.insert(std::move(c));
	}

	uint32_t count = 0;
	c1.for_each([&](auto it) {
		BOOST_CHECK_EQUAL(it->value1, count);
		count++;
	});
	BOOST_CHECK_MESSAGE(count == 100, "Check number of message");
}

BOOST_AUTO_TEST_SUITE_END();
