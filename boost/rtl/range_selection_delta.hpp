// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_RANGE_SELECTION_DELTA_HPP_INCLUDED
#define BOOST_RTL_RANGE_SELECTION_DELTA_HPP_INCLUDED

#include <boost/rtl/traits.hpp>
#include <boost/rtl/range_selection.hpp>

namespace boost { namespace rel {

	template<class R> 
		struct range_selection_traits
	{
		typedef rsel_t<
			typename traits<typename R::arg_type>::plus_type,
			typename R::begin_type,
			typename R::end_type
			> plus_type;

		typedef rsel_t<
			typename traits<typename R::arg_type>::minus_type,
			typename R::begin_type,
			typename R::end_type
			> minus_type;
	};
	template<class T, class Begin, class End>
		BOOST_MSVC_TYPENAME traits<rsel_t<T, Begin, End> >::delta_type 
		_delta(const rsel_t<T, Begin, End>& rs, const transaction& tr)
	{
		return BOOST_MSVC_TYPENAME traits<rsel_t<T, Begin, End> >::delta_type(
			range_selection(delta(rs.argument(), tr).first, rs.begin_functor(), rs.end_functor()),
			range_selection(delta(rs.argument(), tr).second, rs.begin_functor(), rs.end_functor()));
	}
	template<class R> 
		struct equal_selection_traits
	{
		typedef esel_t<
			typename traits<typename R::arg_type>::plus_type,
			typename R::sub_type
			> plus_type;

		typedef esel_t<
			typename traits<typename R::arg_type>::minus_type,
			typename R::sub_type
			> minus_type;
	};
	template<class T, class Sub>
		BOOST_MSVC_TYPENAME traits<esel_t<T, Sub> >::delta_type 
		_delta(const esel_t<T, Sub>& es, const transaction& tr)
	{
		return BOOST_MSVC_TYPENAME traits<esel_t<T, Sub> >::delta_type(
			equal_selection(delta(es.argument(), tr).first, es.sub()),
			equal_selection(delta(es.argument(), tr).second, es.sub()));
	}
}}

#endif//BOOST_RTL_RANGE_SELECTION_DELTA_HPP_INCLUDED
