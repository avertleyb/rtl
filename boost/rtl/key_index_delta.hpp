// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_KEY_INDEX_DELTA_HPP_INCLUDED
#define BOOST_RTL_KEY_INDEX_DELTA_HPP_INCLUDED

#include <boost/rtl/traits.hpp>
#include <boost/rtl/key_index.hpp>

namespace boost { namespace rel {

	template<class R> struct key_index_traits
	{
		typedef key_idx_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::plus_type, BOOST_TYPENAME R::sort_list, BOOST_TYPENAME R::strategy_type> plus_type;
		typedef key_idx_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::minus_type, BOOST_TYPENAME R::sort_list, BOOST_TYPENAME R::strategy_type> minus_type;
	};
	template<class Tb, class SL, class Str>
		BOOST_TYPENAME traits<key_idx_t<Tb, SL, Str> >::delta_type 
		_delta(const key_idx_t<Tb, SL, Str>& idx, const transaction& tr)
	{
        typename traits<typename key_idx_t<Tb, SL, Str>::arg_type>::delta_type d = delta(idx.argument(), tr);

		return typename traits<key_idx_t<Tb, SL, Str> >::delta_type(
			BOOST_TYPENAME traits<key_idx_t<Tb, SL, Str> >::plus_type(d.first),
			BOOST_TYPENAME traits<key_idx_t<Tb, SL, Str> >::minus_type(d.second));
	}
}}

#endif//BOOST_RTL_KEY_INDEX_DELTA_HPP_INCLUDED
