// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_TRAITS_HPP_INCLUDED
#define BOOST_RTL_TRAITS_HPP_INCLUDED

#include "union.hpp"
#include "difference.hpp"
#include "intersection.hpp"
#include "isection.hpp"

namespace rel
{
	template<class Info> class table;
	template<class T> struct table_traits;

	template<class T, class SL, class Str> class key_idx_t;
	template<class T> struct key_index_traits;

	template<class T, class SL, class Str> class it_idx_t;
	template<class T> struct iterator_index_traits;

	template<class T, class Str> class ix_t;
	template<class T> struct partial_index_traits;

	template<class T, class Keys> class k_t;
	template<class T> struct key_traits;

	template<class T, class P> class cnv_t;
	template<class T> struct cnv_traits;

	template<class T, class FieldList> class dprj_t; 
	template<class T> struct dprj_traits;

	template<class T, class Pred> class sel_t;
	template<class T> struct selection_traits;

	template<class T, class P> class join_t;
	template<class T> struct join_traits;

	template<class T1, class T2, int PrefixSize> class mg_t;
	template<class T> struct merge_traits;

	template<class T, class GroupList, int PrefixSize> class gby_t;
	template<class T> struct groupby_traits;

	template<class Tp, class SortList, class Keys> class cover_t;
	template<class T> struct cover_traits;

	template<class T1, class T2, class P> class zigzag;
	template<class T> struct zigzag_traits;

	template<class T, class Policy> class ext_t;
	template<class T> struct extension_traits;

	template<class T1, class T2, class IsPolicy> class is_t;
	template<class T> struct is_traits;
	template<class T> struct isection_traits;
	template<class T> struct rsection_traits;	
	template<class T> struct diff_traits;

	template<class T> class ch_t;
	template<class T> struct cache_traits;

	template<class Table, class Begin, class End> class rsel_t;
	template<class T> struct range_selection_traits;
	template<class Table, class Sub> class esel_t;
	template<class T> struct equal_selection_traits;

	template<class T1, class T2, class Begin, class End> class rj_t;
	template<class T> struct range_join_traits;

	template<class R> struct traits;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

	template<int n> class sizer
	{
		char _[n];
	};
	template<class R> struct traits
	{
		typedef enum 
		{
			table_en = 1,
			key_index_en,
			cross_product_en,
			cnv_en,
			dprj_en,
			iterator_index_en,
			selection_en,
			join_en,
			merge_en,
			is_en,
			groupby_en,
			cover_en,
			zigzag_en,
			cache_en,
			partial_index_en,
			range_selection_en,
			equal_selection_en,
			key_en,
			ext_en
		};

		template<int n> struct s;

		// table
		template<> struct s<sizeof(sizer<table_en>)>
		{
			typedef table_traits<R> traits_type;
		};
		template<class Info>
			static sizer<table_en> redirector(table<Info>*); 

		// key_index
		template<> struct s<sizeof(sizer<key_index_en>)>
		{
			typedef key_index_traits<R> traits_type;
		};
		template<class Tb, class SL, class Str>
			static sizer<key_index_en> redirector(key_idx_t<Tb, SL, Str>*);

		// partial_index
		template<> struct s<sizeof(sizer<partial_index_en>)>
		{
			typedef partial_index_traits<R> traits_type;
		};
		template<class Tb, class Str>
			static sizer<partial_index_en> redirector(ix_t<Tb,Str>*);
		
		// auto and manual renames
		template<> struct s<sizeof(sizer<cnv_en>)>
		{	
			typedef cnv_traits<R> traits_type;
		};
		template<class Table, class Pairs>
			static sizer<cnv_en> redirector(cnv_t<Table,Pairs>*);

		// iterator_index
		template<> struct s<sizeof(sizer<iterator_index_en>)>
		{
			typedef iterator_index_traits<R> traits_type;
		};
		template<class Tb, class SL, class Str>
			static sizer<iterator_index_en> redirector(it_idx_t<Tb, SL, Str>*);

		// selection
		template<> struct s<sizeof(sizer<selection_en>)>
		{
			typedef selection_traits<R> traits_type;
		};
		template<class Tb, class Pred>
			static sizer<selection_en> redirector(sel_t<Tb, Pred>*);

		// join
		template<> struct s<sizeof(sizer<join_en>)>
		{
			typedef join_traits<R> traits_type;
		};
		template<class Tb, class Policy>
			static sizer<join_en> redirector(join_t<Tb, Policy>*);

		template<> struct s<sizeof(sizer<dprj_en>)>
		{
			typedef dprj_traits<R> traits_type;
		};
		template<class Table, class FieldList>
			static sizer<dprj_en> redirector(dprj_t<Table,FieldList>*);

		// merge
		template<> struct s<sizeof(sizer<merge_en>)>
		{
			typedef merge_traits<R> traits_type;
		};
		template<class T1, class T2, int PS>
			static sizer<merge_en> redirector(mg_t<T1,T2,PS>*);

		// zigzag
		template<> struct s<sizeof(sizer<zigzag_en>)>
		{
			typedef zigzag_traits<R> traits_type;
		};
		template<class T1, class T2, class P>
			static sizer<zigzag_en> redirector(zigzag<T1, T2, P>*);

		// isection/rsection/diff
		template<> struct s<sizeof(sizer<is_en>)>
		{
			typedef is_traits<R> traits_type;
		};
		template<class T1, class T2, class IsPolicy>
			static sizer<is_en> redirector(is_t<T1,T2,IsPolicy>*);

		// groupby
		template<> struct s<sizeof(sizer<groupby_en>)>
		{
			typedef groupby_traits<R> traits_type;
		};

		template<class Tb, class GL, int PS>
			static sizer<groupby_en> redirector(gby_t<Tb,GL,PS>*);	

		// cover
		template<> struct s<sizeof(sizer<cover_en>)>
		{
			typedef cover_traits<R> traits_type;
		};
		
		template<class Tuple, class SL, class Keys>
			static sizer<cover_en> redirector(cover_t<Tuple,SL,Keys>*);

		// cache
		template<> struct s<sizeof(sizer<cache_en>)>
		{
			typedef cache_traits<R> traits_type;
		};

		template<class Table>
			static sizer<cache_en> redirector(ch_t<Table>*);

		// range_selection
		template<> struct s<sizeof(sizer<range_selection_en>)>
		{
			typedef range_selection_traits<R> traits_type;
		};

		template<class T, class Begin, class End>
			static sizer<range_selection_en> redirector(rsel_t<T, Begin, End>*);

		// equal_selection
		template<> struct s<sizeof(sizer<equal_selection_en>)>
		{
			typedef equal_selection_traits<R> traits_type;
		};

		template<class T, class Sub>
			static sizer<equal_selection_en> redirector(esel_t<T, Sub>*);

		// key
		template<> struct s<sizeof(sizer<key_en>)>
		{
			typedef key_traits<R> traits_type;
		};

		template<class T, class Keys>
			static sizer<key_en> redirector(k_t<T,Keys>*);

		// extension
		template<> struct s<sizeof(sizer<ext_en>)>
		{
			typedef extension_traits<R> traits_type;		
		};

		template<class T, class Policy>
			static sizer<ext_en> redirector(ext_t<T,Policy>*);

		enum {n = sizeof(redirector((R*)0))};

		typedef s<n>::traits_type::plus_type plus_type;
		typedef s<n>::traits_type::minus_type minus_type;
		typedef std::pair<plus_type, minus_type> delta_type;
		typedef union_t<difference_t<R, minus_type>::type, plus_type>::type modified_type;
	};

#else

	template<class R> struct _traits;

	template<class T> struct intersection_traits;
	template<class T> struct union_traits;
	template<class T> struct difference_traits;

	template<class I> struct isection_traits;
	template<class I> struct rsection_traits;

	struct df_pl;
	struct is_pl;
	struct rs_pl;

	// table
	template<class Info> 
		struct _traits<table<Info> >
	{
		typedef table_traits<table<Info> > traits_type;
	};
	// key_index
	template<class Tb, class SL, class Str>
		struct _traits<key_idx_t<Tb, SL, Str> >
	{
		typedef key_index_traits<key_idx_t<Tb, SL, Str> > traits_type;
	};
	// partial_index
	template<class Tb, class Str>
		struct _traits<ix_t<Tb,Str> >
	{
		typedef partial_index_traits<ix_t<Tb,Str> > traits_type;
	};

	// auto & manual rename
	template<class Table, class Pairs>
		struct _traits<cnv_t<Table,Pairs> >
	{
		typedef cnv_traits<cnv_t<Table,Pairs> > traits_type;
	};

	template<class Table, class FieldList>
		struct _traits<dprj_t<Table, FieldList> >
	{
		typedef dprj_traits<dprj_t<Table, FieldList> > traits_type;
	};

	// iterator_index
	template<class Tb, class SL, class Str>
		struct _traits<it_idx_t<Tb, SL, Str> >
	{
		typedef iterator_index_traits<it_idx_t<Tb, SL, Str> > traits_type;
	};
	// selection
	template<class Tb, class Pred>
		struct _traits<sel_t<Tb, Pred> >
	{
		typedef selection_traits<sel_t<Tb, Pred> > traits_type;
	};
	// join
	template<class Tb, class Policy>
		struct _traits<join_t<Tb, Policy> >
	{
		typedef join_traits<join_t<Tb, Policy> > traits_type;
	};
	// range_selection
	template<class T, class Begin, class End>
		struct _traits<rsel_t<T, Begin, End> >
	{
		typedef range_selection_traits<rsel_t<T, Begin, End> > traits_type;
	};
	// equal_selection
	template<class T, class Sel>
		struct _traits<esel_t<T, Sel> >
	{
		typedef equal_selection_traits<esel_t<T, Sel> > traits_type;
	};
	// merge
	template<class T1, class T2, int PS>
		struct _traits<mg_t<T1,T2,PS> > 
	{
		typedef merge_traits<mg_t<T1,T2,PS> > traits_type;
	};

	// union
	template<class T1, class T2>
		struct _traits<zigzag<T1, T2, u_pl> >
	{
		typedef union_traits<typename union_t<T1,T2>::type> traits_type;
	};

	// difference
	template<class T1, class T2>
		struct _traits<zigzag<T1, T2, d_pl> >
	{
		typedef difference_traits<typename difference_t<T1,T2>::type> traits_type;
	};

	// intersection
	template<class T1, class T2>
		struct _traits<zigzag<T1, T2, i_pl> >
	{
		typedef intersection_traits<typename intersection_t<T1,T2>::type> traits_type;
	};

	// diff
	template<class T1, class T2>
		struct _traits<is_t<T1,T2,df_pl> >
	{
		typedef diff_traits<typename diff_t<T1,T2>::type> traits_type;
	};

	// isection
	template<class T1, class T2>
		struct _traits<is_t<T1,T2,is_pl> >
	{
		typedef isection_traits<typename isection_t<T1,T2>::type> traits_type;
	};

	// rsection
	template<class T1, class T2>
		struct _traits<is_t<T1,T2,rs_pl> >
	{
		typedef rsection_traits<typename rsection_t<T1,T2>::type> traits_type;
	};

	// groupby
	template<class Tb, class GL, int PS>
		struct _traits<gby_t<Tb,GL,PS> >
	{
		typedef groupby_traits<gby_t<Tb,GL,PS> > traits_type;
	};

	// cover
	template<class Tuple, class SL, class Keys>
		struct _traits<cover_t<Tuple,SL,Keys> >
	{
		typedef cover_traits<cover_t<Tuple,SL,Keys> > traits_type;
	};

	// cache
	template<class Table>
		struct _traits<ch_t<Table> >
	{
		typedef cache_traits<ch_t<Table> > traits_type;
	};

	// key
	template<class Table, class Keys>
		struct _traits<k_t<Table,Keys> >
	{
		typedef key_traits<k_t<Table,Keys> > traits_type;
	};

	// extension
	template<class Table, class Policy>
		struct _traits<ext_t<Table, Policy> >
	{
		typedef extension_traits<ext_t<Table, Policy> > traits_type;
	};

	template<class T1, class T2, class Begin, class End> 
		struct _traits<rj_t<T1, T2, Begin, End> >
	{
		typedef range_join_traits<rj_t<T1, T2, Begin, End> > traits_type;
	};
	
	template<class R> struct traits
	{
		typedef typename _traits<R>::traits_type tr;
		typedef typename tr::plus_type plus_type;
		typedef typename tr::minus_type minus_type;
		typedef std::pair<plus_type, minus_type> delta_type;
		typedef typename union_t<typename difference_t<R, minus_type>::type, plus_type>::type modified_type;
	};

#endif

	class transaction;

	template<class R>
		BOOST_TYPENAME traits<R>::delta_type 
		delta(const R& r, const transaction& tr)
	{
		return _delta(r, tr);
	}
	template<class R>
		BOOST_TYPENAME traits<R>::modified_type 
		modified(const R& r, const transaction& tr)
	{
		return Union(difference(r, delta(r, tr).second), delta(r, tr).first);
	}
}

#endif//BOOST_RTL_TRAITS_HPP_INCLUDED

