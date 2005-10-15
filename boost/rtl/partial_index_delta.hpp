// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_PARTIAL_INDEX_DELTA_HPP_INCLUDED
#define BOOST_RTL_PARTIAL_INDEX_DELTA_HPP_INCLUDED

#include "traits.hpp"
#include "partial_index.hpp"

namespace rel
{
	template<class R> struct partial_index_traits
	{
		typedef ix_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::plus_type,BOOST_TYPENAME R::strategy_type> plus_type;
		typedef ix_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::minus_type,BOOST_TYPENAME R::strategy_type> minus_type;
	};

	template<class Tb,class Str>
		BOOST_TYPENAME traits<ix_t<Tb,Str> >::delta_type 
		_delta(const ix_t<Tb,Str>& idx, const transaction& tr)
	{
		return BOOST_TYPENAME traits<ix_t<Tb,Str> >::delta_type
		(
			BOOST_TYPENAME traits<ix_t<Tb,Str> >::plus_type(delta(idx.argument(), tr).first),
			BOOST_TYPENAME traits<ix_t<Tb,Str> >::minus_type(delta(idx.argument(), tr).second)
		);
	}

};

#endif
