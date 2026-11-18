// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RTL_RENAME_DELTA_HPP_INCLUDED
#define BOOST_RTL_RENAME_DELTA_HPP_INCLUDED

#include <boost/rtl/traits.hpp>
#include <boost/rtl/rename.hpp>

namespace boost { namespace rel {

	template<class R> 
    struct traits_impl<R, conversion_tag>
	{
		typedef cnv_t<typename traits<typename R::arg_type>::plus_type, typename R::pairs> plus_type;
		typedef cnv_t<typename traits<typename R::arg_type>::minus_type, typename R::pairs> minus_type;
	};

	template<class R>
	typename traits<R>::delta_type delta(const R& cnv, const transaction& tr, conversion_tag)
	{
        typename traits<R>::delta_type d(delta(cnv.argument(), tr));
		typename traits<R>::plus_type plus(d.first);
		typename traits<R>::minus_type minus(d.second);
        return std::make_pair(plus,minus);
	};

}}

#endif//BOOST_RTL_RENAME_DELTA_HPP_INCLUDED 
