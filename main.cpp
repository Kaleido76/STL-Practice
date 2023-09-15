#include <iostream>
#include "vectorTest.h"
#include "listTest.h"
#include "dequeTest.h"
#include "stackTest.h"
#include "queueTest.h"
#include "testUtil.h"

TEST_GLOBAL_INIT;

int main()
{
	VectorTest::test_all();
	ListTest::test_all();
	DequeTest::test_all();
	StackTest::test_all();
	QueueTest::test_all();
	return 0;
}