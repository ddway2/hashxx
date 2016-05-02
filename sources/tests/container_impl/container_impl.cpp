#define BOOST_TEST_MODULE container_impl

#include <atomic>
#include <hashxx/unit_test.hpp>
#include <hashxx/system_config.hpp>

#include <hashxx/container_impl.hpp>

using namespace hashxx;

struct my_struct {
	uint64_t value1 = 1;
	uint64_t value2 = 2;
};

struct my_noncopyable_struct {
	struct foo {
		uint64_t value1 = 2;
	};

	std::unique_ptr<foo>	value1{new foo()};

	my_noncopyable_struct()
	{}
};

struct my_emplace_struct {
	std::atomic<uint64_t>	value1;
};

struct container_fixture
{
	my_struct m1;
	my_noncopyable_struct m2;
	my_emplace_struct m3;
};

using container_type = container_impl<my_struct>;
using container_noncopyable_type = container_impl<my_noncopyable_struct>;
using container_emplace_type = container_impl<my_emplace_struct>;

BOOST_FIXTURE_TEST_SUITE(container_impl_test, container_fixture);

BOOST_AUTO_TEST_CASE(create_container_and_get_1_entry)
{
	container_type c1{1000};


	auto v1 = c1.available_entry(m1);

	BOOST_CHECK_EQUAL(c1.capacity(), 1000);
	BOOST_CHECK_EQUAL(c1.available_size(), 999);
	BOOST_CHECK(v1 != nullptr);

}

BOOST_AUTO_TEST_CASE(create_container_and_get_entry_by_move)
{
	container_noncopyable_type c1{1000};

	m2.value1->value1 = 5;

	auto v1 = c1.available_entry(std::move(m2));

	BOOST_CHECK_EQUAL(c1.capacity(), 1000);
	BOOST_CHECK_EQUAL(c1.available_size(), 999);
	BOOST_CHECK(v1 != nullptr);
	BOOST_CHECK_EQUAL(v1->data->value1->value1, 5);
}

BOOST_AUTO_TEST_CASE(create_container_and_entry_by_inplace)
{
	container_emplace_type c1{1000};

	auto v1 = c1.emplace_entry();

	BOOST_CHECK_EQUAL(c1.capacity(), 1000);
	BOOST_CHECK_EQUAL(c1.available_size(), 999);
	BOOST_CHECK(v1 != nullptr);
}

BOOST_AUTO_TEST_CASE(get_and_retrieve_entry)
{
	container_type c1{100};

	auto v1 = c1.available_entry(m1);
	BOOST_CHECK_EQUAL(c1.available_size(), 99);
	c1.purge_removed(v1);
	BOOST_CHECK_EQUAL(c1.available_size(), 100);
}


BOOST_AUTO_TEST_SUITE_END();
