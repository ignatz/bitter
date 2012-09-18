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
size_t size(bitset<N> t)
{
	return t.size();
}


// flip
template<size_t N>
bitset<N> flip(bitset<N> t, size_t pos)
{
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
	t.set(pos);
	return t;
}

template<size_t N, typename ... Pos>
bitset<N> set(bitset<N> t, size_t pos0, size_t pos1, Pos ... pos)
{
	t.set(pos0);
	return set(t, pos1, pos...);
}

template<size_t N>
bitset<N> set(bitset<N> t, size_t pos, bool value)
{
	return t.set(pos, value);
}


// reset
template<size_t N>
bitset<N> reset(bitset<N> t, size_t pos)
{
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
	return t.test(pos);
}


// mask
template<size_t N>
bitset<N> mask(bitset<N> t, bitset<N> mask)
{
	return t & mask;
}


// crop
template<size_t N>
bitset<N> crop(bitset<N> t, size_t begin, size_t end)
{
	bitset<N> m ((1 << (end+1)) - (1 << (begin)));
	return mask(t, m) >> begin;
}


// mask
template<size_t N>
bitset<N> mask(bitset<N> t, size_t begin, size_t end)
{
	bitset<N> r (0);
	for (size_t ii = begin; ii<end; ++ii)
		test(t, ii) ? r.set(ii-begin) : r.reset(ii-begin);
	return r;
}


// count
template<size_t N>
size_t count(bitset<N> const& t)
{
	return t.count();
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

} // namespace bit
