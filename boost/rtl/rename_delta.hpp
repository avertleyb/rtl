// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_RENAME_DELTA_HPP_INCLUDED
#define BOOST_RTL_RENAME_DELTA_HPP_INCLUDED

#include <boost/rtl/traits.hpp>
#include <boost/rtl/rename.hpp>

namespace boost { namespace rel {

	template<class R> struct cnv_traits
	{
		typedef cnv_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::plus_type, BOOST_TYPENAME R::pairs> plus_type;
		typedef cnv_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::minus_type, BOOST_TYPENAME R::pairs> minus_type;
	};

	template<class Table, class Pairs>
		BOOST_TYPENAME traits<cnv_t<Table,Pairs> >::delta_type 
	_delta(const cnv_t<Table,Pairs>& cnv, const transaction& tr)
	{
		BOOST_TYPENAME traits<cnv_t<Table,Pairs> >::plus_type plus(delta(cnv.argument(), tr).first);
		BOOST_TYPENAME traits<cnv_t<Table,Pairs> >::minus_type minus(delta(cnv.argument(), tr).second);
		return BOOST_TYPENAME traits<cnv_t<Table,Pairs> >::delta_type(plus,minus);
	};
}}

#endif//BOOST_RTL_RENAME_DELTA_HPP_INCLUDED 
