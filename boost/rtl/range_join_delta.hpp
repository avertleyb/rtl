// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_RANGE_JOIN_DELTA_HPP_INCLUDED
#define BOOST_RTL_RANGE_JOIN_DELTA_HPP_INCLUDED

#include "traits.hpp"
#include "range_join.hpp"

namespace rel
{
	template<class R> struct range_join_traits
	{
		typedef typename traits<typename R::arg1_type>::minus_type min1; 
		typedef typename traits<typename R::arg2_type>::minus_type min2; 
		typedef typename traits<typename R::arg1_type>::plus_type pls1; 
		typedef typename traits<typename R::arg2_type>::plus_type pls2; 
		typedef typename R::arg1_type arg1;
		typedef typename R::arg2_type arg2;
		typedef typename R::begin_type begin_type;
		typedef typename R::end_type end_type;

		typedef typename union_t<
			rj_t<min1, arg2, begin_type, end_type>, 
			rj_t<typename difference_t<arg1, min1>::type, min2, begin_type, end_type> >::type minus_type;

		typedef typename union_t<
			rj_t<pls1, typename difference_t<arg2, min2>::type, begin_type, end_type>, 
			rj_t<typename union_t<typename difference_t<arg1, min1>::type, pls1>::type, pls2, begin_type, end_type> >::type plus_type;
	};
	template<class T1, class T2, class Begin, class End>
		typename traits<rj_t<T1, T2, Begin, End> >::delta_type 
		_delta(const rj_t<T1, T2, Begin, End>& rj, const transaction& tr)
	{
		typedef range_join_traits<rj_t<T1, T2, Begin, End> > traits_type;

		typename traits_type::min1 m1 = delta(rj.argument1(), tr).second;
		typename traits_type::min2 m2 = delta(rj.argument2(), tr).second;
		typename traits_type::pls1 p1 = delta(rj.argument1(), tr).first;
		typename traits_type::pls2 p2 = delta(rj.argument2(), tr).first;
		T1 t1 = rj.argument1();
		T2 t2 = rj.argument2();
		Begin fbegin = rj.begin_functor();
		End fend = rj.end_functor();

		typename traits_type::minus_type minus =
			Union(range_join(m1, t2, fbegin, fend), range_join(difference(t1, m1), m2, fbegin, fend));

		typename traits_type::plus_type plus =
			Union(range_join(p1, difference(t2, m2), fbegin, fend), range_join(Union(difference(t1, m1), p1), p2, fbegin, fend)); 

		return std::make_pair(plus, minus);
	}
};

#endif//BOOST_RTL_RANGE_JOIN_DELTA_HPP_INCLUDED
