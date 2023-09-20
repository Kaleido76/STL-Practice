#include "rb_tree.h"
#include "rbtreeTest.h"
#include "testUtil.h"
#include <functional>

using namespace PracticeStl;

template<typename T>
struct Itself
{
	T operator()(const T& x) { return x; }
};

template<typename T>
struct LessPred
{
	bool operator()(const T& x, const T& y) { return x < y; }
};

template<typename T>
static int compare(const std::initializer_list<T>& vs, rb_tree<T, T, Itself<T>, LessPred<T>>& con)
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
		rb_tree<int, int, Itself<int>, LessPred<int>> test;
		EXPECT(true, test.empty());
	}
}

static void testcase_traverse()
{
	rb_tree<int, int, Itself<int>, LessPred<int>> test;
	for (int i = 0; i < 10; ++i)
		test.insert_unique(i);
	EXPECT(0, compare({ 0,1,2,3,4,5,6,7,8,9 }, test));
}


static void testcase_insert_erase()
{
	rb_tree<int, int, Itself<int>, LessPred<int>> test;
	EXPECT(true, test.insert_unique(10));
	EXPECT(true, test.insert_unique(30));
	for (int i = 0; i < 10; ++i)
		test.insert_unique(i);
	EXPECT(12, test.size());
	EXPECT(0, compare({ 0,1,2,3,4,5,6,7,8,9,10,30 }, test));
	EXPECT(true, test.erase(test.begin()));
	EXPECT(0, compare({ 1,2,3,4,5,6,7,8,9,10,30 }, test));
	EXPECT(true, test.erase(test.begin()));
	EXPECT(0, compare({ 2,3,4,5,6,7,8,9,10,30 }, test));
	EXPECT(true, test.erase(test.begin()));
	EXPECT(0, compare({ 3,4,5,6,7,8,9,10,30 }, test));
	EXPECT(true, test.erase(++(test.begin())));
	EXPECT(0, compare({ 3,5,6,7,8,9,10,30 }, test));

	EXPECT(true, test.erase(--(test.end())));
	EXPECT(0, compare({ 3,5,6,7,8,9,10 }, test));
	test.clear();
	EXPECT(0, test.size());
}

void RBTreeTest::test_all()
{
	testcase_construct();
	testcase_traverse();
	testcase_insert_erase();
}