#pragma once
#include "iterator.h"
#include "memory_all.h"
#include <initializer_list>


namespace PracticeStl
{
	template <typename T>
	struct __list_node
	{
		using value_type = T;
		using pointer = __list_node<T>*;

		pointer prev;
		pointer next;
		value_type data;
	};

	template <typename T, typename Ref, typename Ptr>
	struct __list_iterator : public iterator<__bidirectional_iterator, T>
	{
		using value_type = T;
		using pointer = Ptr;
		using reference = Ref;

		using iterator_type = __list_iterator<T, T&, T*>;
		using self_type = __list_iterator<T, Ref, Ptr>;
		using node_pointer = __list_node<T>*;

		node_pointer node;

		__list_iterator(node_pointer x) : node(x) {}
		__list_iterator() : node(nullptr) {}
		__list_iterator(const iterator_type& another) : node(another.node) {}
		~__list_iterator() = default;

		bool operator==(const self_type& another) const { return this->node == another.node; }
		bool operator!=(const self_type& another) const { return this->node != another.node; }

		reference operator*() const { return node->data; }
		pointer operator->() const { return &(this->operator*()); }
		self_type& operator++()
		{
			this->node = this->node->next;
			return *this;
		}
		self_type operator++(int)
		{
			self_type tmp = *this;
			++* this;
			return tmp;
		}
		self_type& operator--()
		{
			this->node = (node_pointer)(node->prev);
			return *this;
		}
		self_type operator--(int)
		{
			self_type tmp = *this;
			--* this;
			return tmp;
		}
	};

	template <typename T, typename Alloc = allocator<__list_node<T>>>
	class list
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using iterator = __list_iterator<T, T&, T*>;
		using const_iterator = __list_iterator<T, const T&, const T*>;
		using reference = T&;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
	protected:
		using list_node = __list_node<T>;
		using data_allocator = Alloc;
	public:
		using node_pointer = typename list_node::pointer;

	protected:
		node_pointer node;

	public:
		list()
		{
			// empty
			this->node = this->alloc_node();
			this->node->next = this->node;
			this->node->prev = this->node;
		}
		list(const std::initializer_list<value_type>& vs)
		{
			this->node = this->alloc_node();
			this->node->next = this->node;
			this->node->prev = this->node;
			node_pointer cur = this->node->next;
			for (int i = 0; i < vs.size(); ++i)
			{
				node_pointer tmp = this->alloc_node();
				tmp->next = cur;
				tmp->prev = cur->prev;
				cur->prev->next = tmp;
				cur->prev = tmp;
				cur = cur->next;
			}
			uninitialized_copy(vs.begin(), vs.end(), this->begin());
		}
		~list()
		{
			this->clear();
			this->destroy_node(this->node);
		}

		iterator begin() { return { this->node->next }; }
		iterator end() { return { this->node }; };
		const_iterator cbegin() const { return { this->node->next }; }
		const_iterator cend() const { return { this->node }; }
		reference front() { return *(this->begin()); }
		reference back() { return *(--this->end()); }
		bool empty() const { return this->node->next == this->node; }
		size_type size() const { return size_type(__get_distance(this->cbegin(), this->cend())); }

		void push_back(const_reference x) { this->insert(this->end(), x); }
		void push_front(const_reference x) { this->insert(this->begin(), x); }
		void pop_back() { this->erase(--(this->end())); }
		void pop_front() { this->erase(this->begin()); }

		iterator insert(iterator pos, const_reference x)
		{
			node_pointer tmp = this->create_node(x);
			tmp->next = pos.node;
			tmp->prev = pos.node->prev;
			pos.node->prev->next = tmp;
			pos.node->prev = tmp;
			return tmp;
		}
		iterator erase(iterator pos)
		{
			node_pointer res = pos.node->next;
			pos.node->next->prev = pos.node->prev;
			pos.node->prev->next = pos.node->next;
			this->destroy_node(pos.node);
			return { res };
		}
		iterator erase(iterator first, iterator last)
		{
			node_pointer cur = first.node;
			last.node->prev = first.node->prev;
			first.node->prev->next = last.node;
			while (cur != last.node)
			{
				node_pointer tmp = cur;
				cur = cur->next;
				this->destroy_node(tmp);
			}
			return { last.node->prev };
		}
		void remove(const_reference x)
		{
			iterator first = this->begin();
			iterator last = this->end();
			while (first != last)
			{
				if (first->data == x)
					this->erase(first);
			}
		}
		void clear()
		{
			node_pointer cur = this->node->next;
			while (cur != this->node)
			{
				cur = cur->next;
				this->destroy_node(cur->prev);
			}
			this->node->prev = this->node;
			this->node->next = this->node;
		}

	protected:
		// memory helper
		inline node_pointer alloc_node() { return data_allocator::allocate(1); }
		inline void dealloc_node(node_pointer p) { data_allocator::deallocate(p); }
		inline node_pointer create_node(const_reference x)
		{
			node_pointer p = this->alloc_node();
			construct(&p->data, x);
			return p;
		}
		inline void destroy_node(node_pointer p)
		{
			destroy(&p->data);
			this->dealloc_node(p);
		}
	};
}