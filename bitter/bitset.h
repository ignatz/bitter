// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#pragma once

#include <type_traits>
#include <bitset>

#include <bitter/define.h>

namespace bit {

using std::size_t;
using std::bitset;

// size
template<size_t N>
size_t size(bitset<N> const& t)
{
	return t.size();
}


// flip
template<size_t N>
bitset<N> flip(bitset<N> t, size_t pos)
{
	BITTER_RANGE_CHECK(N, pos)
	return t.set(pos, !t.test(pos));
}

template<size_t N>
bitset<N> flip(bitset<N> t)
{
	return t.flip();
}


// set
template<size_t N>
bitset<N> set(bitset<N> t, size_t pos)
{
	BITTER_RANGE_CHECK(N, pos)
	t.set(pos);
	return t;
}

template<size_t N, typename ... Pos>
bitset<N> set(bitset<N> t, size_t pos0, size_t pos1, Pos ... pos)
{
	BITTER_RANGE_CHECK(N, pos0)
	t.set(pos0);
	return set(t, pos1, pos...);
}

template<size_t N>
bitset<N> set(bitset<N> t, size_t pos, bool value)
{
	BITTER_RANGE_CHECK(N, pos)
	return t.set(pos, value);
}


// reset
template<size_t N>
bitset<N> reset(bitset<N> t, size_t pos)
{
	BITTER_RANGE_CHECK(N, pos)
	t.reset(pos);
	return t;
}

template<size_t N, typename ... Pos>
bitset<N> reset(bitset<N> t, size_t pos0, Pos ... pos)
{
	t.reset(pos0);
	return reset(t, pos...);
}


// test
template<size_t N>
bool test(bitset<N> const& t, size_t pos)
{
	BITTER_RANGE_CHECK(N, pos)
	return t.test(pos);
}


// mask
template<size_t N>
bitset<N> mask(bitset<N> const& t, bitset<N> const& mask)
{
	return t & mask;
}


// crop
template<size_t Len, size_t N>
bitset<Len> crop(bitset<N> const& t, size_t offset = 0)
{
	bitset<Len> r;
	size_t c = 0;

	while (c < Len && offset<N) {
		r[c++] = t.test(offset++);
	}
	return r;
}


// count
template<size_t N>
size_t count(bitset<N> const& t)
{
	return t.count();
}


// parity
template<size_t N>
bool parity(bitset<N> const& t)
{
	return count(t) & 1;
}


// all
template<size_t N>
bool all(bitset<N> const& t)
{
	return t.all();
}


// any
template<size_t N>
bool any(bitset<N> const& t)
{
	return t.any();
}


// none
template<size_t N>
bool none(bitset<N> const& t)
{
	return t.none();
}


// reverse
template<size_t N>
bitset<N> reverse(bitset<N> t)
{
	bitset<N> r(t);
	size_t s = t.size()-1;

	for (t >>= 1; t.any(); t >>= 1) {
		r <<= 1;
		r |= t & bitset<N>(1);
		s--;
	}
	return r <<= s;
}

} // namespace bit
