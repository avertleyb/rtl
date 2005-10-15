// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_CACHE_DELTA_HPP_INCLUDED
#define BOOST_RTL_CACHE_DELTA_HPP_INCLUDED

#include "traits.hpp"
#include "cache.hpp"

namespace rel
{
	template<class R> struct cache_traits
	{
		typedef ch_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::plus_type> plus_type;
		typedef ch_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::minus_type> minus_type;
	};

	template<class T>
	BOOST_TYPENAME traits<ch_t<T> >::delta_type
	_delta(const ch_t<T>& ch, const transaction& tr)
	{
		return BOOST_TYPENAME traits<ch_t<T> >::delta_type
		(
			cache(delta(ch.argument(),tr).first),
			cache(delta(ch.argument(),tr).second)
		);
	};

}
#endif
