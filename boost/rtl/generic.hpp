// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RTL_GENERIC_HPP_INCLUDED
#define BOOST_RTL_GENERIC_HPP_INCLUDED

// This file lists all headers we need from STL and BOOST 
// for core library

// STL

#include <cassert>

// BOOST

#include <boost/config.hpp>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/smart_ptr.hpp>

namespace boost { namespace rel {

	template<bool x, bool y> 
		struct static_and
	{
		BOOST_STATIC_CONSTANT(bool, value = (!x) ? false : y);
	};

	template<bool x, bool y> 
		struct static_or
	{
		BOOST_STATIC_CONSTANT(bool, value = x ? true : y);
	};

	struct deleter
	{
		template<class T> 
			void operator()(T* p) const
		{
			delete p;
		}
	};

	struct null_type;
}}

#endif//BOOST_RTL_GENERIC_HPP_INCLUDED
