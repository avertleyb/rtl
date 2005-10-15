#include "pch.hpp"

//#define BOOST_MPL_LIMIT_VECTOR_SIZE 50

#include "generic.hpp"
#include "test_function.hpp"
#include "rename.hpp"
#include "table.hpp"
#include "utils.hpp"
#include "test_tables.hpp"
#include "crossproduct.hpp"

namespace rename_cpp
{
	using namespace rel;
	using namespace unit;
	using namespace boost;

	BOOST_RTL_DEFINE_COLUMN(int, ssn1);
	BOOST_RTL_DEFINE_COLUMN(std::string, name1);
	BOOST_RTL_DEFINE_COLUMN(int, year1);
	BOOST_RTL_DEFINE_COLUMN(std::string, project1);
	BOOST_RTL_DEFINE_COLUMN(int, country1);

	typedef boost::mpl::vector5<ssn1,name1,year1,project1,country1> vec;

	static void test5()
	{
		print
		(
			cross_product
			(
				staff::table(), 
				manual_rename<vec>(staff::table())
			)
		);
	}
	
	test_function t5("Rename5", test5);
}
