#ifndef BOOST_RTL_TRANSACTION_CONCEPTS_HPP_INCLUDED
#define BOOST_RTL_TRANSACTION_CONCEPTS_HPP_INCLUDED

#include <boost/rtl/rtl.hpp>
#include "test_function.hpp"
#include <boost/rtl/invert.hpp>
#include <boost/rtl/utils.hpp>
#include <boost/rtl/key_index_delta.hpp>
#include <boost/rtl/isection_delta.hpp>
#include <boost/rtl/crossproduct_delta.hpp>
#include <boost/rtl/zigzag_delta.hpp>
#include <boost/rtl/table_delta.hpp>
#include <boost/rtl/cover_delta.hpp>
#include "perfmeter.hpp"
#include <boost/rtl/merge_delta.hpp>

namespace tc
{

template<class Table>
	class deleter
{
public:
	deleter(const Table& tab, boost::rel::transaction& tr) : _tab(tab), _tr(tr) {};
	void operator()(const typename Table::value_type& v) 
	{
		_tr.remove(_tab, v);
	}
private:
	Table _tab;
	boost::rel::transaction& _tr;
};

template<class Table>
	class inserter
{
public:
	inserter(const Table& tab, boost::rel::transaction& tr) : _tab(tab), _tr(tr) {};
	void operator() (const typename Table::value_type& v)
	{
		_tr.insert(_tab,v);
	}
private:
	Table _tab;
	boost::rel::transaction& _tr;
};

inline void print_performance(const std::string& tag)
{
	static int oldCopy = Perfmeter::getCopyCount();
	static int oldComp = Perfmeter::getCompCount();
	static int oldAsgn = Perfmeter::getAsgnCount();
	static int oldDeft = Perfmeter::getDeftCount();
	std::cout << tag << "==> copy = " << (Perfmeter::getCopyCount() - oldCopy)
			<< " def ctors = " << (Perfmeter::getDeftCount() - oldDeft)
			<< " comp = " << (Perfmeter::getCompCount() - oldComp)
			<< " asgns = " << (Perfmeter::getAsgnCount() - oldAsgn)
			<< std::endl;
	oldCopy = Perfmeter::getCopyCount();
	oldComp = Perfmeter::getCompCount();
	oldAsgn = Perfmeter::getAsgnCount();
	oldDeft = Perfmeter::getDeftCount();
}

template<class R, class Minus, class Plus>
struct simple_checks
{
	static 
	void 
	perform_binary_test
	(
		R& r, 
		const typename R::arg1_type& min1,   
		const typename R::arg2_type& min2, 
		const typename R::arg1_type& pls1, 
		const typename R::arg2_type& pls2,
		const Minus& mns,
		const Plus& pls
	)
	{
		typedef BOOST_TYPENAME boost::rel::key_index_t<R, BOOST_TYPENAME R::sort_list>::type Key_ix;
		typedef boost::rel::table
		<
			boost::rel::table_info
			<
				BOOST_TYPENAME R::field_list, 
				BOOST_TYPENAME R::sort_list, 
				BOOST_TYPENAME R::keys
			> 
		> r_table;

		// create stable copy of the relation r
		r_table inv(boost::rel::invert<BOOST_TYPENAME R::sort_list, typename r_table::implementation>(r));

		Key_ix idx(r);
		boost::rel::expression_registry exprs;
		exprs.add(idx);
		{
			boost::rel::transaction fwd;
			inserter<BOOST_TYPENAME R::arg1_type> i1(r.argument1(),fwd);
			inserter<BOOST_TYPENAME R::arg2_type> i2(r.argument2(),fwd);
			deleter<BOOST_TYPENAME R::arg1_type>  d1(r.argument1(),fwd);
			deleter<BOOST_TYPENAME R::arg2_type>  d2(r.argument2(),fwd);
			std::for_each(min1.begin(),min1.end(),d1);
			std::for_each(pls1.begin(),pls1.end(),i1);
			std::for_each(min2.begin(),min2.end(),d2);
			std::for_each(pls2.begin(),pls2.end(),i2);
			TESTASSERT(boost::rel::equal_tables(r,idx));
			TESTASSERT(boost::rel::equal_tables(boost::rel::delta(idx,fwd).second,mns));
//			boost::rel::print(boost::rel::delta(idx,fwd).first);
			TESTASSERT(boost::rel::equal_tables(boost::rel::delta(idx,fwd).first,pls));
			TESTASSERT(boost::rel::equal_tables(boost::rel::modified(idx,fwd),boost::rel::modified(r,fwd)));
			fwd.commit(exprs);
			TESTASSERT(boost::rel::equal_tables(idx, r));
		}
		{
			boost::rel::transaction bkw;
			inserter<BOOST_TYPENAME R::arg1_type> i1(r.argument1(),bkw);
			inserter<BOOST_TYPENAME R::arg2_type> i2(r.argument2(),bkw);
			deleter<BOOST_TYPENAME R::arg1_type>  d1(r.argument1(),bkw);
			deleter<BOOST_TYPENAME R::arg2_type>  d2(r.argument2(),bkw);
			std::for_each(pls1.begin(),pls1.end(),d1);
			std::for_each(min1.begin(),min1.end(),i1);
			std::for_each(pls2.begin(),pls2.end(),d2);
			std::for_each(min2.begin(),min2.end(),i2);
			TESTASSERT(boost::rel::equal_tables(r,idx));
			TESTASSERT(boost::rel::equal_tables(boost::rel::delta(idx,bkw).second,pls));
			TESTASSERT(boost::rel::equal_tables(boost::rel::delta(idx,bkw).first,mns));
			TESTASSERT(boost::rel::equal_tables(boost::rel::modified(idx,bkw),boost::rel::modified(r,bkw)));
			bkw.commit(exprs);
			TESTASSERT(boost::rel::equal_tables(idx, r));
		}
		TESTASSERT(boost::rel::equal_tables(idx,inv)); // check if returned
	}

	static 
	void
	perform_cover_test
	(
		R& rela, 
		const typename R::arg1_type& min1,   
		const typename R::arg2_type& min2, 
		const typename R::arg1_type& pls1, 
		const typename R::arg2_type& pls2,
		const Minus& mns,
		const Plus& pls
	)
	{
		typedef boost::rel::cover_t<BOOST_TYPENAME R::value_type, BOOST_TYPENAME R::sort_list, BOOST_TYPENAME R::keys> RC;
		RC r = boost::rel::cover(rela);
		typedef BOOST_TYPENAME boost::rel::key_index_t<RC, BOOST_TYPENAME RC::sort_list>::type Key_ix;
		typedef boost::rel::table
		<
			boost::rel::table_info
			<
				BOOST_TYPENAME RC::field_list, 
				BOOST_TYPENAME RC::sort_list,
				BOOST_TYPENAME RC::keys
			> 
		> r_table;

		// create stable copy of the relation r
		r_table inv(boost::rel::invert<BOOST_TYPENAME RC::sort_list, typename r_table::implementation>(r));

		Key_ix idx(r);
		boost::rel::expression_registry exprs;
		exprs.add(idx);
		{
			boost::rel::transaction fwd;
			inserter<BOOST_TYPENAME R::arg1_type> i1(rela.argument1(),fwd);
			inserter<BOOST_TYPENAME R::arg2_type> i2(rela.argument2(),fwd);
			deleter<BOOST_TYPENAME R::arg1_type>  d1(rela.argument1(),fwd);
			deleter<BOOST_TYPENAME R::arg2_type>  d2(rela.argument2(),fwd);
			std::for_each(min1.begin(),min1.end(),d1);
			std::for_each(pls1.begin(),pls1.end(),i1);
			std::for_each(min2.begin(),min2.end(),d2);
			std::for_each(pls2.begin(),pls2.end(),i2);
			TESTASSERT(boost::rel::equal_tables(r,idx));
			TESTASSERT(boost::rel::equal_tables(boost::rel::delta(idx,fwd).second,mns));
			TESTASSERT(boost::rel::equal_tables(boost::rel::delta(idx,fwd).first,pls));
			TESTASSERT(boost::rel::equal_tables(boost::rel::modified(idx,fwd),boost::rel::modified(r,fwd)));
			fwd.commit(exprs);
			TESTASSERT(boost::rel::equal_tables(idx, r));
		}
		{
			boost::rel::transaction bkw;
			inserter<BOOST_TYPENAME R::arg1_type> i1(rela.argument1(),bkw);
			inserter<BOOST_TYPENAME R::arg2_type> i2(rela.argument2(),bkw);
			deleter<BOOST_TYPENAME R::arg1_type>  d1(rela.argument1(),bkw);
			deleter<BOOST_TYPENAME R::arg2_type>  d2(rela.argument2(),bkw);
			std::for_each(pls1.begin(),pls1.end(),d1);
			std::for_each(min1.begin(),min1.end(),i1);
			std::for_each(pls2.begin(),pls2.end(),d2);
			std::for_each(min2.begin(),min2.end(),i2);
			TESTASSERT(boost::rel::equal_tables(r,idx));
			TESTASSERT(boost::rel::equal_tables(boost::rel::delta(idx,bkw).second,pls));
			TESTASSERT(boost::rel::equal_tables(boost::rel::delta(idx,bkw).first,mns));
			TESTASSERT(boost::rel::equal_tables(boost::rel::modified(idx,bkw),boost::rel::modified(r,bkw)));
			bkw.commit(exprs);
			TESTASSERT(boost::rel::equal_tables(idx, r));
		}
		TESTASSERT(boost::rel::equal_tables(idx,inv)); // check if returned
	}

};

}

#endif//BOOST_RTL_TRANSACTION_CONCEPTS_HPP_INCLUDED

