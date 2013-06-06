# `bitter` Bit Manipulation Library

[![Build Status](https://travis-ci.org/ignatz/bitter.png?branch=master)](https://travis-ci.org/ignatz/bitter)

`bitter` provides a common interface for bit operations on integral types as
well as `std::bitset`. If you have ever worked with `bitset` before the
interface should feel instantly familiar.

## Motivation
Often many ways lead to Rome, but some might be more efficient than others.
Unfortunately, this is particularly true for bit operations. `bitter` prevents
you from reinventing the wheel over and over again by providing efficient and
well tested implementations for most bit operations.
This will also help to make your code more expressive. What would you rather
read:

	int _t = 0xdeadface
	int s = 64;
	int mask = ~0;
	while (s >>= 1) {
		mask ^= (mask << s);
		_t = ((_t >> s) & mask) | ((_t << s) & ~mask);
	}
	int const t = _t;

or

	int const t = bit::reverse(0xdeadface);

(Note, that the former implementation is already pretty concise) ? Now, imagine
many similar but slightly different implementations like this intermingled with
many other bit operations spread across your code base.

Finally, `bitter` enables you to mix integral and `std::bitset` types
arbitrarily and/or switch from one to the other without breaking code.

## Using `bitter`
`bitter` is a header only library, which makes it pretty easy to use.
It should be enough to add `bitter` to the include path and enable your
compiler's `C++11` mode.

## Requirements
* modern C++11 compiler

## Licensing
Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)

bitter is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

bitter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with bitter.  If not, see <http://www.gnu.org/licenses/>.
