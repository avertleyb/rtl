#include "pch.hpp"

#include "generic.hpp"
#include "test_function.hpp"
#include "test_tables.hpp"
#include "crossproduct.hpp"
#include "utils.hpp"
#include "test_decrement.hpp"

namespace crossproduct_cpp
{
	using namespace std;
	using namespace rel;
	using namespace unit;
	using namespace boost;

	void _test()
	{
		staff::table t1;
		country::table t2;
		// print(cross_product(t1, t2)); 
		TESTASSERT(count(cross_product(t1, t2)) == 12);

		table<table_info<mpl::vector0<> > > null_table;
		TESTASSERT(count(cross_product(t1, null_table)) == 0);
		
		test_decrement(cross_product(t1, t2));
	}

	test_function test("CrossProductTest", _test);
}
