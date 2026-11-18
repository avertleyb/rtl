// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RTL_IDENTIFIABLE_HPP_INCLUDED
#define BOOST_RTL_IDENTIFIABLE_HPP_INCLUDED

#include <boost/smart_ptr.hpp>
#include <cstddef>

namespace boost { namespace rel {

	class identifiable
	{
	public:
		identifiable()
			: p(new char)
		{}
		ptrdiff_t id() const
		{
			return p.get() - (char*)0;
		}
	private:
		boost::shared_ptr<char> p;
	};

	struct id_less
	{
		template<class Ptr>
			bool operator()(Ptr p1, Ptr p2) const
		{
			return p1->id() < p2->id();
		}
	};

}}

#endif//BOOST_RTL_IDENTIFIABLE_HPP_INCLUDED
