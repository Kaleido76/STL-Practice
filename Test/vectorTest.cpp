#include "vector.h"
#include "vectorTest.h"
#include <cassert>

using namespace PracticeStl;

static void testcase_construct()
{
	{
		vector<int> test;
		assert(test.size() == 0);
		assert(test.capacity() == 0);
		assert(test.begin() == nullptr);
		assert(test.end() == nullptr);
	}

	{
		vector<int> test(5, -10);
		assert(test.size() == 5);
		assert(test.capacity() == 5);
		assert(test.begin() != nullptr);
		assert(test.end() != nullptr);
	}

	{
		vector<int> test = { 1, 3, 5 };
		assert(test.size() == 3);
		assert(test.capacity() == 3);
		assert(test.begin() != nullptr);
		assert(test.end() != nullptr);
		assert(test[0] == 1);
		assert(test[1] == 3);
		assert(test[2] == 5);
	}

}

static void testcase_push_pop()
{
	vector<int> test;
	test.push_back(1);
	assert(test.size() == 1);
	assert(test.capacity() == 1);
	assert(test.front() == 1);
	assert(test.back() == 1);

	test.push_back(5);
	assert(test.size() == 2);
	assert(test.capacity() == 2);
	assert(test.front() == 1);
	assert(test.back() == 5);

	test.push_back(-8);
	assert(test.size() == 3);
	assert(test.capacity() == 4);
	assert(test.front() == 1);
	assert(test.back() == -8);

	test.push_back(99);
	assert(test.size() == 4);
	assert(test.capacity() == 4);
	assert(test.front() == 1);
	assert(test.back() == 99);

	test.push_back(789);
	assert(test.size() == 5);
	assert(test.capacity() == 8);
	assert(test.front() == 1);
	assert(test.back() == 789);

	test.pop_back();
	assert(test.size() == 4);
	assert(test.capacity() == 8);
	assert(test.front() == 1);
	assert(test.back() == 99);

	test.pop_back();
	test.pop_back();
	test.pop_back();
	test.pop_back();
	assert(test.size() == 0);
	assert(test.capacity() == 8);
}

static void testcase_macroscopic()
{
	vector<int> test(5, 99);
	test.resize(8, 10);
	assert(test.size() == 8);
	assert(test.capacity() == 8);
	assert(test[4] == 99);
	assert(test[5] == 10);
	assert(test[7] == 10);

	test.reserve(5);
	assert(test.size() == 8);
	assert(test.capacity() == 8);

	test.resize(5, 0);
	assert(test.size() == 5);
	assert(test.capacity() == 8);

	test.reserve(20);
	assert(test.size() == 5);
	assert(test.capacity() == 20);


	assert(test.empty() == false);
	test.clear();
	assert(test.empty() == true);
	assert(test.size() == 0);
	assert(test.capacity() == 20);
}

static void testcase_insert_erase()
{
	vector<int> test;
	test.push_back(1);
	test.push_back(5);
	test.insert(test.begin(), 1616);
	assert(test.size() == 3);
	assert(test.capacity() == 4);
	assert(test[0] == 1616);
	assert(test[1] == 1);
	assert(test[2] == 5);

	test.insert(test.end(), 111);
	assert(test.size() == 4);
	assert(test.capacity() == 4);
	assert(test[0] == 1616);
	assert(test[1] == 1);
	assert(test[2] == 5);
	assert(test[3] == 111);

	test.insert(test.end(), 123);
	assert(test.size() == 5);
	assert(test.capacity() == 8);
	assert(test[3] == 111);
	assert(test[4] == 123);

	test.erase(test.begin());
	assert(test.size() == 4);
	assert(test.capacity() == 8);
	assert(test[0] == 1);
	assert(test[1] == 5);
	assert(test[2] == 111);
	assert(test[3] == 123);

	test.erase(test.end() - 1);
	assert(test.size() == 3);
	assert(test.capacity() == 8);
	assert(test[0] == 1);
	assert(test[1] == 5);
	assert(test[2] == 111);

	test.erase(test.begin() + 1, test.end() - 1);
	assert(test.size() == 2);
	assert(test.capacity() == 8);
	assert(test[0] == 1);
	assert(test[1] == 111);
}

static void testcase_loop()
{
	{
		vector<int> test(5, -10);
		for (auto& item : test)
		{
			assert(item == -10);
		}

		decltype(test)::iterator iter;
		for (iter = test.begin(); iter != test.end(); ++iter)
		{
			assert(*iter == -10);
		}

		decltype(test)::size_type idx;
		for (idx = 0; idx != test.size(); ++idx)
		{
			assert(test[idx] == -10);
		}
	}
}

void TestUnits::VectorTest::test_all()
{
	testcase_construct();
	testcase_push_pop();
	testcase_insert_erase();
	testcase_loop();
}