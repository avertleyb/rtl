#include "pch.hpp"

#include "generic.hpp"
#include "test.hpp"
#include "crossproduct.hpp"
#include "selection.hpp"
#include "select_functors.hpp"
#include "test_tables.hpp"
#include "utils.hpp"
#include <functional>

namespace my_test_cpp
{
	using namespace rel;
	using namespace unit;
	using namespace boost;
	using namespace lambda;

	struct MyTest : public test
	{
		MyTest() : test("MyTest") {}
		virtual void run();
	};

	void MyTest::run()
	{
		staff::table t1;
		country::table t2;
/*
		print(
			projection<mpl::vector2<staff::name, country::name> >(
				selection(
					cross_product(t1, t2), 
					eq2<staff::country, country::code>())));
*/
		TESTASSERT(count(selection(cross_product(t1, t2), 
			lambda::_1[staff::country()]==lambda::_1[country::code()] && lambda::_1[staff::country()]==lambda::_1[country::code()]
			//And(eq2<staff::country, country::code>(), eq2<staff::country, country::code>())
			)) == 3);
		TESTASSERT(count(selection(cross_product(t1, t2), 
			lambda::_1[staff::country()]==lambda::_1[country::code()] || lambda::_1[staff::country()]==lambda::_1[country::code()]
			//Or(eq2<staff::country, country::code>(), eq2<staff::country, country::code>())
			)) == 3);
	}

	MyTest test;
}
