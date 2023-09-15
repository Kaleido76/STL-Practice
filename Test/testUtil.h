#pragma once
#define SHOW_PASSED false
#define SHOW_STR_CONTENT false
#define COLUMN_1_WIDTH 30
#define COLUMN_2_WIDTH 30

#include <initializer_list>
#include <iostream>
#include <iomanip>

#define TEST_GLOBAL_INIT							\
	__Tester* __Tester::pinstance = new __Tester;	\
	int __Tester::test_count = 0;					\
	int __Tester::test_pass = 0;					\
	__Tester::__MemoryManager __Tester::__

#define INSTANCE (__Tester::get_instance())

#define __STDERR std::cerr
#define __ENDL std::endl

#define __EXPECT_BASE(equality, expect, actual)                                         \
    do                                                                                  \
    {                                                                                   \
        if (equality)                                                                   \
        {                                                                               \
			INSTANCE.count(true);                                                       \
            if (SHOW_PASSED)                                                            \
            {                                                                           \
                __STDERR << std::setw(COLUMN_1_WIDTH) << expect                         \
                     << "\033[32m" << std::setw(COLUMN_2_WIDTH) << actual << "\033[39m" \
                     << " in   " << __FILE__ << ":" << __LINE__ << __ENDL;              \
            }                                                                           \
        }                                                                               \
        else                                                                            \
        {                                                                               \
			INSTANCE.count(false);                                                      \
            __STDERR << std::setw(COLUMN_1_WIDTH) << expect                             \
                 << "\033[31m" << std::setw(COLUMN_2_WIDTH) << actual << "\033[39m"     \
                 << " in   " << __FILE__ << ":" << __LINE__ << __ENDL;                  \
        }                                                                               \
    } while (0)

#define EXPECT(expect, actual) __EXPECT_BASE((expect) == (actual), expect, actual)


class __Tester
{
	static int test_count;
	static int test_pass;
	static __Tester* pinstance;

	__Tester() { head(); }
	~__Tester() { statistics(); }

	struct __MemoryManager
	{
		~__MemoryManager() { delete __Tester::pinstance; }
	};
	static __MemoryManager __;

	__Tester(const __Tester& another) = delete;
	__Tester(__Tester&& another) = delete;
	__Tester& operator=(const __Tester& another) = delete;
	__Tester& operator=(__Tester&& another) = delete;

	void head()
	{
		std::cerr << std::left
			<< "\033[34m"
			<< std::setw(COLUMN_1_WIDTH) << "Expect"
			<< std::setw(COLUMN_2_WIDTH) << "Actual"
			<< "\033[39m" << std::endl;
	}
	void statistics()
	{
		double pass_rate;
		pass_rate = (this->test_pass == this->test_count) ? 100. : (this->test_pass * 100.0 / this->test_count);
		std::cerr << "----------------" << std::endl
			<< "Total:  " << this->test_pass << "/" << this->test_count << "  ("
			<< std::fixed << std::setprecision(2) << pass_rate << "%)"
			<< std::endl;
	}
public:
	static inline __Tester& get_instance() { return *pinstance; }
	static inline void count(bool pass)
	{
		++test_count;
		test_pass += (pass ? 1 : 0);
	}
};
