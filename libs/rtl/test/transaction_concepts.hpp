#ifndef BOOST_RTL_TRANSACTION_CONCEPTS_HPP_INCLUDED
#define BOOST_RTL_TRANSACTION_CONCEPTS_HPP_INCLUDED

#include "rtl.hpp"
#include "test_function.hpp"
#include "invert.hpp"
#include "utils.hpp"
#include "key_index_delta.hpp"
#include "isection_delta.hpp"
#include "crossproduct_delta.hpp"
#include "zigzag_delta.hpp"
#include "table_delta.hpp"
#include "cover_delta.hpp"
#include "perfmeter.hpp"
#include "merge_delta.hpp"

namespace tc
{

template<class Table>
	class deleter
{
public:
	deleter(const Table& tab, rel::transaction& tr) : _tab(tab), _tr(tr) {};
	void operator()(const typename Table::value_type& v) 
	{
		_tr.remove(_tab, v);
	}
private:
	Table _tab;
	rel::transaction& _tr;
};

template<class Table>
	class inserter
{
public:
	inserter(const Table& tab, rel::transaction& tr) : _tab(tab), _tr(tr) {};
	void operator() (const typename Table::value_type& v)
	{
		_tr.insert(_tab,v);
	}
private:
	Table _tab;
	rel::transaction& _tr;
};

inline void print_performance(const std::string& tag)
{
	static int oldCopy = rel::Perfmeter::getCopyCount();
	static int oldComp = rel::Perfmeter::getCompCount();
	static int oldAsgn = rel::Perfmeter::getAsgnCount();
	static int oldDeft = rel::Perfmeter::getDeftCount();
	std::cout << tag << "==> copy = " << (rel::Perfmeter::getCopyCount() - oldCopy)
			<< " def ctors = " << (rel::Perfmeter::getDeftCount() - oldDeft)
			<< " comp = " << (rel::Perfmeter::getCompCount() - oldComp)
			<< " asgns = " << (rel::Perfmeter::getAsgnCount() - oldAsgn)
			<< std::endl;
	oldCopy = rel::Perfmeter::getCopyCount();
	oldComp = rel::Perfmeter::getCompCount();
	oldAsgn = rel::Perfmeter::getAsgnCount();
	oldDeft = rel::Perfmeter::getDeftCount();
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
		typedef BOOST_TYPENAME rel::key_index_t<R, BOOST_TYPENAME R::sort_list>::type Key_ix;
		typedef rel::table
		<
			rel::table_info
			<
				BOOST_TYPENAME R::field_list, 
				BOOST_TYPENAME R::sort_list, 
				BOOST_TYPENAME R::keys
			> 
		> r_table;

		// create stable copy of the relation r
		r_table inv(rel::invert<BOOST_TYPENAME R::sort_list, typename r_table::implementation>(r));

		Key_ix idx(r);
		rel::expression_registry exprs;
		exprs.add(idx);
		{
			rel::transaction fwd;
			inserter<BOOST_TYPENAME R::arg1_type> i1(r.argument1(),fwd);
			inserter<BOOST_TYPENAME R::arg2_type> i2(r.argument2(),fwd);
			deleter<BOOST_TYPENAME R::arg1_type>  d1(r.argument1(),fwd);
			deleter<BOOST_TYPENAME R::arg2_type>  d2(r.argument2(),fwd);
			std::for_each(min1.begin(),min1.end(),d1);
			std::for_each(pls1.begin(),pls1.end(),i1);
			std::for_each(min2.begin(),min2.end(),d2);
			std::for_each(pls2.begin(),pls2.end(),i2);
			TESTASSERT(rel::equal_tables(r,idx));
			TESTASSERT(rel::equal_tables(rel::delta(idx,fwd).second,mns));
//			rel::print(rel::delta(idx,fwd).first);
			TESTASSERT(rel::equal_tables(rel::delta(idx,fwd).first,pls));
			TESTASSERT(rel::equal_tables(rel::modified(idx,fwd),rel::modified(r,fwd)));
			fwd.commit(exprs);
			TESTASSERT(rel::equal_tables(idx, r));
		}
		{
			rel::transaction bkw;
			inserter<BOOST_TYPENAME R::arg1_type> i1(r.argument1(),bkw);
			inserter<BOOST_TYPENAME R::arg2_type> i2(r.argument2(),bkw);
			deleter<BOOST_TYPENAME R::arg1_type>  d1(r.argument1(),bkw);
			deleter<BOOST_TYPENAME R::arg2_type>  d2(r.argument2(),bkw);
			std::for_each(pls1.begin(),pls1.end(),d1);
			std::for_each(min1.begin(),min1.end(),i1);
			std::for_each(pls2.begin(),pls2.end(),d2);
			std::for_each(min2.begin(),min2.end(),i2);
			TESTASSERT(rel::equal_tables(r,idx));
			TESTASSERT(rel::equal_tables(rel::delta(idx,bkw).second,pls));
			TESTASSERT(rel::equal_tables(rel::delta(idx,bkw).first,mns));
			TESTASSERT(rel::equal_tables(rel::modified(idx,bkw),rel::modified(r,bkw)));
			bkw.commit(exprs);
			TESTASSERT(rel::equal_tables(idx, r));
		}
		TESTASSERT(rel::equal_tables(idx,inv)); // check if returned
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
		typedef rel::cover_t<BOOST_TYPENAME R::value_type, BOOST_TYPENAME R::sort_list, BOOST_TYPENAME R::keys> RC;
		RC r = rel::cover(rela);
		typedef BOOST_TYPENAME rel::key_index_t<RC, BOOST_TYPENAME RC::sort_list>::type Key_ix;
		typedef rel::table
		<
			rel::table_info
			<
				BOOST_TYPENAME RC::field_list, 
				BOOST_TYPENAME RC::sort_list,
				BOOST_TYPENAME RC::keys
			> 
		> r_table;

		// create stable copy of the relation r
		r_table inv(rel::invert<BOOST_TYPENAME RC::sort_list, typename r_table::implementation>(r));

		Key_ix idx(r);
		rel::expression_registry exprs;
		exprs.add(idx);
		{
			rel::transaction fwd;
			inserter<BOOST_TYPENAME R::arg1_type> i1(rela.argument1(),fwd);
			inserter<BOOST_TYPENAME R::arg2_type> i2(rela.argument2(),fwd);
			deleter<BOOST_TYPENAME R::arg1_type>  d1(rela.argument1(),fwd);
			deleter<BOOST_TYPENAME R::arg2_type>  d2(rela.argument2(),fwd);
			std::for_each(min1.begin(),min1.end(),d1);
			std::for_each(pls1.begin(),pls1.end(),i1);
			std::for_each(min2.begin(),min2.end(),d2);
			std::for_each(pls2.begin(),pls2.end(),i2);
			TESTASSERT(rel::equal_tables(r,idx));
			TESTASSERT(rel::equal_tables(rel::delta(idx,fwd).second,mns));
			TESTASSERT(rel::equal_tables(rel::delta(idx,fwd).first,pls));
			TESTASSERT(rel::equal_tables(rel::modified(idx,fwd),rel::modified(r,fwd)));
			fwd.commit(exprs);
			TESTASSERT(rel::equal_tables(idx, r));
		}
		{
			rel::transaction bkw;
			inserter<BOOST_TYPENAME R::arg1_type> i1(rela.argument1(),bkw);
			inserter<BOOST_TYPENAME R::arg2_type> i2(rela.argument2(),bkw);
			deleter<BOOST_TYPENAME R::arg1_type>  d1(rela.argument1(),bkw);
			deleter<BOOST_TYPENAME R::arg2_type>  d2(rela.argument2(),bkw);
			std::for_each(pls1.begin(),pls1.end(),d1);
			std::for_each(min1.begin(),min1.end(),i1);
			std::for_each(pls2.begin(),pls2.end(),d2);
			std::for_each(min2.begin(),min2.end(),i2);
			TESTASSERT(rel::equal_tables(r,idx));
			TESTASSERT(rel::equal_tables(rel::delta(idx,bkw).second,pls));
			TESTASSERT(rel::equal_tables(rel::delta(idx,bkw).first,mns));
			TESTASSERT(rel::equal_tables(rel::modified(idx,bkw),rel::modified(r,bkw)));
			bkw.commit(exprs);
			TESTASSERT(rel::equal_tables(idx, r));
		}
		TESTASSERT(rel::equal_tables(idx,inv)); // check if returned
	}

};

}

#endif//BOOST_RTL_TRANSACTION_CONCEPTS_HPP_INCLUDED

