// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#pragma once

#include <bitset>
#include <string>

#include <bitter/define.h>
#include <bitter/detail/detail.h>

#define BITTER_IF_INTEGRAL(T, RET) \
	typename std::enable_if<(std::is_integral<T>::value), RET>::type

namespace bit {

using std::size_t;
using std::bitset;

// type converter
template<typename T = unsigned long long, size_t N>
inline
T convert(bitset<N> const& t)
{
	static_assert(N <= sizeof(T)*BITTER_BITS_PER_BYTE, "unsupported conversion");
	return T(t.to_ullong());
}

template<typename T, size_t N = sizeof(T)*BITTER_BITS_PER_BYTE>
inline
BITTER_IF_INTEGRAL(T, bitset<N>)
convert(T t) noexcept
{
	return bitset<N>(t);
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
template<size_t N>
inline
bitset<N> concat(bitset<N> const& t)
{
	return t;
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
