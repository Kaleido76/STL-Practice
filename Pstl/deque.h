#pragma once
#include "iterator.h"
#include "memory_all.h"
#include <initializer_list>
#include <utility>
#include <algorithm>

namespace PracticeStl
{
	inline size_t __deque_buf_size(size_t n, size_t sz)
	{
		return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
	}

	template <typename T, typename Ref, typename Ptr, size_t Bufsize>
	struct __deque_iterator : public iterator<__random_access_iterator, T>
	{
		using value_type = T;
		using difference_type = ptrdiff_t;
		using pointer = Ptr;
		using reference = Ref;

		using iterator_type = __deque_iterator<T, T&, T*, Bufsize>;
		using self_type = __deque_iterator<T, Ref, Ptr, Bufsize>;
		using map_pointer = value_type**;

		pointer cur;
		pointer first;
		pointer last;
		map_pointer map_slot;
		static const size_t node_size{ Bufsize != 0 ? Bufsize : (sizeof(T) < 512 ? size_t(512 / sizeof(T)) : size_t(1)) };

		__deque_iterator()
			: cur(nullptr), first(nullptr), last(nullptr), map_slot(nullptr)
		{}

		__deque_iterator(const iterator_type& another)
			: cur(another.cur), first(another.first), last(another.last),
			map_slot(another.map_slot)
		{}

		// functional
		void set_map_slot(map_pointer x)
		{
			this->map_slot = x;
			this->first = *x;
			this->last = this->first + difference_type(this->node_size);
		}

		reference operator*() const { return *cur; };
		pointer operator->() const { return &(this->operator*()); }
		difference_type operator-(const self_type& another) const
		{
			return difference_type(this->node_size)
				* (this->map_slot - another.map_slot - 1)
				+ (this->cur - this->first) + (another.last - another.cur);
		}
		self_type& operator++()
		{
			++this->cur;
			if (this->cur == this->last)
			{
				this->set_map_slot(this->map_slot + 1);
				this->cur = this->first;
			}
			return *this;
		}
		self_type operator++(int)
		{
			self_type tmp = *this;
			++(*this);
			return tmp;
		}
		self_type& operator--()
		{
			if (this->cur == this->first)
			{
				this->set_map_slot(this->map_slot - 1);
				this->cur = this->last;
			}
			--this->cur;
			return *this;
		}
		self_type operator--(int)
		{
			self_type tmp = *this;
			--(*this);
			return tmp;
		}
		self_type& operator+=(difference_type n)
		{
			difference_type ofst = n + this->cur - this->first;
			if (ofst >= 0 && ofst < difference_type(this->node_size))
			{
				this->cur += n;
			}
			else
			{
				difference_type slot_ofst = (ofst > 0)
					? ofst / difference_type(this->node_size)
					: -difference_type((-ofst - 1) / this->node_size) - 1;
				this->set_map_slot(this->map_slot + slot_ofst);
				this->cur = this->first + (ofst - slot_ofst * difference_type(this->node_size));
			}
			return *this;
		}
		self_type operator+(difference_type n) const
		{
			self_type tmp = *this;
			return tmp += n;
		}
		self_type& operator-=(difference_type n) { return *this += -n; }
		self_type operator-(difference_type n) const
		{
			self_type tmp = *this;
			return tmp -= n;
		}
		reference operator[](difference_type n) const { return *(*this + n); }
		bool operator==(const self_type& another) const { return this->cur == another.cur; }
		bool operator!=(const self_type& another) const { return !(*this == another); }
		bool operator<(const self_type& another) const
		{
			return (this->map_slot == another.map_slot)
				? (this->cur < another.cur)
				: (this->map_slot < another.map_slot);
		}
	};

	template <typename T, typename Alloc = allocator<T>, size_t Bufsize = 0>
	class deque
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using iterator = __deque_iterator<T, T&, T*, Bufsize>;
		using const_iterator = __deque_iterator<T, const T&, const T*, Bufsize>;
		using reference = T&;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
	protected:
		using map_pointer = pointer*;
		using data_allocator = Alloc;
		using map_allocator = allocator<pointer>;

	protected:
		iterator start;
		iterator finish;
		map_pointer map;
		size_type map_size;
		static const size_t node_size{ Bufsize != 0 ? Bufsize : (sizeof(T) < 512 ? size_t(512 / sizeof(T)) : size_t(1)) };

	public:
		deque() : start(), finish(), map(nullptr), map_size(0) {}
		deque(int n, const_reference v)
		{
			this->init_map_and_nodes(n);
			map_pointer cur;
			for (cur = this->start.map_slot; cur < this->finish.map_slot; ++cur)
			{
				uninitialized_fill(*cur, *cur + this->node_size, v);
			}
			uninitialized_fill(this->finish.first, this->finish.cur, v);
		}
		deque(const std::initializer_list<value_type>& vs)
		{
			this->init_map_and_nodes(vs.size());
			uninitialized_copy(vs.begin(), vs.end(), this->start);
		}

		iterator begin() { return this->start; }
		iterator end() { return this->finish; }
		reference operator[](size_type n) { return this->start[difference_type(n)]; }
		reference front() { return *this->start; }
		reference back() { return *(this->finish - 1); }
		size_type size() const { return  (this->map) ? (this->finish - this->start) : 0; }
		bool empty() const { return this->finish == this->start; }

		void push_back(const_reference x) { this->insert(this->finish, x); }
		void push_front(const_reference x) { this->insert(this->start, x); }
		void pop_back() { this->erase(--(this->finish)); }
		void pop_front() { this->erase(this->start); }
		void clear() { this->erase(this->start, this->finish); }

		iterator insert(iterator pos, const_reference x)
		{
			if (pos.cur == this->start.cur)
			{
				if (this->start.cur != this->start.first)
				{
					construct(this->start.cur - 1, x);
					--(this->start.cur);
				}
				else
				{
					// no mem
					this->extend_map_front();
					*(this->start.map_slot - 1) = this->alloc_node();
					this->start.set_map_slot(this->start.map_slot - 1);
					this->start.cur = this->start.last - 1;
					construct(this->start.cur, x);
				}
				return this->start;
			}
			else if (pos.cur == this->finish.cur)
			{
				if (this->finish.cur != this->finish.last)
				{
					construct(this->finish.cur, x);
					++(this->finish.cur);
				}
				else
				{
					this->extend_map_back();
					*(this->finish.map_slot + 1) = this->alloc_node();
					construct(this->finish.cur, x);
					this->finish.set_map_slot(this->finish.map_slot + 1);
					this->finish.cur = this->finish.first;
				}
				return this->finish - 1;
			}
			else
			{
				size_type index = size_type(pos - this->start);
				if (index < this->size() / 2)
				{
					this->push_front(this->front());
					iterator front1 = this->start + 1;
					iterator front2 = front1 + 1;
					pos = this->start + index;
					iterator pos1 = pos + 1;
					std::copy(front2, pos1, front1);
				}
				else
				{
					this->push_back(this->back());
					iterator back1 = this->finish - 1;
					iterator back2 = back1 - 1;
					pos = this->start + index;
					std::copy_backward(pos, back2, back1);
				}
				*pos = x;
				return pos;
			}
		}
		iterator erase(iterator pos)
		{
			iterator next = pos + 1;
			size_type index = size_type(pos - this->start);
			if (index < (this->size() / 2))
			{
				std::copy_backward(this->start, pos, next);
				this->pop_front();
			}
			else
			{
				std::copy(next, this->finish, pos);
				this->pop_back();
			}
			return this->start + index;
		}
		iterator erase(iterator first, iterator last)
		{

		}


	protected:
		// memory helper
		pointer alloc_node() { return data_allocator::allocate(this->node_size); }
		void dealloc_node(pointer p) { data_allocator::deallocate(p); }
		map_pointer alloc_map(size_type n) { return map_allocator::allocate(n); }
		void dealloc_map(map_pointer p) { map_allocator::deallocate(p); }
		void init_map_and_nodes(size_type e_n)
		{
			size_type node_n = e_n / this->node_size + 1;
			this->map_size = std::max(size_type(8), node_n + 2);
			this->map = this->alloc_map(this->map_size);

			// in use
			map_pointer nstart = this->map + (this->map_size - node_n) / 2;
			map_pointer nfinish = nstart + node_n - 1;
			map_pointer cur;
			for (cur = nstart; cur <= nfinish; ++cur)
			{
				*cur = this->alloc_node();
			}
			this->start.set_map_slot(nstart);
			this->start.cur = this->start.first;
			this->finish.set_map_slot(nfinish);
			this->finish.cur = this->finish.first + e_n % this->node_size;
		}
		void extend_map_front()
		{
			if (this->start.map_slot == this->map)
				this->realloc_map(true);
		}
		void extend_map_back()
		{
			if (this->finish.map_slot - this->map + 1 == this->map_size)
				this->realloc_map(false);
		}
		void realloc_map(bool at_front)
		{
			size_type old_node_n = this->finish.map_slot - this->start.map_slot + 1;
			size_type new_node_n = old_node_n + 1;
			map_pointer new_nstart;
			if (this->map_size > 2 * new_node_n)
			{
				new_nstart = this->map + (this->map_size - new_node_n) / 2 + (at_front ? 1 : 0);
				if (new_nstart < this->start.map_slot)
					std::copy(this->start.map_slot, this->finish.map_slot + 1, new_nstart);
				else
					std::copy_backward(this->start.map_slot, this->finish.map_slot + 1, new_nstart + old_node_n);
			}
			else
			{
				size_type new_map_size = this->map_size * 2 + 2;
				map_pointer new_map = this->alloc_map(new_map_size);
				new_nstart = new_map + (new_map_size - new_node_n) / 2 + (at_front ? 1 : 0);
				if (this->map)
				{
					std::copy(this->start.map_slot, this->finish.map_slot + 1, new_nstart);
					this->dealloc_map(this->map);
				}
				this->map = new_map;
				this->map_size = new_map_size;
			}
			this->start.set_map_slot(new_nstart);
			this->finish.set_map_slot(new_nstart + old_node_n - 1);
		}
	};
}