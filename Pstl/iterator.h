#pragma once
#include <cstddef>

namespace PracticeStl
{
	struct __input_iterator {};
	struct __output_iterator {};
	struct __forward_iterator : public __input_iterator {};
	struct __bidirectional_iterator : public __forward_iterator {};
	struct __random_access_iterator : public __bidirectional_iterator {};

	template <typename Iterator>
	struct __iterator_traits
	{
		using iterator_category = typename Iterator::iterator_category;
		using value_type = typename Iterator::value_type;
		using difference_type = typename Iterator::difference_type;
		using pointer = typename Iterator::pointer;
		using reference = typename Iterator::reference;
	};

	template<typename T>
	struct __iterator_traits<T*>
	{
		using iterator_category = __random_access_iterator;
		using value_type = T;
		using difference_type = ptrdiff_t;
		using pointer = T*;
		using reference = T&;
	};

	template<typename T>
	struct __iterator_traits<const T*>
	{
		using iterator_category = __random_access_iterator;
		using value_type = T;
		using difference_type = ptrdiff_t;
		using pointer = T*;
		using reference = T&;
	};

	template <
		typename Category,
		typename T,
		typename Distance = ptrdiff_t,
		typename Pointer = T*,
		typename Reference = T&
	>
	struct iterator
	{
		using iterator_category = Category;
		using value_type = T;
		using difference_type = Distance;
		using pointer = Pointer;
		using reference = Reference;
	};

	template<typename Iter>
	inline typename __iterator_traits<Iter>::iterator_category
		__get_iterator_category(const Iter& i)
	{
		using _ = typename __iterator_traits<Iter>::iterator_category;
		return _();
	}

	template<typename Iter>
	inline typename __iterator_traits<Iter>::value_type*
		__get_value_type(const Iter& i)
	{
		using _ = typename __iterator_traits<Iter>::value_type;
		return static_cast<_*>(0);
	}

	template<typename Iter>
	inline typename __iterator_traits<Iter>::difference_type*
		__get_difference_type(const Iter& i)
	{
		using _ = typename __iterator_traits<Iter>::difference_type;
		return static_cast<_*>(0);
	}

	template<typename Iter>
	inline typename __iterator_traits<Iter>::difference_type
		__get_distance_impl(Iter& start, Iter& end, __input_iterator)
	{
		using _diff_type = typename __iterator_traits<Iter>::difference_type;
		_diff_type diff = 0;
		while (start != end)
		{
			++start;
			++diff;
		}
		return diff;
	}

	template<typename Iter>
	inline typename __iterator_traits<Iter>::difference_type
		__get_distance_impl(Iter& start, Iter& end, __random_access_iterator)
	{
		return end - start;
	}

	template<typename Iter>
	inline typename __iterator_traits<Iter>::difference_type
		__get_distance(Iter start, Iter end)
	{
		using _category = typename __iterator_traits<Iter>::iterator_category;
		return __get_distance_impl(start, end, _category());
	}

	template<typename Iter, typename Distance>
	inline void __advance_impl(Iter& cur, Distance n, __input_iterator)
	{
		while (n--) ++cur;
	}

	template<typename Iter, typename Distance>
	inline void __advance_impl(Iter& cur, Distance n, __bidirectional_iterator)
	{
		if (n > 0)
			while (n--) ++cur;
		else
			while (n++) --cur;
	}

	template<typename Iter, typename Distance>
	inline void __advance_impl(Iter& cur, Distance n, __random_access_iterator)
	{
		cur += n;
	}

	template<typename Iter, typename Distance>
	inline void __advance(Iter& cur, Distance n)
	{
		using _category = typename __iterator_traits<Iter>::iterator_category;
		__advance_impl(cur, n, _category());
	}
}