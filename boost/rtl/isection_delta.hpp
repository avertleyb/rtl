// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_ISECTION_DELTA_HPP_INCLUDED
#define BOOST_RTL_ISECTION_DELTA_HPP_INCLUDED

#include <boost/rtl/traits.hpp>
#include <boost/rtl/isection.hpp>

namespace boost { namespace rel {

	template<class R> struct isection_traits
	{
		typedef typename R::arg1_type tab1;
		typedef typename R::arg2_type tab2;
		typedef typename traits<tab1>::minus_type min1;
		typedef typename traits<tab2>::minus_type min2;
		typedef typename traits<tab1>::plus_type pls1;
		typedef typename traits<tab2>::plus_type pls2;

	//  ((m1*t2)+(t1*(m2-p2)))
		typedef typename union_t
		<
			typename isection_t
			<
				min1,
				tab2
			>::type,
			typename rsection_t
			<
				typename diff_t
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

	template <class R> struct rsection_traits
	{
		typedef BOOST_TYPENAME R::arg1_type tab1;
		typedef BOOST_TYPENAME R::arg2_type tab2;
		typedef BOOST_TYPENAME traits<BOOST_TYPENAME R::arg1_type>::minus_type min1;
		typedef BOOST_TYPENAME traits<BOOST_TYPENAME R::arg2_type>::minus_type min2;
		typedef BOOST_TYPENAME traits<BOOST_TYPENAME R::arg1_type>::plus_type pls1;
		typedef BOOST_TYPENAME traits<BOOST_TYPENAME R::arg2_type>::plus_type pls2;

		typedef BOOST_TYPENAME union_t
		<
			typename intersection_t
			<
				min2,
				BOOST_TYPENAME R::arg1_type
			>::type,
			typename intersection_t
			<
				BOOST_TYPENAME R::arg2_type,
				BOOST_TYPENAME difference_t<min1,pls1>::type
			>::type
		>::type minus_type;

		typedef BOOST_TYPENAME union_t
		<
			typename intersection_t<
				pls2,
				typename union_t
				<
					typename difference_t
					<
						BOOST_TYPENAME R::arg1_type,
						min1
					>::type,
					pls1
				>::type
			>::type,
			typename difference_t
			<
				typename difference_t
				<
					typename intersection_t
					<
						BOOST_TYPENAME R::arg2_type,
						pls1
					>::type,
					min2
				>::type,
				min1
			>::type
		>::type plus_type;
	};

	template<class R> struct diff_traits
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

	// is_t
	template<class IsP> struct is_policy_traits
	{
		template<class T> struct apply
		{
			typedef isection_traits<T> type;
		};
	};

	template<> struct is_policy_traits<rs_pl>
	{
		template<class T> struct apply
		{
			typedef rsection_traits<T> type;
		};
	};

	template<> struct is_policy_traits<df_pl>
	{
		template<class T> struct apply
		{
			typedef diff_traits<T> type;
		};
	};

	template<class R> struct is_traits
	{
		typedef typename is_policy_traits<typename R::is_policy>::template apply<R>::type traits_type;
		typedef typename traits_type::plus_type plus_type;
		typedef typename traits_type::minus_type minus_type;
	};


	template<class T1, class T2>
	BOOST_TYPENAME traits<is_t<T1,T2,is_pl> >::delta_type
	f_delta(const is_t<T1,T2,is_pl>& is, const transaction& tr)
	{
		BOOST_TYPENAME isection_traits<is_t<T1,T2,is_pl> >::min1 m1 = delta(is.argument1(), tr).second;
		BOOST_TYPENAME isection_traits<is_t<T1,T2,is_pl> >::min2 m2 = delta(is.argument2(), tr).second;
		BOOST_TYPENAME isection_traits<is_t<T1,T2,is_pl> >::pls1 p1 = delta(is.argument1(), tr).first;
		BOOST_TYPENAME isection_traits<is_t<T1,T2,is_pl> >::pls2 p2 = delta(is.argument2(), tr).first;

		T1 t1 = is.argument1();
		T2 t2 = is.argument2();


		BOOST_TYPENAME isection_traits<is_t<T1,T2,is_pl> >::minus_type minus =
			Union(isection(m1,t2),rsection(diff(m2,p2),t1));

		BOOST_TYPENAME isection_traits<is_t<T1,T2,is_pl> >::plus_type plus = 
			Union(Union(difference(isection(p1,t2),m2),intersection(p1,p2)),difference(difference(rsection(p2,t1),m1),m2));

		return BOOST_TYPENAME traits<is_t<T1,T2,is_pl> >::delta_type(plus,minus);
	}

	template<class T1, class T2>
	BOOST_TYPENAME traits<is_t<T1,T2,rs_pl> >::delta_type
	s_delta(const is_t<T1,T2,rs_pl>& is, const transaction& tr)
	{
		BOOST_TYPENAME isection_traits<is_t<T1,T2,rs_pl> >::min1 m1 = delta(is.argument1(), tr).second;
		BOOST_TYPENAME isection_traits<is_t<T1,T2,rs_pl> >::min2 m2 = delta(is.argument2(), tr).second;
		BOOST_TYPENAME isection_traits<is_t<T1,T2,rs_pl> >::pls1 p1 = delta(is.argument1(), tr).first;
		BOOST_TYPENAME isection_traits<is_t<T1,T2,rs_pl> >::pls2 p2 = delta(is.argument2(), tr).first;

		T1 t1 = is.argument1();
		T2 t2 = is.argument2();

		BOOST_TYPENAME traits<is_t<T1,T2,rs_pl> >::minus_type minus = 
		Union
		(
			intersection(m2,t1),
			intersection
			(
				t2,
				difference(m1,p1)
			)
		);

		BOOST_TYPENAME traits<is_t<T1,T2,rs_pl> >::plus_type plus =
		Union
		(
			intersection
			(
				p2,
				Union
				(
					difference
					(
						t1,
						m1
					),
					p1
				)
			),
			difference
			(
				difference
				(
					intersection
					(
						t2,
						p1
					),
					m2
				),
				m1
			)
		);

		return BOOST_TYPENAME traits<is_t<T1,T2,rs_pl> >::delta_type(plus,minus);
	}

	template<class T1, class T2>
	BOOST_TYPENAME traits<is_t<T1,T2,df_pl> >::delta_type
	m_delta(const is_t<T1,T2,df_pl>& df, const transaction& tr)
	{
		BOOST_TYPENAME diff_traits<is_t<T1,T2,df_pl> >::min1 m1 = delta(df.argument1(), tr).second; 
		BOOST_TYPENAME diff_traits<is_t<T1,T2,df_pl> >::min2 m2 = delta(df.argument2(), tr).second; 
		BOOST_TYPENAME diff_traits<is_t<T1,T2,df_pl> >::pls1 p1 = delta(df.argument1(), tr).first; 
		BOOST_TYPENAME diff_traits<is_t<T1,T2,df_pl> >::pls2 p2 = delta(df.argument2(), tr).first; 
		T1 t1 = df.argument1();
		T2 t2 = df.argument2();
		BOOST_TYPENAME diff_traits<is_t<T1,T2,df_pl> >::minus_type minus =
			diff(Union(m1,rsection(p2,t1)),t2);

		BOOST_TYPENAME diff_traits<is_t<T1,T2,df_pl> >::plus_type plus =
			diff(Union(rsection(m2,difference(t1,m1)),p1),Union(difference(t2,m2),p2));

		return BOOST_TYPENAME traits<is_t<T1,T2,df_pl> >::delta_type(plus,minus);
	}

	template<class I>
	BOOST_TYPENAME traits<I>::delta_type
	_delta(const is_pl*, const I& is, const transaction& tr)
	{
		return f_delta(is, tr);
	}

	template<class I>
	BOOST_TYPENAME traits<I>::delta_type
	_delta(const rs_pl*, const I& is, const transaction& tr)
	{
		return s_delta(is, tr);
	}

	template<class I>
	BOOST_TYPENAME traits<I>::delta_type
	_delta(const df_pl*, const I& is, const transaction& tr)
	{
		return m_delta(is, tr);
	}

	template<class T1, class T2, class IsP>
	BOOST_TYPENAME traits<is_t<T1, T2, IsP> >::delta_type
	_delta(const is_t<T1, T2, IsP>& is, const transaction& tr)
	{
		return _delta((IsP*)0, is, tr);
	}

}}

#endif//BOOST_RTL_ISECTION_DELTA_HPP_INCLUDED
