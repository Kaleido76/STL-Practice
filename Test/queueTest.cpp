#include "queue.h"
#include "queueTest.h"
#include "list.h"
#include "testUtil.h"

using namespace PracticeStl;

static void testcase_construct()
{
	{
		queue<int> test;
		EXPECT(0, test.size());
		EXPECT(true, test.empty());
	}

	{
		queue<int, list<int>> test;
		EXPECT(0, test.size());
		EXPECT(true, test.empty());
	}
}

static void testcase_push_pop()
{
	{
		queue<int> test;
		test.push(1);
		EXPECT(1, test.size());
		EXPECT(1, test.front());
		EXPECT(1, test.back());
		EXPECT(false, test.empty());

		test.push(4);
		EXPECT(2, test.size());
		EXPECT(1, test.front());
		EXPECT(4, test.back());
		EXPECT(false, test.empty());

		test.pop();
		EXPECT(1, test.size());
		EXPECT(4, test.front());
		EXPECT(4, test.back());
		EXPECT(false, test.empty());

		test.pop();
		EXPECT(0, test.size());
		EXPECT(true, test.empty());
	}

	{
		queue<int, list<int>> test;
		test.push(1);
		EXPECT(1, test.size());
		EXPECT(1, test.front());
		EXPECT(1, test.back());
		EXPECT(false, test.empty());

		test.push(4);
		EXPECT(2, test.size());
		EXPECT(1, test.front());
		EXPECT(4, test.back());
		EXPECT(false, test.empty());

		test.pop();
		EXPECT(1, test.size());
		EXPECT(4, test.front());
		EXPECT(4, test.back());
		EXPECT(false, test.empty());

		test.pop();
		EXPECT(0, test.size());
		EXPECT(true, test.empty());
	}
}

void QueueTest::test_all()
{
	testcase_construct();
	testcase_push_pop();
}