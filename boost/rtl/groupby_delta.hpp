// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_GROUPBY_DELTA_HPP_INCLUDED
#define BOOST_RTL_GROUPBY_DELTA_HPP_INCLUDED

#include "traits.hpp"
#include "groupby.hpp"
#include "range_join.hpp"
#include "projection.hpp"
#include "rename.hpp"
#include "list.hpp"

namespace rel
{
	template<class R> struct groupby_traits
	{
		// affected groups

		typedef typename projection_t<
			typename union_t<
				typename traits<typename R::arg_type>::plus_type, 
				typename traits<typename R::arg_type>::minus_type
			>::type, 
			typename R::key_list
		>::type affected_groups_t;

		typedef typename rename_pairs<
			typename R::key_list
		>::type rename_key_list;

		// plus/minus

		typedef typename groupby_t<
			typename equal_join_t<
				affected_groups_t,
				typename manual_rename_t<typename R::arg_type, rename_key_list>::type,
				typename R::key_list
			>::type,
			typename R::group_list,
			R::prefix_size
		>::type minus_type;

		typedef typename groupby_t<
			typename equal_join_t<
				affected_groups_t,
				typename manual_rename_t<typename traits<typename R::arg_type>::modified_type, rename_key_list>::type,
				typename R::key_list
			>::type,
			typename R::group_list,
			R::prefix_size
		>::type plus_type;
	};

	template<class Table, class GroupList, int PrefixSize>
		typename traits<gby_t<Table, GroupList, PrefixSize> >::delta_type
		_delta(const gby_t<Table, GroupList, PrefixSize>& gby, const transaction& tr)
	{
		typedef gby_t<Table, GroupList, PrefixSize> R;

		typename traits<Table>::delta_type arg_delta = 
			delta(gby.argument(), tr);

		typename groupby_traits<R>::affected_groups_t affected_groups = 
            projection<typename R::key_list>(Union(arg_delta.first, arg_delta.second));

		typename groupby_traits<R>::minus_type minus =
			groupby<PrefixSize, GroupList>(
				equal_join<typename R::key_list>(
					affected_groups, 
					manual_rename<typename groupby_traits<R>::rename_key_list>(gby.argument())
				)
			);

		typename groupby_traits<R>::plus_type plus =
			groupby<PrefixSize, GroupList>(
				equal_join<typename R::key_list>(
					affected_groups, 
					manual_rename<typename groupby_traits<R>::rename_key_list>(modified(gby.argument(), tr))
				)
			);

		return std::make_pair(plus, minus);
	}
}

#endif//BOOST_RTL_GROUPBY_DELTA_HPP_INCLUDED
