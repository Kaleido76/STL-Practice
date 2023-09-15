#include "vector.h"
#include "vectorTest.h"
#include "testUtil.h"

using namespace PracticeStl;

template<typename T>
static int compare(const std::initializer_list<T>& vs, vector<T>& con)
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
		vector<int> test;
		EXPECT(0, test.size());
		EXPECT(0, test.capacity());
		EXPECT(true, test.empty());
	}

	{
		vector<int> test(5, -10);
		EXPECT(5, test.size());
		EXPECT(5, test.capacity());
		EXPECT(false, test.empty());
	}

	{
		vector<int> test = { 1, 3, 5 };
		EXPECT(3, test.size());
		EXPECT(3, test.capacity());
		EXPECT(false, test.empty());
		EXPECT(1, test[0]);
		EXPECT(3, test[1]);
		EXPECT(5, test[2]);
	}

}

static void testcase_push_pop()
{
	vector<int> test;
	test.push_back(1);
	EXPECT(1, test.capacity());
	EXPECT(0, compare({ 1 }, test));
	EXPECT(1, test.front());
	EXPECT(1, test.back());


	test.push_back(5);
	EXPECT(2, test.capacity());
	EXPECT(0, compare({ 1, 5 }, test));
	EXPECT(1, test.front());
	EXPECT(5, test.back());

	test.push_back(-8);
	EXPECT(4, test.capacity());
	EXPECT(0, compare({ 1, 5, -8 }, test));

	test.push_back(99);
	EXPECT(4, test.capacity());
	EXPECT(0, compare({ 1, 5, -8, 99 }, test));

	test.push_back(789);
	EXPECT(8, test.capacity());
	EXPECT(0, compare({ 1, 5, -8, 99, 789 }, test));

	test.pop_back();
	EXPECT(8, test.capacity());
	EXPECT(0, compare({ 1, 5, -8, 99 }, test));

	test.pop_back();
	test.pop_back();
	test.pop_back();
	test.pop_back();
	EXPECT(0, test.size());
	EXPECT(8, test.capacity());
	EXPECT(true, test.empty());
}

static void testcase_macroscopic()
{
	vector<int> test(5, 99);
	test.resize(8, 10);
	EXPECT(8, test.size());
	EXPECT(8, test.capacity());
	EXPECT(0, compare({ 99,99,99,99,99,10,10,10 }, test));

	test.reserve(5);
	EXPECT(8, test.size());
	EXPECT(8, test.capacity());
	EXPECT(0, compare({ 99,99,99,99,99,10,10,10 }, test));

	test.resize(5, 0);
	EXPECT(5, test.size());
	EXPECT(8, test.capacity());
	EXPECT(0, compare({ 99,99,99,99,99 }, test));

	test.reserve(20);
	EXPECT(5, test.size());
	EXPECT(20, test.capacity());
	EXPECT(0, compare({ 99,99,99,99,99 }, test));
	EXPECT(false, test.empty());

	test.clear();
	EXPECT(true, test.empty());
	EXPECT(0, test.size());
	EXPECT(20, test.capacity());
}

static void testcase_insert_erase()
{
	vector<int> test;
	test.push_back(1);
	test.push_back(5);
	test.insert(test.begin(), 1616);
	EXPECT(3, test.size());
	EXPECT(4, test.capacity());
	EXPECT(0, compare({ 1616, 1, 5 }, test));

	test.insert(test.end(), 111);
	EXPECT(4, test.size());
	EXPECT(4, test.capacity());
	EXPECT(0, compare({ 1616, 1, 5 , 111 }, test));

	test.insert(test.end(), 123);
	EXPECT(5, test.size());
	EXPECT(8, test.capacity());
	EXPECT(0, compare({ 1616, 1, 5 , 111, 123 }, test));

	test.erase(test.begin());
	EXPECT(4, test.size());
	EXPECT(8, test.capacity());
	EXPECT(0, compare({ 1, 5 , 111, 123 }, test));

	test.erase(test.end() - 1);
	EXPECT(3, test.size());
	EXPECT(8, test.capacity());
	EXPECT(0, compare({ 1, 5 , 111 }, test));

	test.erase(test.begin() + 1, test.end() - 1);
	EXPECT(2, test.size());
	EXPECT(8, test.capacity());
	EXPECT(0, compare({ 1 , 111 }, test));
}

static void testcase_loop()
{
	{
		vector<int> test(1, -10);
		for (auto& item : test)
		{
			EXPECT(-10, item);
		}

		decltype(test)::iterator iter;
		for (iter = test.begin(); iter != test.end(); ++iter)
		{
			EXPECT(-10, *iter);
		}

		decltype(test)::size_type idx;
		for (idx = 0; idx != test.size(); ++idx)
		{
			EXPECT(-10, test[idx]);
		}
	}
}

void VectorTest::test_all()
{
	testcase_construct();
	testcase_push_pop();
	testcase_insert_erase();
	testcase_loop();
}