// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RTL_UNION_HPP_INCLUDED
#define BOOST_RTL_UNION_HPP_INCLUDED

#include <boost/rtl/zigzag.hpp>
#include <boost/rtl/keys.hpp>

namespace boost { namespace rel {

	template<class Table1, class Table2>
		struct union_t
	{
		typedef zigzag<Table1, Table2, u_pl> type;
	};

	template<class Table1, class Table2>
		typename union_t<Table1, Table2>::type Union(const Table1& t1, const Table2& t2)
	{
		return typename union_t<Table1, Table2>::type(t1, t2);
	}
}}

#endif//BOOST_RTL_UNION_HPP_INCLUDED
