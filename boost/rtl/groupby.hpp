// Copyright (C) 2002 Arkadiy Vertleyb, Dmitriy Arapov
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_RTL_GROUPBY_HPP_INCLUDED
#define BOOST_RTL_GROUPBY_HPP_INCLUDED

#include "unary.hpp"
#include "list.hpp"
#include <boost/mpl/for_each.hpp>
#include "keys.hpp"

namespace rel
{
	// group_field

	template <class It, class TargetTuple> 
		class group_field
	{
	public:
		group_field(const It& it, TargetTuple& trg)
			: it_(it)
			, trg_(trg) 
		{}

		template <class T> 
			void operator()(const T&)
		{
			static_cast<BOOST_TYPENAME T::impl_type&>(trg_.fields_)(it_);
		}
	private:
		const It& it_;
		TargetTuple& trg_;
	};

	template <class FieldList, class It, class TargetTuple>
	void group_fields(const It& it, TargetTuple& trg)
	{
		boost::mpl::for_each<FieldList>(group_field<It, TargetTuple>(it, trg));
	}

	// groupby_iterator

	template <class Table, class GroupList, int PrefixSize> class gby_t;

	template <class Table, class GroupList, int PrefixSize> 
		class groupby_iterator : public unary_iterator
		<
			groupby_iterator<Table, GroupList, PrefixSize>, 
			gby_t<Table, GroupList, PrefixSize> 
		>
	{
		typedef unary_iterator
		<
			groupby_iterator<Table, GroupList, PrefixSize>, 
			gby_t<Table, GroupList, PrefixSize> 
		> base_type;

		friend class iterator_root<groupby_iterator<Table, GroupList, PrefixSize>, gby_t<Table, GroupList, PrefixSize> >;
		friend class gby_t<Table,GroupList,PrefixSize>;

	public:
		BOOST_RTL_PULL_UNARY_ITERATOR_TYPEDEFS(base_type);
        using base_type::itr_;
        using base_type::relation_;

		typedef typename list_prefix<typename Table::sort_list,PrefixSize>::type key_list;
		BOOST_STATIC_CONSTANT(int, prefix_size=PrefixSize);

		groupby_iterator(const arg_iterator& itr, const relation_type* o) 
			: base_type(itr, o) 
		{}

		groupby_iterator() 
		{} 

		template<class Column>
			typename Column::type
		get_internal(const Column&) const 
		{
			return BOOST_RTL_FIELD(result_,Column);
		}

	private:
		void make_step_forward() 
		{
			copy_fields<key_list>(*itr_, result_);
            arg_iterator end_arg = relation_->arg_.end();

			for (end_range_ = itr_; end_range_ != end_arg && 
				compare_tuples<key_list>(*end_range_, *itr_) == 0; ++end_range_)
			{
				group_fields<GroupList>(*end_range_, result_);
			}
		}

		void increment()
		{
            value_type temp;
			result_ = temp;

			itr_ = end_range_;
            arg_iterator end_arg = relation_->arg_.end();

            if (itr_ != end_arg) // DA
			{
				copy_fields<key_list>(*itr_, result_);

				for (end_range_ = itr_; end_range_ != end_arg && 
					compare_iterators<key_list>(end_range_, itr_) == 0; ++end_range_)
				{
					group_fields<GroupList>(end_range_, result_);
				}
			}
		}
		
		void decrement()
		{
            value_type temp;
			result_ = temp;

			end_range_ = itr_;
			arg_iterator prev(itr_);
			--prev;

			do
			{
				itr_ = prev;
				group_fields<GroupList>(*itr_, result_);
			}
			while (itr_ != relation_->arg_.begin() &&
				compare_tuples<key_list>(*itr_, *--prev) == 0);

			copy_fields<key_list>(*itr_, result_);
		}

	private:
		value_type result_;
		arg_iterator end_range_;
	};

	// gby_base

	template <class Table, class GroupList, int PrefixSize>
		struct gby_base : boost::mpl::identity<unary_operator<
			Table,
			BOOST_TYPENAME concat
			<
				typename list_prefix<typename Table::sort_list,PrefixSize>::type, 
				GroupList
			>::type,
			typename list_prefix<typename Table::sort_list,PrefixSize>::type,
			groupby_iterator<Table, GroupList, PrefixSize>
		> > {};

	// gby_t

	template <class Table, class GroupList, int PrefixSize>
		class gby_t : public gby_base<Table, GroupList, PrefixSize>::type
	{
		typedef typename gby_base<Table, GroupList, PrefixSize>::type base_type;
		friend class groupby_iterator<Table, GroupList, PrefixSize>;

	public:
		BOOST_RTL_PULL_UNARY_OPERATOR_TYPEDEFS(base_type);
        using base_type::arg_;
		typedef typename list_prefix<typename Table::sort_list, PrefixSize>::type key_list;
		typedef GroupList group_list;
        enum{prefix_size = PrefixSize};
		typedef boost::mpl::vector1<sort_list> keys; // all keys are discarded, except new sort_list

		gby_t(const arg_type& x) 
			: base_type(x) 
		{}

		const_iterator begin() const 
		{
			const_iterator temp(arg_.begin(),this);
			temp.make_step_forward();
			return temp;
		}

		const_iterator end() const
		{
			return const_iterator(arg_.end(), this);
		}

		template<class SubTuple>
			range_t<const_iterator> equal_range(const SubTuple& sub) const
		{
			const bool valid = valid_subtuple<SubTuple,sort_list>::value;
			BOOST_STATIC_ASSERT(valid);

			range_t<BOOST_TYPENAME Table::const_iterator> r(arg_.equal_range(sub));

			const_iterator lbound(r.begin().base(),this);
			const_iterator ubound(r.end().base(),this);
			lbound.make_step_forward();

			return range(lbound, ubound);
		}

		template<class SubTuple>
			const_iterator lower_bound(const SubTuple& sub) const
		{
			const bool valid = valid_subtuple<SubTuple,sort_list>::value;
			BOOST_STATIC_ASSERT(valid);

			const_iterator lbound(arg_.lower_bound(sub),this);
			lbound.make_step_forward();

			return lbound;
		}

		template<class SubTuple>
			const_iterator upper_bound(const SubTuple& sub) const
		{
			const bool valid = valid_subtuple<SubTuple,sort_list>::value;
			BOOST_STATIC_ASSERT(valid);

			return const_iterator(arg_.upper_bound(sub),this);
		}
	};

	// groupby
	template <class Table, class GroupList, int PrefixSize>
		struct groupby_t
	{
		typedef gby_t<Table, GroupList, PrefixSize> type;
	};

	template <int PrefixSize, class GroupList, class Table>
		BOOST_TYPENAME groupby_t<Table, GroupList, PrefixSize>::type 
	groupby(const Table& t)
	{
		return BOOST_TYPENAME groupby_t<Table, GroupList, PrefixSize>::type(t);
	}

}

#endif//BOOST_RTL_GROUPBY_HPP_INCLUDED
