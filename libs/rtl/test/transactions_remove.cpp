#include "pch.hpp"

#include "generic.hpp"
#include <iostream>
#include <set>
#include <vector>
#include <utility>
#include <boost/smart_ptr.hpp>
#include "utils.hpp"
#include "test_function.hpp"
#include "table_delta.hpp" 
#include "key_index_delta.hpp"
#include "iterator_index_delta.hpp"
#include "rename_delta.hpp"
#include "crossproduct_delta.hpp"
#include "selection_delta.hpp"
#include "projection_delta.hpp"
#include "cover_delta.hpp"
#include "merge_delta.hpp"
#include "zigzag_delta.hpp"
#include "range_selection_delta.hpp"
#include "groupby_delta.hpp"
#include "expression_registry.hpp"

using namespace unit;
using namespace rel;
using namespace std;
using namespace boost;

namespace trans_remove_test
{
	BOOST_RTL_DEFINE_COLUMN(int, c1);
	typedef table<table_info<mpl::vector1<c1> > > table_t;

	void foo()
	{
		table_t t;
		transaction tr;

		TESTASSERT(tr.insert(t, row<mpl::vector1<c1> >(1)));
		TESTASSERT(tr.insert(t, row<mpl::vector1<c1> >(2)));
		TESTASSERT(tr.insert(t, row<mpl::vector1<c1> >(3)));
		TESTASSERT(tr.insert(t, row<mpl::vector1<c1> >(4)));
		TESTASSERT(tr.insert(t, row<mpl::vector1<c1> >(5)));

		TESTASSERT(tr.remove(t, row<mpl::vector1<c1> >(3)));
		TESTASSERT(count(modified(t, tr)) == 4);

		tr.commit();

		TESTASSERT(tr.remove(t.begin()));
		TESTASSERT(count(modified(t, tr)) == 3);
	}
	test_function test("trans_remove_test", foo);
}
