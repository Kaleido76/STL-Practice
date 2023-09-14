#include "list.h"
#include "listTest.h"
#include <cassert>

using namespace PracticeStl;

static int res = 0;

template<typename T>
static int compareList(const std::initializer_list<T>& vs, list<T>& con)
{
	if (vs.size() != con.size()) return -1;
	auto v_cur = vs.begin();
	auto c_cur = con.begin();
	for (int i = 0; i < vs.size(); ++i)
	{
		if (*v_cur != *c_cur)
			return i;
		++v_cur;
		++c_cur;
	}
	return 0;
}

static void testcase_construct()
{
	{
		list<int> test;
		assert(test.size() == 0);
		assert(test.empty() == true);
	}

	{
		list<int> test = { 123, 456, 789 };
		assert(test.size() == 3);
		assert(test.empty() == false);
	}
}

static void testcase_traverse()
{
	{
		list<int> test = { 10, 10, 10 };
		compareList({ 10, 10, 10 }, test);

		decltype(test)::iterator iter;
		for (iter = test.begin(); iter != test.end(); ++iter)
		{
			assert(*iter == 10);
		}
	}
}

static void testcase_push_pop()
{
	list<int> test;
	test.push_back(1);
	res = compareList({ 1 }, test);
	assert(res == 0);

	test.push_back(5);
	res = compareList({ 1, 5 }, test);
	assert(res == 0);

	test.push_front(-8);
	res = compareList({ -8, 1, 5 }, test);
	assert(res == 0);

	test.pop_back();
	res = compareList({ -8, 1 }, test);
	assert(res == 0);

	test.pop_front();
	res = compareList({ 1 }, test);
	assert(res == 0);

	test.pop_back();
	assert(test.size() == 0);
	assert(test.empty() == true);
}

static void testcase_macroscopic()
{
	list<int> test = { 1, 3, 5, 6, 8 };
	test.clear();
	assert(test.size() == 0);
	assert(test.empty() == true);
}

static void testcase_insert_erase()
{
	list<int> test = { 1, 2, 3 };
	test.insert(test.begin(), 111);
	res = compareList({ 111, 1, 2, 3 }, test);
	assert(res == 0);

	test.insert(test.end(), 111);
	res = compareList({ 111, 1, 2, 3, 111 }, test);
	assert(res == 0);

	test.insert(test.end(), 123);
	res = compareList({ 111, 1, 2, 3, 111, 123 }, test);
	assert(res == 0);

	test.erase(test.begin());
	res = compareList({ 1, 2, 3, 111, 123 }, test);
	assert(res == 0);

	test.erase(--test.end());
	res = compareList({ 1, 2, 3, 111 }, test);
	assert(res == 0);

	test.erase(++test.begin(), --test.end());
	res = compareList({ 1, 111 }, test);
	assert(res == 0);
}

void TestUnits::ListTest::test_all()
{
	testcase_construct();
	testcase_traverse();
	testcase_macroscopic();
	testcase_push_pop();
	testcase_insert_erase();
}