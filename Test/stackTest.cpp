#include "stack.h"
#include "stackTest.h"
#include "list.h"
#include "testUtil.h"

using namespace PracticeStl;

static void testcase_construct()
{
	{
		stack<int> test;
		EXPECT(0, test.size());
		EXPECT(true, test.empty());
	}

	{
		stack<int, list<int>> test;
		EXPECT(0, test.size());
		EXPECT(true, test.empty());
	}
}

static void testcase_push_pop()
{
	{
		stack<int> test;
		test.push(1);
		EXPECT(1, test.size());
		EXPECT(1, test.top());
		EXPECT(false, test.empty());

		test.push(4);
		EXPECT(2, test.size());
		EXPECT(4, test.top());
		EXPECT(false, test.empty());

		test.pop();
		EXPECT(1, test.size());
		EXPECT(1, test.top());
		EXPECT(false, test.empty());

		test.pop();
		EXPECT(0, test.size());
		EXPECT(true, test.empty());
	}

	{
		stack<int, list<int>> test;
		test.push(1);
		EXPECT(1, test.size());
		EXPECT(1, test.top());
		EXPECT(false, test.empty());

		test.push(4);
		EXPECT(2, test.size());
		EXPECT(4, test.top());
		EXPECT(false, test.empty());

		test.pop();
		EXPECT(1, test.size());
		EXPECT(1, test.top());
		EXPECT(false, test.empty());

		test.pop();
		EXPECT(0, test.size());
		EXPECT(true, test.empty());
	}
}

void StackTest::test_all()
{
	testcase_construct();
	testcase_push_pop();
}