#pragma once
#include <initializer_list>

namespace TestUnits
{
	template<typename VT, typename Container>
	inline int compareContent(const std::initializer_list<VT>& vs, const Container& con)
	{
		if (vs.size() != con.size()) return -1;
		auto v_cur = vs.begin();
		auto c_cur = con.cbegin();
		for (int i = 0; i < vs.size(); ++i)
		{
			if (*v_cur != *c_cur)
				return i;
			++v_cur;
			++c_cur;
		}
		return 0;
	}
}