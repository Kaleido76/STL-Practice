#include "deque.h"
#include "dequeTest.h"
#include <cassert>

using namespace PracticeStl;
static int res = 0;

template<typename T, typename C>
static int compare(const std::initializer_list<T>& vs, C& con)
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
		deque<int> test;
		assert(test.size() == 0);
		assert(test.empty() == true);
	}

	{
		deque<int> test(10, 20);
		assert(test.size() == 10);
		assert(test.empty() == false);
	}

	{
		deque<int> test(300, 20);
		assert(test.size() == 300);
		assert(test.empty() == false);
	}

	{
		deque<int> test = { 1, 3, 5 };
		assert(test.size() == 3);
		assert(test.empty() == false);
	}
}

static void testcase_traverse()
{
	{
		deque<int> test = { 10, 10, 10 };
		decltype(test)::iterator iter;
		for (iter = test.begin(); iter != test.end(); ++iter)
		{
			assert(*iter == 10);
		}
	}

	{
		deque<int> test = { 11, 12, 13 };
		assert(test[0] == 11);
		assert(test[1] == 12);
		assert(test[2] == 13);
	}

	{
		deque<int> test(300, 20);
		decltype(test)::iterator iter;
		for (iter = test.begin(); iter != test.end(); ++iter)
		{
			assert(*iter == 20);
		}
	}
}

static void testcase_push_pop()
{
	deque<int> test;
	test.push_back(1);
	res = compare({ 1 }, test);
	assert(res == 0);

	test.push_back(5);
	res = compare({ 1, 5 }, test);
	assert(res == 0);

	test.push_front(-8);
	res = compare({ -8, 1, 5 }, test);
	assert(res == 0);

	test.pop_back();
	res = compare({ -8, 1 }, test);
	assert(res == 0);

	test.pop_front();
	res = compare({ 1 }, test);
	assert(res == 0);

	test.pop_back();
	assert(test.size() == 0);
	assert(test.empty() == true);
}

static void testcase_macroscopic()
{
	deque<int> test = { 1, 3, 5, 6, 8 };

	test.clear();
	assert(test.size() == 0);
	assert(test.empty() == true);
}

static void testcase_insert_erase()
{
	deque<int> test = { 1, 2, 3 };
	test.insert(test.begin(), 111);
	res = compare({ 111, 1, 2, 3 }, test);
	assert(res == 0);

	test.insert(test.end(), 111);
	res = compare({ 111, 1, 2, 3, 111 }, test);
	assert(res == 0);

	test.insert(test.begin() + 3, 123);
	res = compare({ 111, 1, 2, 123, 3, 111 }, test);
	assert(res == 0);

	test.erase(test.begin());
	res = compare({ 1, 2, 123, 3, 111 }, test);
	assert(res == 0);

	test.erase(test.end() - 1);
	res = compare({ 1, 2, 123, 3 }, test);
	assert(res == 0);

	test.erase(test.begin() + 1, test.end() - 1);
	res = compare({ 1, 3 }, test);
	assert(res == 0);
}

void TestUnits::DequeTest::test_all()
{
	testcase_construct();
	testcase_traverse();
	testcase_push_pop();
	testcase_macroscopic();
	testcase_insert_erase();
}