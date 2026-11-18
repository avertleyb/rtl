// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RTL_TRAVERSE_HPP_INCLUDED
#define BOOST_RTL_TRAVERSE_HPP_INCLUDED

namespace boost { namespace rel {

	template<class T, class F> 
		void traverse(T t, F f)
	{
        typename T::tag tag;
		f(t, tag);

		t.traverse_args(f);
	}
}}

#endif//BOOST_RTL_TRAVERSE_HPP_INCLUDED
