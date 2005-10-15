#include "pch.hpp"

#include "generic.hpp"
#include "test_function.hpp"
#include "table.hpp"
#include "cache_delta.hpp"
#include "transaction.hpp"
#include "expression_registry.hpp"

using namespace rel;
using namespace boost;

namespace che
{
	BOOST_RTL_DEFINE_COLUMN(int, c1);
	
	typedef table_info<
		mpl::vector<c1>
		> my_info;

	void foo()
	{
		table<my_info> t;
		cache_t<table<my_info> >::type c(t);
		
		expression_registry exprs;
		exprs.add(c);

		for (int i = 0; i < 10; ++i)
			t.insert(table<my_info>::value_type(i));

		c.begin();
		c.end();

		transaction tr;
		tr.insert(t, table<my_info>::value_type(-1));
		tr.insert(t, table<my_info>::value_type(10));
		tr.commit(exprs);

		print(t);
		print(c);

		TESTASSERT(equal_tables(c, t));
	}
	unit::test_function _("cache", foo);
}
