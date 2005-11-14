// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_SELECTION_DELTA_HPP_INCLUDED
#define BOOST_RTL_SELECTION_DELTA_HPP_INCLUDED

#include "traits.hpp"
#include "selection.hpp"

namespace rel
{
	template<class R> struct selection_traits
	{
		typedef sel_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::plus_type, BOOST_TYPENAME R::pred_type> plus_type;
		typedef sel_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::minus_type, BOOST_TYPENAME R::pred_type> minus_type;
	};
	template<class Tb, class Pred>
		BOOST_TYPENAME traits<sel_t<Tb, Pred> >::delta_type 
		_delta(const sel_t<Tb, Pred>& sel, const transaction& tr)
	{
        typename traits<typename sel_t<Tb, Pred>::arg_type>::delta_type d = 
            delta(sel.argument(), tr); 

		return BOOST_TYPENAME traits<sel_t<Tb, Pred> >::delta_type(
			selection(d.first, sel.pred()),
			selection(d.second, sel.pred()));
	}
}

#endif//BOOST_RTL_SELECTION_DELTA_HPP_INCLUDED
