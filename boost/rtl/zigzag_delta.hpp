// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_ZIGZAG_DELTA_HPP_INCLUDED
#define BOOST_RTL_ZIGZAG_DELTA_HPP_INCLUDED

#include <boost/rtl/traits.hpp>
#include <boost/rtl/intersection.hpp>
#include <boost/rtl/union.hpp>
#include <boost/rtl/difference.hpp>
#include <boost/rtl/isection.hpp>

namespace boost { namespace rel {

	// union
	template<class R> struct union_traits
	{
		typedef typename R::arg1_type tab1;
		typedef typename R::arg2_type tab2;
		typedef typename traits<tab1>::minus_type min1;
		typedef typename traits<tab2>::minus_type min2;
		typedef typename traits<tab1>::plus_type pls1;
		typedef typename traits<tab2>::plus_type pls2;

		//  ((m1+(m2-t1))+(t2*p1))
		typedef typename union_t
		<
			typename union_t
			<
				min1,
				typename diff_t
				<
					min2,
					tab1
				>::type
			>::type,
			typename rsection_t
			<
				pls1,
				tab2
			>::type
		>::type minus_type;

	// ((p1+((t2*m1)-m2))+(p2-(t1-m1)))	
		typedef typename union_t
		<
			typename union_t
			<
				pls1,
				typename difference_t
				<
					typename rsection_t
					<
						min1,
						tab2
					>::type,
					min2
				>::type
			>::type,
			typename diff_t
			<
				pls2,
				typename difference_t
				<
					tab1,
					min1
				>::type
			>::type
		>::type plus_type;
	};

	// intersection
	template<class R> struct intersection_traits
	{
		typedef typename R::arg1_type tab1;
		typedef typename R::arg2_type tab2;
		typedef typename traits<tab1>::minus_type min1;
		typedef typename traits<tab2>::minus_type min2;
		typedef typename traits<tab1>::plus_type pls1;
		typedef typename traits<tab2>::plus_type pls2;

		// ((m1*t2)+(t1*(m2-p2)))
		typedef typename union_t
		<
			typename isection_t
			<
				min1,
				tab2
			>::type,
			typename rsection_t
			<
				typename difference_t
				<
					min2,
					pls2
				>::type,
				tab1
			>::type
		>::type minus_type;
		
	//  (((p1*t2)-m2)+(p1*p2))+(((t1*p2)-m1)-m2))
		typedef typename union_t
		<
			typename union_t
			<
				typename difference_t
				<
					typename isection_t
					<
						pls1,
						tab2
					>::type,
					min2
				>::type,
				typename intersection_t
				<
					pls1,
					pls2
				>::type
			>::type,
			typename difference_t
			<
				typename difference_t
				<
					typename rsection_t
					<
						pls2,
						tab1
					>::type,
					min1
				>::type,
				min2
			>::type
		>::type plus_type;
	};

	// difference
	template<class R> struct difference_traits
	{
		typedef typename R::arg1_type tab1;
		typedef typename R::arg2_type tab2;
		typedef typename traits<tab1>::minus_type min1;
		typedef typename traits<tab2>::minus_type min2;
		typedef typename traits<tab1>::plus_type pls1;
		typedef typename traits<tab2>::plus_type pls2;

		//  ((m1+(t1*p2))-t2)
		typedef typename diff_t
		<
			typename union_t
			<
				min1,
				typename rsection_t
				<
					pls2,
					tab1
				>::type
			>::type,
			tab2
		>::type minus_type;

	//  ((((t1-m1)*m2)+p1)-((t2-m2)+p2))
		typedef typename diff_t
		<
			typename union_t
			<
				typename rsection_t
				<
					min2,
					typename difference_t
					<
						tab1,
						min1
					>::type
				>::type,
				pls1
			>::type,
			typename union_t
			<
				typename difference_t
				<
					tab2,
					min2
				>::type,
				pls2
			>::type
		>::type plus_type;
	};

	// zigzag
	template<class P> struct policy_traits
	{
		template<class T> struct apply
		{
			typedef union_traits<T> type;
		};
	};

	template<> struct policy_traits<i_pl>
	{
		template<class T> struct apply
		{
			typedef intersection_traits<T> type;
		};
	};

	template<> struct policy_traits<d_pl>
	{
		template<class T> struct apply
		{
			typedef difference_traits<T> type;
		};
	};

	template<class R> struct zigzag_traits
	{
		typedef typename policy_traits<typename R::policy_type>::template apply<R>::type traits_type;
		typedef typename traits_type::plus_type plus_type;
		typedef typename traits_type::minus_type minus_type;
	};

	// union

	template<class T1, class T2>
	BOOST_TYPENAME traits<zigzag<T1,T2,u_pl> >::delta_type
	u_delta(const zigzag<T1,T2,u_pl>& un, const transaction& tr)
	{
		BOOST_TYPENAME union_traits<BOOST_TYPENAME union_t<T1,T2>::type>::min1 m1 = delta(un.argument1(), tr).second;
		BOOST_TYPENAME union_traits<BOOST_TYPENAME union_t<T1,T2>::type>::min2 m2 = delta(un.argument2(), tr).second;
		BOOST_TYPENAME union_traits<BOOST_TYPENAME union_t<T1,T2>::type>::pls1 p1 = delta(un.argument1(), tr).first;
		BOOST_TYPENAME union_traits<BOOST_TYPENAME union_t<T1,T2>::type>::pls2 p2 = delta(un.argument2(), tr).first;
		T1 t1 = un.argument1();
		T2 t2 = un.argument2();

		BOOST_TYPENAME union_traits<BOOST_TYPENAME union_t<T1,T2>::type>::minus_type minus =
			Union(Union(m1,diff(m2,t1)),rsection(p1,t2));

		BOOST_TYPENAME union_traits<BOOST_TYPENAME union_t<T1,T2>::type>::plus_type plus = 
			Union(Union(p1,difference(rsection(m1,t2),m2)),diff(p2,difference(t1,m1)));

		return BOOST_TYPENAME traits<BOOST_TYPENAME union_t<T1,T2>::type>::delta_type(plus,minus);
	}

	//intersection

	template<class T1, class T2>
	BOOST_TYPENAME traits<zigzag<T1, T2, i_pl> >::delta_type
	i_delta(const zigzag<T1, T2, i_pl>& is, const transaction& tr)
	{
		BOOST_TYPENAME intersection_traits<BOOST_TYPENAME intersection_t<T1,T2>::type>::min1 m1 = delta(is.argument1(), tr).second;
		BOOST_TYPENAME intersection_traits<BOOST_TYPENAME intersection_t<T1,T2>::type>::min2 m2 = delta(is.argument2(), tr).second;
		BOOST_TYPENAME intersection_traits<BOOST_TYPENAME intersection_t<T1,T2>::type>::pls1 p1 = delta(is.argument1(), tr).first;
		BOOST_TYPENAME intersection_traits<BOOST_TYPENAME intersection_t<T1,T2>::type>::pls2 p2 = delta(is.argument2(), tr).first;
		T1 t1 = is.argument1();
		T2 t2 = is.argument2();

		BOOST_TYPENAME intersection_traits<BOOST_TYPENAME intersection_t<T1,T2>::type>::minus_type minus = 
			Union(isection(m1,t2),rsection(difference(m2,p2),t1));

		BOOST_TYPENAME intersection_traits<BOOST_TYPENAME intersection_t<T1,T2>::type>::plus_type plus = 
			Union(Union(difference(isection(p1,t2),m2),intersection(p1,p2)),difference(difference(rsection(p2,t1),m1),m2));
		return BOOST_TYPENAME traits<BOOST_TYPENAME intersection_t<T1,T2>::type>::delta_type(plus,minus);
	}

	// difference

	template<class T1, class T2>
	BOOST_TYPENAME traits<zigzag<T1, T2, d_pl> >::delta_type
	d_delta(const zigzag<T1, T2, d_pl>& df, const transaction& tr)
	{
		BOOST_TYPENAME difference_traits<BOOST_TYPENAME difference_t<T1, T2>::type>::min1 m1 = delta(df.argument1(), tr).second; 
		BOOST_TYPENAME difference_traits<BOOST_TYPENAME difference_t<T1, T2>::type>::min2 m2 = delta(df.argument2(), tr).second; 
		BOOST_TYPENAME difference_traits<BOOST_TYPENAME difference_t<T1, T2>::type>::pls1 p1 = delta(df.argument1(), tr).first; 
		BOOST_TYPENAME difference_traits<BOOST_TYPENAME difference_t<T1, T2>::type>::pls2 p2 = delta(df.argument2(), tr).first; 
		T1 t1 = df.argument1();
		T2 t2 = df.argument2();

		BOOST_TYPENAME difference_traits<BOOST_TYPENAME difference_t<T1, T2>::type>::minus_type minus =
			diff(Union(m1,rsection(p2,t1)),t2);

		BOOST_TYPENAME difference_traits<BOOST_TYPENAME difference_t<T1, T2>::type>::plus_type plus =
			diff(Union(rsection(m2,difference(t1,m1)),p1),Union(difference(t2,m2),p2));

		return BOOST_TYPENAME traits<BOOST_TYPENAME difference_t<T1, T2>::type>::delta_type(plus,minus);
	}

	template<class Z>
	BOOST_TYPENAME traits<Z>::delta_type
	_delta(const u_pl*, const Z& zz, const transaction& tr)
	{
		return u_delta(zz, tr);
	}

	template<class Z>
	BOOST_TYPENAME traits<Z>::delta_type
	_delta(const i_pl*, const Z& zz, const transaction& tr)
	{
		return i_delta(zz, tr);
	}

	template<class Z>
	BOOST_TYPENAME traits<Z>::delta_type
	_delta(const d_pl*, const Z& zz, const transaction& tr)
	{
		return d_delta(zz, tr);
	}

	// common _delta for zigzag
	template<class T1, class T2, class P>
	BOOST_TYPENAME traits<zigzag<T1, T2, P> >::delta_type
	_delta(const zigzag<T1, T2, P>& zz, const transaction& tr)
	{
		return _delta((P*)0, zz, tr);
	}
}}

#endif //BOOST_RTL_ZIGZAG_DELTA_HPP_INCLUDED
