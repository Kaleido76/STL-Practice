#include <iostream>
#include "vectorTest.h"
#include "listTest.h"
#include "dequeTest.h"


int main()
{
	using namespace TestUnits;
	VectorTest::test_all();
	ListTest::test_all();
	DequeTest::test_all();
	return 0;
}