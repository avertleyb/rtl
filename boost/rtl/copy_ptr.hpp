// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RTL_COPY_PTR_HPP_INCLUDED
#define BOOST_RTL_COPY_PTR_HPP_INCLUDED

namespace boost { namespace rel {

	template<bool P>
	struct copy_ptr_helper
	{
		template<class T>
			static T* do_copy(const T* p) {return new T(*p);}
	};

	template<>
	struct copy_ptr_helper<true>
	{
		template<class T>
			static T* do_copy(const T* p) {return p->clone();}
	};

	template<class T, bool Polymorph = false> class copy_ptr
	{
	public:
		copy_ptr(T* p = 0) : ptr(p)
		{
		}
		copy_ptr(const copy_ptr& rhs) : ptr(0)
		{
			if (rhs.ptr != 0)
				ptr = copy_ptr_helper<Polymorph>::do_copy(rhs.ptr);
		}
		~copy_ptr()
		{
			reset();
		}
		copy_ptr& operator=(const copy_ptr& rhs)
		{
			if (rhs.ptr == 0)
				reset();
			else
				reset(copy_ptr_helper<Polymorph>::do_copy(rhs.ptr));

			return *this;
		}
		T& operator*()
		{
			return *ptr;
		}
		const T& operator*() const
		{
			return *ptr;
		}
		T* get()
		{
			return ptr;
		}
		const T* get() const
		{
			return ptr;
		}
		T* operator->()
		{
			return ptr;
		}
		const T* operator->() const
		{
			return ptr;
		}
		void reset(T* p = 0)
		{
			if (ptr != p)
			{
				delete ptr;
				ptr = p;
			}
		}
	private:
		T* ptr;
	};
}}

#endif //BOOST_RTL_COPY_PTR_HPP_INCLUDED

