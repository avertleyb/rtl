// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov 
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RTL_TABLE_DELTA_HPP_INCLUDED
#define BOOST_RTL_TABLE_DELTA_HPP_INCLUDED

#include <boost/rtl/traits.hpp>
#include <boost/rtl/table.hpp>
#include <boost/rtl/transaction.hpp> // added

namespace boost { namespace rel {

	template<class R> 
    struct traits_impl<R, table_tag>
	{
		typedef typename table_delta<R>::plus_type plus_type;
		typedef typename table_delta<R>::minus_type minus_type;
	};

	template<class R>
	typename traits<R>::delta_type delta(const R& t, const transaction& tr, table_tag) 
	{
		return tr.delta(t);
	}
}}

#endif//BOOST_RTL_TABLE_DELTA_HPP_INCLUDED
