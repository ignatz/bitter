#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>
#include <bitset>

namespace bit {

using std::size_t;
using std::bitset;

// size
template<size_t N>
inline
size_t size(bitset<N> const& t)
{
	return t.size();
}


// flip
template<size_t N>
inline
bitset<N> flip(bitset<N> t, size_t pos)
{
	return t.set(pos, !t.test(pos));
}

template<size_t N>
inline
bitset<N> flip(bitset<N> t)
{
	return t.flip();
}


// set
template<size_t N>
inline
bitset<N> set(bitset<N> t, size_t pos)
{
	t.set(pos);
	return t;
}

template<size_t N, typename ... Pos>
inline
bitset<N> set(bitset<N> t, size_t pos0, size_t pos1, Pos ... pos)
{
	t.set(pos0);
	return set(t, pos1, pos...);
}

template<size_t N>
inline
bitset<N> set(bitset<N> t, size_t pos, bool value)
{
	return t.set(pos, value);
}


// reset
template<size_t N>
inline
bitset<N> reset(bitset<N> t, size_t pos)
{
	t.reset(pos);
	return t;
}

template<size_t N, typename ... Pos>
inline
bitset<N> reset(bitset<N> t, size_t pos0, Pos ... pos)
{
	t.reset(pos0);
	return reset(t, pos...);
}


// test
template<size_t N>
inline
bool test(bitset<N> const& t, size_t pos)
{
	return t.test(pos);
}


// mask
template<size_t N>
inline
bitset<N> mask(bitset<N> const& t, bitset<N> const& mask)
{
	return t & mask;
}


// crop
template<size_t Len, size_t N>
inline
bitset<Len> crop(bitset<N> const& t, size_t offset = 0)
{
	return bitset<Len>(t.to_ullong() >> offset);
}


// count
template<size_t N>
inline
size_t count(bitset<N> const& t)
{
	return t.count();
}


// parity
template<size_t N>
inline
bool parity(bitset<N> const& t)
{
	return count(t) & 1;
}


// all
template<size_t N>
inline
bool all(bitset<N> const& t)
{
	return t.all();
}


// any
template<size_t N>
inline
bool any(bitset<N> const& t)
{
	return t.any();
}


// none
template<size_t N>
inline
bool none(bitset<N> const& t)
{
	return t.none();
}


// reverse
template<size_t N>
inline
bitset<N>
reverse(bitset<N> t)
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
