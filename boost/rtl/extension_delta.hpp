// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_EXTENSION_DELTA_HPP_INCLUDED
#define BOOST_RTL_EXTENSION_DELTA_HPP_INCLUDED

#include <boost/rtl/traits.hpp>
#include <boost/rtl/extension.hpp>

namespace boost { namespace rel {

	template<class R> 
		struct extension_traits
	{
		typedef ext_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::plus_type, BOOST_TYPENAME R::policy_type> plus_type;
		typedef ext_t<BOOST_TYPENAME traits<BOOST_TYPENAME R::arg_type>::minus_type, BOOST_TYPENAME R::policy_type> minus_type;
	};
	template<class Table, class Policy>
		BOOST_TYPENAME traits<ext_t<Table, Policy> >::delta_type 
		_delta(const ext_t<Table, Policy>& ext, const transaction& tr)
	{
		return BOOST_TYPENAME traits<ext_t<Table, Policy> >::delta_type(
			extension(delta(ext.argument(), tr).first, ext.policy()),
			extension(delta(ext.argument(), tr).second, ext.policy()));
	}
}}

#endif//BOOST_RTL_EXTENSION_DELTA_HPP_INCLUDED
