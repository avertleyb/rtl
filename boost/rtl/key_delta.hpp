// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RTL_KEY_DELTA_HPP_INCLUDED
#define BOOST_RTL_KEY_DELTA_HPP_INCLUDED

#include <boost/rtl/key.hpp>

namespace boost { namespace rel {

	template<class R> 
    struct traits_impl<R, key_tag>
	{
        typedef typename key_t<typename traits<typename R::arg_type>::plus_type, typename R::key_type>::type plus_type;
        typedef typename key_t<typename traits<typename R::arg_type>::minus_type, typename R::key_type>::type minus_type;
	};

	template<class R>
	typename traits<R>::delta_type delta(const R& k, const transaction& tr, key_tag)
	{
        typename traits<typename R::arg_type>::delta_type d = delta(k.argument(), tr);

        typename traits<R>::plus_type plus(d.first);
        typename traits<R>::minus_type minus(d.second);

        return std::make_pair(plus, minus);
	};

}}

#endif

