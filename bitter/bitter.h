#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "bitter/integral.h"
#include "bitter/bitset.h"

#include <string>

#define BITTER_IF_INTEGRAL(T, RET) \
	typename std::enable_if<(std::is_integral<T>::value), RET>::type

namespace bit {

using std::size_t;
using std::bitset;

// type converter
template<size_t N, typename T = unsigned long>
inline
T convert(bitset<N> const& t)
{
	return static_cast<T>(t.to_ulong());
}

template<typename T, size_t N = sizeof(T)*8>
inline
BITTER_IF_INTEGRAL(T, bitset<N>)
convert(T t) noexcept
{
	return bitset<N>(static_cast<unsigned long long>(t));
}


// string converter
template<size_t N>
inline
std::string
str(bitset<N> const& t)
{
	return t.to_string();
}

template<size_t Len, size_t N>
inline
std::string
str(bitset<N> const& t)
{
	return bitset<Len>(t.to_ulong()).to_string();
}

template<typename T>
inline
BITTER_IF_INTEGRAL(T, std::string)
str(T t)
{
	return convert(t).to_string();
}

template<size_t Len, typename T>
inline
BITTER_IF_INTEGRAL(T, std::string)
str(T t)
{
	return convert<T, Len>(t).to_string();
}

} // namespace bit

#undef BITTER_IF_INTEGRAL
