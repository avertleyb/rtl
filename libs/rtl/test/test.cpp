#include "pch.hpp"

#include "generic.hpp"
#include <cassert>
#include "test.hpp"
#include "test_suite.hpp"

using namespace unit;

test::test(const std::string& name)
{
	bool result = test_suite::instance().add_test(name, *this);
	assert(result);
}
