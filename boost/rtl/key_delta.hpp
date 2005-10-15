// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_KEY_DELTA_HPP_INCLUDED
#define BOOST_RTL_KEY_DELTA_HPP_INCLUDED

#include "key.hpp"

namespace rel
{
	template<class R> struct key_traits
	{
		typedef k_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::plus_type, BOOST_TYPENAME R::key_type> plus_type;
		typedef k_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::minus_type, BOOST_TYPENAME R::key_type> minus_type;
	};

	template<class T, class Keys>
	BOOST_TYPENAME traits<k_t<T,Keys> >::delta_type
		_delta(const k_t<T,Keys>& k, const transaction& tr)
	{
		return BOOST_TYPENAME traits<k_t<T,Keys> >::delta_type
		(
			key<Keys>(delta(k.argument(),tr).first),
			key<Keys>(delta(k.argument(),tr).second)
		);
	};

}

#endif

