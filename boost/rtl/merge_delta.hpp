// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_MERGE_DELTA_HPP_INCLUDED
#define BOOST_RTL_MERGE_DELTA_HPP_INCLUDED

#include <boost/rtl/traits.hpp>
#include <boost/rtl/merge.hpp>

namespace boost { namespace rel {

	// merge
	template<class R> struct merge_traits
	{
        typedef typename table_type<R>::type plus_type;
        typedef typename table_type<R>::type minus_type;
	};

	template<class T1, class T2, int PS>
	typename traits<mg_t<T1, T2, PS> >::delta_type
	_delta(const mg_t<T1, T2, PS>& mg, const transaction& tr)
	{
        typedef typename merge_t<T1, T2, PS>::type R;

		typedef typename R::arg1_type tab1;
		typedef typename R::arg2_type tab2;
		typedef typename traits<tab1>::delta_type del1;
		typedef typename traits<tab2>::delta_type del2;
		typedef typename traits<tab1>::minus_type min1;
		typedef typename traits<tab2>::minus_type min2;
		typedef typename traits<tab1>::plus_type pls1;
		typedef typename traits<tab2>::plus_type pls2;

        del1 d1 = delta(mg.argument1(), tr);
        del2 d2 = delta(mg.argument2(), tr);

        min1 m1 = d1.second;
		min2 m2 = d2.second;
		pls1 p1 = d1.first;
		pls2 p2 = d2.first;

		T1 t1 = mg.argument1();
		T2 t2 = mg.argument2();

        typename merge_traits<R>::minus_type minus;
        minus.append(Union(merge<PS>(m1, t2), merge<PS>(difference(t1, m1), m2)));

        typename merge_traits<R>::plus_type plus;
        plus.append(Union(merge<PS>(p1, difference(t2, m2)), merge<PS>(Union(difference(t1, m1), p1), p2)));

        return std::make_pair(plus, minus);
	}
}}

#endif
