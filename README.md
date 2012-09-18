`bitter` Bit Manipulation Library
===============================

`bitter` eases working with low level legacy code, where people tend to use
most cryptic and often not very efficient bit operations.

When to use `bitter`
------------------
C++11 provides `std::bitset`, which brings a nice interface for frequent
operations like setting, resetting and testing of bits. So, if you have an
option, use `std::bitset` always, but if you work on C or legacy code you
might consider using `bitter`.
Besides, with `bitter` you can arbitrarily mix integral and `std::bitset`
types.

Licensing
---------
Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)

Distributed under the terms of the GPLv2 or newer
