#pragma once
#include <cstddef>
#include <new>
#include "type_traits.h"

// all alloc functions in one file
// - construct.h
// - uninitialized.h
// - alloc.h

// costruct related
namespace PracticeStl
{
	template <typename T>
	inline void construct(T* p, const T& value)
	{
		new (p) T(value);
	}

	template <typename T>
	inline void destroy(T* p)
	{
		p->~T();
	}

	template <typename Iter>
	inline void __destroy_impl_if_tri(Iter&, Iter&, __true_type)
	{
		// do nothing
	}

	template <typename Iter>
	inline void __destroy_impl_if_tri(Iter& first, Iter& last, __false_type)
	{
		while (first < last)
		{
			destroy(&*first);
			++first;
		}
	}

	template <typename Iter, typename VT>
	inline void __destroy_impl(Iter& first, Iter& last, VT*)
	{
		using _ = typename __type_traits<VT>::has_trivial_destructor;
		__destroy_impl_if_tri(first, last, _());
	}

	template <typename Iter>
	inline void destroy(Iter first, Iter last)
	{
		__destroy_impl(first, last, __get_value_type(first));
	}
}

// uninitialized related
namespace PracticeStl
{
	template <typename ForwardIter, typename Size, typename T>
	inline ForwardIter __uninitialized_fill_n_impl_if_POD(ForwardIter& first, Size n, const T& x, __true_type)
	{
		// fill_n
		while (n > 0)
		{
			*first = x;
			++first;
			--n;
		}
		return first;
	}

	template <typename ForwardIter, typename Size, typename T>
	inline ForwardIter __uninitialized_fill_n_impl_if_POD(ForwardIter& first, Size n, const T& x, __false_type)
	{
		while (n > 0)
		{
			construct(first, x);
			++first;
			--n;
		}
		return first;
	}

	template <typename ForwardIter, typename Size, typename T, class T_>
	inline ForwardIter __uninitialized_fill_n_impl(ForwardIter& first, Size n, const T& x, T_*)
	{
		using _ = typename __type_traits<T_>::is_POD_type;
		return __uninitialized_fill_n_impl_if_POD(first, n, x, _());
	}

	template <typename ForwardIter, typename Size, typename T>
	inline ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& x)
	{
		return __uninitialized_fill_n_impl(first, n, x, __get_value_type(first));
	}

	template <typename InputIter, typename ForwardIter>
	inline ForwardIter __uninitialized_copy_impl_if_POD(InputIter& from_first, InputIter& from_last, ForwardIter& to_first, __true_type)
	{
		while (from_first != from_last)
		{
			*to_first = *from_first;
			++to_first;
			++from_first;
		}
		return to_first;
	}

	template <typename InputIter, typename ForwardIter>
	inline ForwardIter __uninitialized_copy_impl_if_POD(InputIter& from_first, InputIter& from_last, ForwardIter& to_first, __false_type)
	{
		while (from_first != from_last)
		{
			construct(&*to_first, *from_first);
			++to_first;
			++from_first;
		}
		return to_first;
	}

	template <typename InputIter, typename ForwardIter, typename T>
	inline ForwardIter __uninitialized_copy_impl(InputIter& from_first, InputIter& from_last, ForwardIter& to_first, T*)
	{
		using _ = typename __type_traits<T>::is_POD_type;
		return __uninitialized_copy_impl_if_POD(from_first, from_last, to_first, _());
	}

	template <typename InputIter, typename ForwardIter>
	inline ForwardIter uninitialized_copy(InputIter from_first, InputIter from_last, ForwardIter to_first)
	{
		return __uninitialized_copy_impl(from_first, from_last, to_first, __get_value_type(from_first));
	}

	template <typename ForwardIter, typename T>
	inline void __uninitialized_fill_impl_if_POD(ForwardIter& first, ForwardIter& last, const T& x, __true_type)
	{
		while (first != last)
		{
			*first = x;
			++first;
		}
	}

	template <typename ForwardIter, typename T>
	inline void __uninitialized_fill_impl_if_POD(ForwardIter& first, ForwardIter& last, const T& x, __false_type)
	{
		while (first != last)
		{
			construct(&*first, x);
			++first;
		}
	}

	template <typename ForwardIter, typename T, typename T_>
	inline void __uninitialized_fill_impl(ForwardIter& first, ForwardIter& last, const T& x, T_*)
	{
		using _ = typename __type_traits<T_>::is_POD_type;
		__uninitialized_fill_impl_if_POD(first, last, x, _());
	}


	template <typename ForwardIter, typename T>
	inline void uninitialized_fill(ForwardIter first, ForwardIter last, const T& x)
	{
		__uninitialized_fill_impl(first, last, x, __get_value_type(first));
	}
}

// alloc related
namespace PracticeStl
{
	template <typename T>
	class allocator
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_point = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

	public:
		static pointer allocate(size_type n);
		static void deallocate(pointer ptr);
	};

	template<typename T>
	typename allocator<T>::pointer allocator<T>::allocate(size_type n)
	{
		pointer _res = static_cast<pointer>(operator new ((size_type)(sizeof(value_type) * n)));
		if (_res)
		{
			return _res;
		}
		else
		{
			exit(0x77);
		}
	}

	template<typename T>
	void allocator<T>::deallocate(pointer ptr)
	{
		operator delete(ptr);
	}
}