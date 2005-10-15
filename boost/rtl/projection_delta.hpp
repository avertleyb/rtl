// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_PROJECTION_DELTA_HPP_INCLUDED
#define BOOST_RTL_PROJECTION_DELTA_HPP_INCLUDED

#include "traits.hpp"
#include "projection.hpp"
#include "groupby_delta.hpp"
#include "groupby_functors.hpp"
#include "join_xx.hpp"
#include "isection.hpp"
#include "difference.hpp"
#include "isection.hpp"

namespace rel
{
	template<class R> struct dprj_traits
	{
		// min(proj(t)) = diff(proj(min(t)), proj(difference(t, min(t))); 
		// pls(proj(t)) = diff(proj(pls(t)), proj(difference(t, min(t))); 

		typedef dprj_t
		<
			typename difference_t
			<
				typename R::arg_type,
				typename traits<typename R::arg_type>::minus_type
			>::type,
			typename R::restriction
		> helper_type;

		typedef typename diff_t
		<
			dprj_t
			<
				BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::minus_type, 
				BOOST_TYPENAME R::restriction
			>,
			helper_type
		>::type minus_type;

		typedef typename diff_t
		<
			dprj_t
			<
				BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::plus_type, 
				BOOST_TYPENAME R::restriction
			>,
			helper_type
		>::type plus_type;
	};

	template<class Table, class FieldList>
		BOOST_TYPENAME traits<dprj_t<Table, FieldList> >::delta_type 
		_delta(const dprj_t<Table, FieldList>& proj, const transaction& tr)
	{
		BOOST_TYPENAME dprj_traits<dprj_t<Table, FieldList> >::helper_type helper = difference(proj.argument(),delta(proj.argument(), tr).second);			
		BOOST_TYPENAME dprj_traits<dprj_t<Table, FieldList> >::minus_type minus  = diff(projection<FieldList>(delta(proj.argument(), tr).second),helper);
		BOOST_TYPENAME dprj_traits<dprj_t<Table, FieldList> >::plus_type plus = diff(projection<FieldList>(delta(proj.argument(), tr).first),helper);
		return BOOST_TYPENAME traits<dprj_t<Table, FieldList> >::delta_type(plus, minus);
	}

}

#endif//BOOST_RTL_PROJECTION_DELTA_HPP_INCLUDED
