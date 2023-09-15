#pragma once
#include "deque.h"
#include "memory_all.h"

namespace PracticeStl
{
	template <typename T, typename Sequence = deque<T>>
	class queue
	{
		friend bool operator==(const queue<T>&, const queue<T>&);
		friend bool operator<(const queue<T>&, const queue<T>&);
	public:
		using value_type = typename Sequence::value_type;
		using size_type = typename Sequence::size_type;
		using reference = typename Sequence::reference;
		using const_reference = typename Sequence::const_reference;
	protected:
		Sequence base_con;
	public:
		bool empty() const { return base_con.empty(); }
		size_type size() const { return base_con.size(); }
		reference front() { return base_con.front(); }
		const_reference front() const { return base_con.front(); }
		reference back() { return base_con.back(); }
		const_reference back() const { return base_con.back(); }
		void push(const_reference x) { base_con.push_back(x); }
		void pop() { base_con.pop_front(); }
	};

	template <typename T, typename Sequence>
	bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y)
	{
		return x.base_con == y.base_con;
	}

	template <typename T, typename Sequence>
	bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y)
	{
		return x.base_con < y.base_con;
	}
}