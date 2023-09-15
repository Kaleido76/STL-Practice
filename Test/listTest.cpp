#include "list.h"
#include "listTest.h"
#include "testUtil.h"

using namespace PracticeStl;

template<typename T>
static int compare(const std::initializer_list<T>& vs, list<T>& con)
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
		EXPECT(0, test.size());
		EXPECT(true, test.empty());
	}

	{
		list<int> test = { 123, 456, 789 };
		EXPECT(3, test.size());
		EXPECT(false, test.empty());
	}
}

static void testcase_traverse()
{
	{
		list<int> test = { 10, 10, 10 };
		compare({ 10, 10, 10 }, test);

		decltype(test)::iterator iter;
		for (iter = test.begin(); iter != test.end(); ++iter)
		{
			EXPECT(10, *iter);
		}
	}
}

static void testcase_push_pop()
{
	list<int> test;
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
	EXPECT(0, test.size());
	EXPECT(true, test.empty());
}

static void testcase_macroscopic()
{
	list<int> test = { 1, 3, 5, 6, 8 };
	test.clear();
	EXPECT(0, test.size());
	EXPECT(true, test.empty());
}

static void testcase_insert_erase()
{
	list<int> test = { 1, 2, 3 };
	test.insert(test.begin(), 111);
	EXPECT(0, compare({ 111, 1, 2, 3 }, test));

	test.insert(test.end(), 111);
	EXPECT(0, compare({ 111, 1, 2, 3, 111 }, test));

	test.insert(test.end(), 123);
	EXPECT(0, compare({ 111, 1, 2, 3, 111, 123 }, test));

	test.erase(test.begin());
	EXPECT(0, compare({ 1, 2, 3, 111, 123 }, test));

	test.erase(--test.end());
	EXPECT(0, compare({ 1, 2, 3, 111 }, test));

	test.erase(++test.begin(), --test.end());
	EXPECT(0, compare({ 1, 111 }, test));
}

void ListTest::test_all()
{
	testcase_construct();
	testcase_traverse();
	testcase_macroscopic();
	testcase_push_pop();
	testcase_insert_erase();
}