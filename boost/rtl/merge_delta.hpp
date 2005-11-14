// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_MERGE_DELTA_HPP_INCLUDED
#define BOOST_RTL_MERGE_DELTA_HPP_INCLUDED

#include "traits.hpp"
#include "merge.hpp"

namespace rel
{
	// merge
	template<class R> struct merge_traits
	{
		typedef typename R::arg1_type tab1;
		typedef typename R::arg2_type tab2;
		typedef typename traits<tab1>::delta_type del1;
		typedef typename traits<tab2>::delta_type del2;
		typedef typename traits<tab1>::minus_type min1;
		typedef typename traits<tab2>::minus_type min2;
		typedef typename traits<tab1>::plus_type pls1;
		typedef typename traits<tab2>::plus_type pls2;

		typedef typename union_t
		<
			mg_t<min1, tab2, R::prefix_size>, // TODO!!! replace with equal_join
			mg_t<typename difference_t<tab1, min1>::type, min2, R::prefix_size> // TODO!!! replace with equal_join
		>::type minus_type;

		typedef BOOST_TYPENAME union_t
		<
			mg_t<pls1, typename difference_t<tab2, min2>::type, R::prefix_size>, // TODO!!! replace with equal_join
			mg_t																 // TODO!!! replace with equal_join
			<
				typename union_t<typename difference_t<tab1, min1>::type, pls1>::type, 
				pls2, 
				R::prefix_size
			> 
		>::type plus_type;
	};

	template<class T1, class T2, int PS>
	BOOST_TYPENAME traits<mg_t<T1,T2,PS> >::delta_type
	_delta(const mg_t<T1,T2,PS>& mg, const transaction& tr)
	{
        typename merge_traits<typename merge_t<T1, T2, PS>::type>::del1 d1 = delta(mg.argument1(), tr);
        typename merge_traits<typename merge_t<T1, T2, PS>::type>::del2 d2 = delta(mg.argument2(), tr);

        BOOST_TYPENAME merge_traits<BOOST_TYPENAME merge_t<T1, T2, PS>::type >::min1 m1 = d1.second;
		BOOST_TYPENAME merge_traits<BOOST_TYPENAME merge_t<T1, T2, PS>::type >::min2 m2 = d2.second;
		BOOST_TYPENAME merge_traits<BOOST_TYPENAME merge_t<T1, T2, PS>::type >::pls1 p1 = d1.first;
		BOOST_TYPENAME merge_traits<BOOST_TYPENAME merge_t<T1, T2, PS>::type >::pls2 p2 = d2.first;
		T1 t1 = mg.argument1();
		T2 t2 = mg.argument2();

		BOOST_TYPENAME merge_traits<BOOST_TYPENAME merge_t<T1, T2, PS>::type>::minus_type minus =
			Union(merge<PS>(m1, t2), merge<PS>(difference(t1, m1), m2));

		BOOST_TYPENAME merge_traits<BOOST_TYPENAME merge_t<T1, T2, PS>::type>::plus_type plus =
			Union(merge<PS>(p1, difference(t2, m2)), merge<PS>(Union(difference(t1, m1), p1), p2));

		return BOOST_TYPENAME traits<BOOST_TYPENAME merge_t<T1, T2, PS>::type>::delta_type(plus, minus);
	}
}

#endif
