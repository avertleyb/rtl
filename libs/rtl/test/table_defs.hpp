// Copyright (C) 2002 Dmitriy Arapov, Arkadiy Vertleyb
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
#ifndef BOOST_RTL_TABLE_DEFS_HPP_INCLUDED
#define BOOST_RTL_TABLE_DEFS_HPP_INCLUDED
#include "table.hpp"
#define BOOST_RTL_TABLE_1(Name,FieldList) struct\
   Name##_table_info : public rel::table_info<FieldList>{}; \
   typedef rel::table<Name##_table_info> Name
#define BOOST_RTL_TABLE_2(Name,FieldList,SortList) struct\
   Name##_table_info : public rel::table_info<FieldList,SortList>{}; \
   typedef rel::table<Name##_table_info> Name
#define BOOST_RTL_TABLE_3(Name,FieldList,SortList,Keys) struct \
   Name##_table_info : public rel::table_info<FieldList,SortList,Keys>{}; \
   typedef rel::table<Name##_table_info> Name
#define BOOST_RTL_TABLE_4(Name,FieldList,SortList,Keys,Strategy) struct \
   Name##_table_info : public rel::table_info<FieldList,SortList,Keys,Strategy>{}; \
   typedef rel::table<Name##_table_info> Name

#endif
