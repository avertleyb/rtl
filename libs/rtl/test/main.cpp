#include "pch.hpp"

#include "generic.hpp"

#ifdef _MSC_VER
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include <iostream>
#include "test_suite.hpp"

using namespace unit;

static bool test_app(int argc, char** argv)
{
	if (argc == 0)
		return test_suite::instance().run();

	else
	{
		while (argc-- != 0)
			test_suite::instance().run(*argv++);

		return true;
	}
}

int
main(int argc, char* argv[])
{
#ifdef _MSC_VER
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	if (!test_app(argc - 1, argv + 1))
	{
		std::cout << "*** Some tests failed!!! ***" << std::endl;
		return 1;
	}

	return 0;
}
