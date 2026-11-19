# Relational Template Library (RTL)

RTL is a C++ library that implements relational algebra using C++ templates and template metaprogramming.

## What RTL provides

- **Typed relational data:** columns, column lists, tuples, and tables for representing relational structures in C++.
- **Relational operations:** selection, projection, Cartesian product, extension, set operations, and joins.
- **Additional query operations:** renaming, grouping, range-based selection, and related operations.
- **Indexes:** facilities for indexed access to relational data.
- **Transactions:** support for tracking table changes and updating dependent relations and indexes, including incremental updates.
- **C++ integration:** relational expressions that can incorporate application-defined C++ types and computations.

The library's abstractions and operations are described in detail in the [RTL documentation](libs/rtl/doc/rtl.htm).

## Getting started

RTL's public headers are in [`boost/rtl/`](boost/rtl/). The accompanying examples and tests are under [`libs/rtl/`](libs/rtl/).

Start with the [worked example](libs/rtl/doc/rtl.htm) in the documentation. It demonstrates a query over a table of cities and their coordinates, selecting pairs of cities based on the distance between them.

For additional examples of the API, see:

- [`libs/rtl/test/`](libs/rtl/test/) — tests exercising the library's operations.
- [`libs/rtl/doc/transactions_sample.cpp`](libs/rtl/doc/transactions_sample.cpp) — transaction-related sample code.

## Documentation

The [RTL reference document](libs/rtl/doc/rtl.htm) covers:

- Motivation and a worked example
- Columns, column lists, tuples, tables, and iterators
- Relational expressions and operations
- Indexes and inverted relations
- Type deduction in relational expressions
- Transactions and incremental updates
- Tests and compiler information

## Authors

RTL was developed by **Arkadiy Vertleyb** and **Dmitriy Arapov**.

## License

Distributed under the [Boost Software License, Version 1.0](LICENSE_1_0.txt).
