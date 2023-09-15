#include "deque.h"
#include "dequeTest.h"
#include "testUtil.h"

using namespace PracticeStl;

template<typename T>
static int compare(const std::initializer_list<T>& vs, deque<T>& con)
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
		EXPECT(0, test.size());
		EXPECT(true, test.empty());
	}

	{
		deque<int> test(10, 20);
		EXPECT(10, test.size());
		EXPECT(false, test.empty());
	}

	{
		deque<int> test(300, 20);
		EXPECT(300, test.size());
		EXPECT(false, test.empty());
	}

	{
		deque<int> test = { 1, 3, 5 };
		EXPECT(3, test.size());
		EXPECT(false, test.empty());
	}
}

static void testcase_traverse()
{
	{
		deque<int> test = { 10, 10, 10 };
		decltype(test)::iterator iter;
		for (iter = test.begin(); iter != test.end(); ++iter)
		{
			EXPECT(10, *iter);
		}
	}

	{
		deque<int> test = { 11, 12, 13 };
		EXPECT(11, test[0]);
		EXPECT(12, test[1]);
		EXPECT(13, test[2]);
	}
}

static void testcase_push_pop()
{
	deque<int> test;
	test.push_back(1);
	EXPECT(0, compare({ 1 }, test));

	test.push_back(5);
	EXPECT(0, compare({ 1, 5 }, test));

	test.push_front(-8);
	EXPECT(0, compare({ -8, 1, 5 }, test));

	test.pop_back();
	EXPECT(0, compare({ -8, 1 }, test));

	test.pop_front();
	EXPECT(0, compare({ 1 }, test));

	test.pop_back();
	EXPECT(0, compare({ }, test));
	EXPECT(true, test.empty());
}

static void testcase_macroscopic()
{
	deque<int> test = { 1, 3, 5, 6, 8 };
	test.clear();
	EXPECT(0, compare({ }, test));
	EXPECT(true, test.empty());
}

static void testcase_insert_erase()
{
	deque<int> test = { 1, 2, 3 };
	test.insert(test.begin(), 111);
	EXPECT(0, compare({ 111, 1, 2, 3 }, test));


	test.insert(test.end(), 111);
	EXPECT(0, compare({ 111, 1, 2, 3, 111 }, test));

	test.insert(test.begin() + 3, 123);
	EXPECT(0, compare({ 111, 1, 2, 123, 3, 111 }, test));

	test.erase(test.begin());
	EXPECT(0, compare({ 1, 2, 123, 3, 111 }, test));

	test.erase(test.end() - 1);
	EXPECT(0, compare({ 1, 2, 123, 3 }, test));

	test.erase(test.begin() + 1, test.end() - 1);
	EXPECT(0, compare({ 1, 3 }, test));
}

void DequeTest::test_all()
{
	testcase_construct();
	testcase_traverse();
	testcase_push_pop();
	testcase_macroscopic();
	testcase_insert_erase();
}