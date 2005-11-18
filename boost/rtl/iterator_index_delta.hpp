// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_ITERATOR_INDEX_DELTA_HPP_INCLUDED
#define BOOST_RTL_ITERATOR_INDEX_DELTA_HPP_INCLUDED

#include <boost/rtl/traits.hpp>
#include <boost/rtl/iterator_index.hpp>

namespace boost { namespace rel {

	template<class R> struct iterator_index_traits
	{
		typedef it_idx_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::plus_type, BOOST_TYPENAME R::sort_list, BOOST_TYPENAME R::strategy_type> plus_type;
		typedef it_idx_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::minus_type, BOOST_TYPENAME R::sort_list, BOOST_TYPENAME R::strategy_type> minus_type;
	};
	template<class Tb, class SL, class Str>
		BOOST_TYPENAME traits<it_idx_t<Tb, SL, Str> >::delta_type 
		_delta(const it_idx_t<Tb, SL, Str>& idx, const transaction& tr)
	{
		return BOOST_TYPENAME traits<it_idx_t<Tb, SL, Str> >::delta_type(
			BOOST_TYPENAME traits<it_idx_t<Tb, SL, Str> >::plus_type(delta(idx.argument(), tr).first),
			BOOST_TYPENAME traits<it_idx_t<Tb, SL, Str> >::minus_type(delta(idx.argument(), tr).second));
	}
}}

#endif//BOOST_RTL_ITERATOR_INDEX_DELTA_HPP_INCLUDED
