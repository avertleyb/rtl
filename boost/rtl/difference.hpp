// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RTL_DIFFERENCE_HPP_INCLUDED
#define BOOST_RTL_DIFFERENCE_HPP_INCLUDED

#include <boost/rtl/zigzag.hpp>

namespace boost { namespace rel {

	template<class Table1, class Table2>
		struct difference_t
	{
		typedef zigzag<Table1, Table2, d_pl> type;
	};

	template<class Table1, class Table2>
		typename difference_t<Table1, Table2>::type difference(const Table1& t1, const Table2& t2)
	{
		return typename difference_t<Table1, Table2>::type(t1, t2);
	}
}}

#endif//BOOST_RTL_DIFFERENCE_HPP_INCLUDED
