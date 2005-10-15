// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov 
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_TABLE_DELTA_HPP_INCLUDED
#define BOOST_RTL_TABLE_DELTA_HPP_INCLUDED

#include "traits.hpp"
#include "table.hpp"
#include "transaction.hpp" // added

namespace rel
{
	template<class R> struct table_traits
	{
		typedef typename table_delta<R>::plus_type plus_type;
		typedef typename table_delta<R>::minus_type minus_type;
	};
	template<class Info>
		BOOST_TYPENAME traits<table<Info> >::delta_type 
		_delta(const table<Info>& t, const transaction& tr) 
	{
		return tr.delta(t);
	}
}

#endif//BOOST_RTL_TABLE_DELTA_HPP_INCLUDED
