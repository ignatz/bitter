#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <bitset>
#include <string>

#include "bitter/detail/detail.h"

#define BITTER_IF_INTEGRAL(T, RET) \
	typename std::enable_if<(std::is_integral<T>::value), RET>::type

namespace bit {

using std::size_t;
using std::bitset;

// type converter
template<typename T = unsigned long, size_t N>
inline
T convert(bitset<N> const& t)
{
	return T(t.to_ullong());
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
	return bitset<Len>(t.to_ullong()).to_string();
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


// concatenation of bitsets
inline
bitset<0> concat()
{
	return bitset<0> {};
}

template<size_t N, size_t ... Ns>
bitset<detail::sum<N, Ns...>::value>
concat(bitset<N> const& t, bitset<Ns> const& ... ts)
{
	typedef bitset<detail::sum<N, Ns...>::value> type;
	return convert<type>(t) << detail::sum<Ns...>::value
		| convert<type>(concat(ts...));
}

} // namespace bit

#undef BITTER_IF_INTEGRAL
