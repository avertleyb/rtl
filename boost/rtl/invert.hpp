// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_INVERT_HPP_INCLUDED
#define BOOST_RTL_INVERT_HPP_INCLUDED

#include "table.hpp"

namespace rel
{
	template
	<
		class SortList, 
		class Strategy,
		class Table
	>
	table
	<
		table_info
		<
			typename Table::field_list,
			SortList,
			typename Table::keys,
			Strategy
		> 
	>
		invert(const Table& t, const SortList* s = 0, const Strategy* p = 0)
	{
		table
		<
			table_info
			<
				BOOST_TYPENAME Table::field_list,
				SortList,
				BOOST_TYPENAME Table::keys,
				Strategy
			> 
		> res;

		// res.reserve(t.size()); Sometimes it can make operations faster
				
		BOOST_TYPENAME Table::const_iterator e(t.end());
		for(BOOST_TYPENAME Table::const_iterator itr = t.begin(); itr != e; ++itr)
		{
			BOOST_TYPENAME Table::value_type temp;
			copy_fields<BOOST_TYPENAME Table::value_type::field_list>(*itr,temp);
			res.insert(temp);
		}
		return res;
	}

}
#endif//BOOST_RTL_INVERT_HPP_INCLUDED
