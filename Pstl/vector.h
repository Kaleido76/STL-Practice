#pragma once
#include "iterator.h"
#include "memory_all.h"
#include <initializer_list>

namespace PracticeStl
{
	template <typename T, typename Alloc = allocator<T>>
	class vector
	{
	public:
		using value_type = T;
		using pointer = T*;
		using iterator = T*;
		using const_iterator = const T*;
		using reference = T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

	protected:
		using data_allocator = Alloc;

		iterator _first;
		iterator _tail;
		iterator _mem_tail;

	public:
		vector() : _first(nullptr), _tail(nullptr), _mem_tail(nullptr) {}
		vector(const std::initializer_list<value_type>& vs)
		{
			this->_first = this->alloc(vs.size());
			this->_mem_tail = this->_tail = this->_first + vs.size();
			uninitialized_copy(vs.begin(), vs.end(), this->_first);
		}
		vector(size_type n, const value_type& x)
		{
			this->initialize_by_fill(n, x);
		}
		~vector()
		{
			destroy(this->_first, this->_tail);
			this->dealloc();
		}

		iterator begin() { return this->_first; }
		iterator end() { return this->_tail; }
		const_iterator cbegin() { return this->_first; }
		const_iterator cend() { return this->_tail; }
		size_type size() const { return size_type(this->_tail - this->_first); }
		size_type capacity() const { return size_type(this->_mem_tail - this->_first); }
		bool empty() const { return this->_tail == this->_first; }
		reference front() { return *(this->_first); }
		reference back() { return *(this->_tail - 1); }
		reference operator[] (size_type n) { return *(this->_first + n); }

		void push_back(const value_type& x) { this->insert(this->_tail, x); }
		void pop_back() { this->erase(this->_tail - 1); }
		void clear() { this->erase(this->_first, this->_tail); }
		void resize(size_type n, const value_type& x)
		{
			if (this->_first + n <= this->_tail)
			{
				this->erase(this->_first + n, this->_tail);
			}
			else
			{
				this->extend_mem(n);
				uninitialized_fill(this->_tail, this->_mem_tail, x);
				this->_tail = this->_mem_tail;
			}
		}
		void reserve(size_type n)
		{
			if (this->_first + n > this->_mem_tail)
				this->extend_mem(n);
		}

		iterator insert(iterator pos, const value_type& x)
		{
			difference_type d = pos - this->_first;
			if (this->_mem_tail == this->_tail)
				this->extend_mem();

			pos = this->_tail;
			while (pos - this->_first != d)
			{
				construct(pos, *(pos - 1));
				destroy(pos - 1);
				--pos;
			}
			++this->_tail;

			construct(pos, x);
			return pos;
		}
		iterator erase(iterator pos)
		{
			destroy(pos);
			iterator cur = pos + 1;
			while (cur != this->_tail)
			{
				construct(cur - 1, *cur);
				destroy(cur);
				++cur;
			}
			--(this->_tail);
			return pos;
		}
		iterator erase(iterator first, iterator last)
		{
			iterator cur = first;
			difference_type dis = last - first;
			while (cur != last)
			{
				destroy(cur);
				++cur;
			}

			while (cur != this->_tail)
			{
				construct(cur - dis, *cur);
				destroy(cur);
				++cur;
			}
			this->_tail -= dis;
			return first;
		}

	protected:
		inline void dealloc() { data_allocator::deallocate(this->_first); }
		inline iterator alloc(size_type n) { return data_allocator::allocate(n); }
		inline void initialize_by_fill(size_type n, const value_type& x)
		{
			this->_first = this->alloc(n);
			uninitialized_fill_n(this->_first, n, x);
			this->_tail = this->_first + n;
			this->_mem_tail = this->_tail;
		}
		inline void extend_mem()
		{
			this->extend_mem((this->_mem_tail == 0) ? 1 : ((this->_mem_tail - this->_first) * 2));
		}
		inline void extend_mem(size_type new_size)
		{
			iterator new_first = data_allocator::allocate(new_size);
			uninitialized_copy(this->_first, this->_tail, new_first);
			destroy(this->_first, this->_tail);
			data_allocator::deallocate(this->_first);
			this->_tail = this->_tail - this->_first + new_first;
			this->_first = new_first;
			this->_mem_tail = this->_first + new_size;
		}
	};
}