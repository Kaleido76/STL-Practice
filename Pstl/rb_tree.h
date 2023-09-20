#pragma once
#include "memory_all.h"
#include "iterator.h"

namespace PracticeStl
{
	enum __rb_tree_color_type : bool
	{
		__rb_tree_red = false,
		__rb_tree_black = true
	};

	struct __rb_tree_node_base
	{
		using color_type = __rb_tree_color_type;
		using base_ptr = __rb_tree_node_base*;

		color_type color;
		base_ptr parent;
		base_ptr left;
		base_ptr right;

		static base_ptr minimum(base_ptr x)
		{
			while (x->left != nullptr) x = x->left;
			return x;
		}

		static base_ptr maximum(base_ptr x)
		{
			while (x->right != nullptr) x = x->right;
			return x;
		}

	};

	template<typename Value>
	struct __rb_tree_node : public __rb_tree_node_base
	{
		using node_pointer = __rb_tree_node<Value>*;
		Value value_field;
	};


	struct __rb_tree_base_iterator
	{
		using base_ptr = __rb_tree_node_base::base_ptr;
		using iterator_category = __bidirectional_iterator;
		using difference_type = ptrdiff_t;

		base_ptr cur_node;
		void increment()
		{
			if (cur_node->right != nullptr)
			{
				cur_node = cur_node->right;
				while (cur_node->left != nullptr) cur_node = cur_node->left;
			}
			else
			{
				base_ptr prt = cur_node->parent;
				while (cur_node == prt->right)
				{
					cur_node = prt;
					prt = prt->parent;
				}
				if (cur_node->right != prt)
					cur_node = prt;
			}
		}
		void decrement()
		{
			if (cur_node->color == __rb_tree_red && cur_node->parent->parent == cur_node) // cur_node == header
			{
				cur_node = cur_node->right;
			}
			else if (cur_node->left != nullptr)
			{
				base_ptr lower = cur_node->left;
				while (lower->right != nullptr) lower = lower->right;
				cur_node = lower;
			}
			else
			{
				base_ptr prt = cur_node->parent;
				while (cur_node == prt->left)
				{
					cur_node = prt;
					prt = prt->parent;
				}
				cur_node = prt;
			}
		}
	};

	template <typename T, typename Ref, typename Ptr>
	struct __rb_tree_iterator : public __rb_tree_base_iterator
	{
		using value_type = T;
		using reference = Ref;
		using pointer = Ptr;
		using iterator = __rb_tree_iterator<T, T&, T*>;
		using const_iterator = __rb_tree_iterator<T, const T&, const T*>;
		using self_type = __rb_tree_iterator < T, Ref, Ptr>;
		using node_pointer = __rb_tree_node<T>*;

		__rb_tree_iterator() {}
		__rb_tree_iterator(__rb_tree_node_base* x) { cur_node = node_pointer(x); }
		__rb_tree_iterator(node_pointer x) { cur_node = x; }
		__rb_tree_iterator(const iterator& another) { cur_node = another.cur_node; }

		bool operator==(const iterator& another) const { return cur_node == another.cur_node; }
		bool operator!=(const iterator& another) const { return cur_node != another.cur_node; }

		reference operator*() const { return node_pointer(cur_node)->value_field; }
		pointer operator->() const { return &(operator*()); }

		self_type& operator++() { increment(); return *this; }
		self_type operator++(int)
		{
			self_type tmp = *this;
			increment();
			return tmp;
		}

		self_type& operator--() { decrement(); return *this; }
		self_type operator--(int)
		{
			self_type tmp = *this;
			decrement();
			return tmp;
		}

	};

	inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root)
	{
		__rb_tree_node_base* tmp = x->right;
		x->right = tmp->left;
		if (tmp->left != nullptr)
		{
			tmp->left->parent = x;
		}
		tmp->parent = x->parent;

		if (x == root)
		{
			root = tmp;
		}
		else if (x == x->parent->left)
		{
			x->parent->left = tmp;
		}
		else
		{
			x->parent->right = tmp;
		}
		tmp->left = x;
		x->parent = tmp;
	}

	inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root)
	{
		__rb_tree_node_base* tmp = x->left;
		x->left = tmp->right;
		if (tmp->right != nullptr)
		{
			tmp->right->parent = x;
		}
		tmp->parent = x->parent;

		if (x == root)
		{
			root = tmp;
		}
		else if (x == x->parent->right)
		{
			x->parent->right = tmp;
		}
		else
		{
			x->parent->left = tmp;
		}
		tmp->right = x;
		x->parent = tmp;
	}

	inline void __rb_tree_insert_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root)
	{
		x->color = __rb_tree_red;
		while (x != root && x->parent->color == __rb_tree_red)
		{
			if (x->parent == x->parent->parent->left)
			{
				__rb_tree_node_base* y = x->parent->parent->right;
				if (y && y->color == __rb_tree_red)
				{
					x->parent->color = __rb_tree_black;
					y->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					x = x->parent->parent;
				}
				else
				{
					if (x == x->parent->right)
					{
						x = x->parent;
						__rb_tree_rotate_left(x, root);
					}
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_right(x->parent->parent, root);
				}
			}
			else
			{
				__rb_tree_node_base* y = x->parent->parent->left;
				if (y && y->color == __rb_tree_red)
				{
					x->parent->color = __rb_tree_black;
					y->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					x = x->parent->parent;
				}
				else
				{
					if (x == x->parent->left)
					{
						x = x->parent;
						__rb_tree_rotate_right(x, root);
					}
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_left(x->parent->parent, root);
				}
			}
		}
		root->color = __rb_tree_black;
	}

	inline void __rb_tree_erase_rebalance(__rb_tree_node_base* bro, __rb_tree_node_base*& root)
	{
		bool x_in_left = (bro->parent->right == bro);
		if (bro->color == __rb_tree_black)
		{
			if (x_in_left)
			{
				if (bro->right != nullptr && bro->right->color == __rb_tree_red)
				{
					bro->right->color = __rb_tree_black;
					bro->color = bro->parent->color;
					bro->parent->color = __rb_tree_black;
					__rb_tree_rotate_left(bro->parent, root);
				}
				else if (bro->left != nullptr && bro->left->color == __rb_tree_red)
				{
					bro->left->color = bro->parent->color;
					bro->parent->color = __rb_tree_black;
					__rb_tree_rotate_right(bro, root);
					__rb_tree_rotate_left(bro->parent->parent, root);
				}
				else // all son is black/null
				{
					if (bro->parent->color == __rb_tree_red)
					{
						bro->parent->color = __rb_tree_black;
						bro->color = __rb_tree_red;
					}
					else // black
					{
						if (bro->parent == root)
							return;
						__rb_tree_node_base* prt_bro;
						if (bro->parent->parent->left == bro->parent)
						{
							prt_bro = bro->parent->parent->right;
						}
						else
						{
							prt_bro = bro->parent->parent->left;
						}
						bro->color = __rb_tree_red;
						__rb_tree_erase_rebalance(prt_bro, root);
					}
				}
			}
			else
			{
				if (bro->left != nullptr && bro->left->color == __rb_tree_red)
				{
					bro->left->color = __rb_tree_black;
					bro->color = bro->parent->color;
					bro->parent->color = __rb_tree_black;
					__rb_tree_rotate_right(bro->parent, root);
				}
				else if (bro->right != nullptr && bro->right->color == __rb_tree_red)
				{
					bro->right->color = bro->parent->color;
					bro->parent->color = __rb_tree_black;
					__rb_tree_rotate_left(bro, root);
					__rb_tree_rotate_right(bro->parent->parent, root);
				}
				else // all son is black/null
				{
					if (bro->parent->color == __rb_tree_red)
					{
						bro->parent->color = __rb_tree_black;
						bro->color = __rb_tree_red;
					}
					else // black
					{
						if (bro->parent == root)
							return;
						__rb_tree_node_base* prt_bro;
						if (bro->parent->parent->left == bro->parent)
						{
							prt_bro = bro->parent->parent->right;
						}
						else
						{
							prt_bro = bro->parent->parent->left;
						}
						bro->color = __rb_tree_red;
						__rb_tree_erase_rebalance(prt_bro, root);
					}
				}
			}
		}
		else
		{
			if (x_in_left)
			{
				// bro must have son
				__rb_tree_color_type tmp = bro->color;
				bro->color = bro->left->color;
				bro->left->color = tmp;
				__rb_tree_rotate_left(bro->parent, root);
			}
			else
			{
				__rb_tree_color_type tmp = bro->color;
				bro->color = bro->right->color;
				bro->right->color = tmp;
				__rb_tree_rotate_right(bro->parent, root);
			}
		}
	}

	template<
		typename K, typename V, typename Kov,
		typename Compare,
		typename Alloc = allocator<__rb_tree_node<V>>
	>
	class rb_tree
	{
	protected:
		using void_pointer = void*;
		using base_ptr = __rb_tree_node_base*;
		using rb_tree_node = __rb_tree_node<V>;
		using node_allocator = Alloc;
		using color_type = __rb_tree_color_type;
	public:
		using key_type = K;
		using value_type = V;
		using pointer = V*;
		using const_pointer = const V*;
		using reference = V&;
		using const_reference = const V&;
		using node_pointer = rb_tree_node*;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		using self_type = rb_tree<K, V, Kov, Compare, Alloc>;
		using iterator = __rb_tree_iterator<value_type, reference, pointer>;

	public:
		rb_tree(const Compare& comp = Compare(), const Kov& kov = Kov())
			: node_count(0), key_compare(comp), key_of_value(kov)
		{
			init_header();
		}
		~rb_tree()
		{
			clear();
			this->dealloc_node(header);
		}

		iterator begin() { return left_most(); }
		iterator end() { return header; }
		bool empty() const { return node_count == 0; }
		size_type size() const { return node_count; }

		void clear()
		{
			if (node_count != 0)
			{
				destroy_subtree(root());
				root() = nullptr;
				left_most() = header;
				right_most() = header;
				node_count = 0;
			}
		}
		bool insert_unique(const_reference x)
		{
			// find position
			node_pointer prt = header;
			node_pointer pos = root();
			bool less = true;
			while (pos != nullptr)
			{
				prt = pos;
				less = key_compare(Kov()(x), key(pos));
				pos = less ? left(pos) : right(pos);
			}

			iterator prt_iter = iterator(prt);
			if (less)
			{
				if (prt_iter == begin())
				{
					__insert(prt, x);
					return true;
				}
				else
				{
					--prt_iter;
				}
			}
			if (key_compare(key(prt_iter.cur_node), Kov()(x)))
			{
				__insert(prt, x);
				return true;
			}
			// same key
			return false;
		}
		bool erase(const iterator& x)
		{
			node_pointer target = node_pointer(x.cur_node);
			node_pointer prt = header;
			node_pointer pos = root();
			while (pos != target)
			{
				prt = pos;
				pos = key_compare(key_of_value(value(target)), key(pos)) ? left(pos) : right(pos);
				if (pos == nullptr) return false;
			}

			if (pos == header->parent && node_count == 1)
			{
				header->parent = nullptr;
				header->left = header;
				header->right = header;
				destroy_node(pos);
				return true;
			}

			// target == pos now
			if (target->left != nullptr && target->right != nullptr)
			{
				pos = node_pointer(target->right);
				while (pos->left != nullptr) pos = node_pointer(pos->left);
				__exchange_by_value(target, pos);
			}

			if (pos->left != nullptr)
			{
				__exchange_by_value(pos, node_pointer(pos->left));
				__erase(pos->left);
			}
			else if (pos->right != nullptr)
			{
				__exchange_by_value(pos, node_pointer(pos->right));
				__erase(pos->right);
			}
			else
			{
				__erase(pos);
			}
			return true;
		}

	protected:
		size_type node_count;
		node_pointer header;
		Compare key_compare;
		Kov key_of_value;

		// pointer helper
		node_pointer& root() const { return (node_pointer&)(header->parent); }
		node_pointer& left_most() const { return (node_pointer&)(header->left); }
		node_pointer& right_most() const { return (node_pointer&)(header->right); }

		static node_pointer& left(node_pointer x) { return (node_pointer&)(x->left); }
		static node_pointer& right(node_pointer x) { return (node_pointer&)(x->right); }
		static node_pointer& parent(node_pointer x) { return (node_pointer&)(x->parent); }
		static reference value(node_pointer x) { return x->value_field; }
		static color_type& color(node_pointer x) { return x->color; }
		static const key_type& key(node_pointer x) { return Kov()(value(x)); }

		static node_pointer& left(base_ptr x) { return (node_pointer&)(x->left); }
		static node_pointer& right(base_ptr x) { return (node_pointer&)(x->right); }
		static node_pointer& parent(base_ptr x) { return (node_pointer&)(x->parent); }
		static reference value(base_ptr x) { return node_pointer(x)->value_field; }
		static color_type& color(base_ptr x) { return x->color; }
		static const key_type& key(base_ptr x) { return Kov()(value(x)); }

		static node_pointer minimum(node_pointer x) { return node_pointer(__rb_tree_node_base::minimum(x)); }
		static node_pointer maximum(node_pointer x) { return node_pointer(__rb_tree_node_base::maximum(x)); }

	private:
		void init_header()
		{
			header = this->alloc_node();
			color(header) = __rb_tree_red;
			root() = nullptr;
			left_most() = header;
			right_most() = header;
		}
		iterator __insert(base_ptr _pos_prt, const_reference x)
		{
			node_pointer pos_prt = node_pointer(_pos_prt);
			node_pointer new_node = create_node(x);
			if (pos_prt == header)
			{
				left_most() = new_node;
				root() = new_node;
				right_most() = new_node;
			}
			else if (key_compare(Kov()(x), key(pos_prt))) // less
			{
				left(pos_prt) = new_node;
				if (pos_prt == left_most())
					left_most() = new_node;
			}
			else
			{
				right(pos_prt) = new_node;
				if (pos_prt == right_most())
					right_most() = new_node;
			}

			parent(new_node) = pos_prt;
			left(new_node) = nullptr;
			right(new_node) = nullptr;

			__rb_tree_insert_rebalance(new_node, header->parent);
			++node_count;
			return iterator(new_node);
		}
		void __exchange_by_value(node_pointer a, node_pointer b)
		{
			decltype(a->value_field) tmp = a->value_field;
			a->value_field = b->value_field;
			b->value_field = tmp;
		}
		void __erase(base_ptr x)
		{
			bool need_adapt = (x->color == __rb_tree_black);
			if (right_most() == x)
			{
				right_most() = node_pointer((--iterator(x)).cur_node);
			}
			if (left_most() == x)
			{
				left_most() = node_pointer((++iterator(x)).cur_node);
			}

			base_ptr bro = nullptr;
			if (x->parent->right == x)
			{
				x->parent->right = nullptr;
				bro = x->parent->left;
			}
			else
			{
				x->parent->left = nullptr;
				bro = x->parent->right;
			}
			destroy_node(node_pointer(x));

			if (need_adapt)
			{
				__rb_tree_erase_rebalance(bro, header->parent);
			}
			--node_count;
		}

	protected:
		// memory helper
		node_pointer alloc_node() { return node_allocator::allocate(1); }
		void dealloc_node(node_pointer p) { return node_allocator::deallocate(p); }
		node_pointer create_node(const value_type& x)
		{
			node_pointer tmp = this->alloc_node();
			construct(&(tmp->value_field), x);
			return tmp;
		}
		void destroy_node(node_pointer p)
		{
			destroy(&(p->value_field));
			this->dealloc_node(p);
		}
		void destroy_subtree(node_pointer p)
		{
			if (p->left != nullptr)
				destroy_subtree(node_pointer(p->left));
			if (p->right != nullptr)
				destroy_subtree(node_pointer(p->right));
			destroy_node(p);
		}
	};
}