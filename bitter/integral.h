// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#pragma once

#include <type_traits>

#include <bitter/define.h>

#define BITTER_IF_INTEGRAL(T, RET) \
	typename std::enable_if<(std::is_integral<T>::value), RET>::type

namespace bit {

using std::size_t;

// size
template<typename T>
inline
BITTER_IF_INTEGRAL(T, size_t)
size(T) noexcept
{
	return sizeof(T)*BITTER_BITS_PER_BYTE;
}


// flip
template<typename T>
inline
BITTER_IF_INTEGRAL(T, T)
flip(T t, size_t pos) noexcept
{
	return t ^ (1<<pos);
}

template<typename T>
inline
BITTER_IF_INTEGRAL(T, T)
flip(T t) noexcept
{
	return ~t;
}


// set
template<typename T>
inline
BITTER_IF_INTEGRAL(T, void)
_set(T& t, size_t pos) noexcept
{
	t |= (1<<pos);
}

template<typename T>
inline
BITTER_IF_INTEGRAL(T, T)
set(T t, size_t pos) noexcept
{
	_set(t, pos);
	return t;
}

template<typename T, typename ... Pos>
inline
BITTER_IF_INTEGRAL(T, T)
set(T t, size_t pos0, size_t pos1, Pos ... pos) noexcept
{
	_set(t, pos0);
	return set(t, pos1, pos...);
}

template<typename T>
inline
BITTER_IF_INTEGRAL(T, T)
set(T t, size_t pos, bool value) noexcept
{
	return (t & ~(1<<pos)) | (-value & (1<<pos));
}


// reset
template<typename T>
inline
BITTER_IF_INTEGRAL(T, void)
_reset(T& t, size_t pos) noexcept
{
	t &= flip(t, pos);
}

template<typename T>
inline
BITTER_IF_INTEGRAL(T, T)
reset(T t, size_t pos) noexcept
{
	_reset(t, pos);
	return t;
}

template<typename T, typename ... Pos>
inline
BITTER_IF_INTEGRAL(T, T)
reset(T t, size_t pos0, Pos ... pos) noexcept
{
	_reset(t, pos0);
	return reset(t, pos...);
}


// test
template<typename T>
inline
BITTER_IF_INTEGRAL(T, bool)
test(T t, size_t pos) noexcept
{
	return t & (1<<pos);
}


// mask
template<typename T>
inline
BITTER_IF_INTEGRAL(T, T)
mask(T t, T mask) noexcept
{
	return t & mask;
}


// crop
template<size_t Len, typename T>
inline
BITTER_IF_INTEGRAL(T, T)
crop(T t, size_t offset = 0) noexcept
{
	return (t >> offset) & ~(~0 << Len);
}


// count
template<typename T>
typename std::enable_if<std::is_signed<T>::value, size_t>::type
count(T t) noexcept
{
	// Brian Kernighan's way
	size_t c = 0;
	for (; t; c++)
		t &= t - 1;
	return c;
}

#if BITTER_USE_BUILTIN && defined(__GNUG__) && __GNUG__ >= 4

inline
size_t
count(unsigned int t) noexcept
{
	return __builtin_popcount(t);
}

inline
size_t
count(unsigned long t) noexcept
{
	return __builtin_popcountl(t);
}

inline
size_t
count(unsigned long long t) noexcept
{
	return __builtin_popcountll(t);
}

template<typename T>
inline
typename std::enable_if<std::is_unsigned<T>::value, size_t>::type
count(T t) noexcept
{
	return count(static_cast<unsigned long long>(t));
}

#else

template<typename T>
inline
typename std::enable_if<std::is_unsigned<T>::value, size_t>::type
count(T t) noexcept
{
	static_assert(sizeof(T) <= 16, "unsigned integral type too long");

	t = t - ((t>>1) & static_cast<T>(~static_cast<T>(0)/3));
	t = (t & static_cast<T>(~static_cast<T>(0))/15*3) +
		((t >> 2) & static_cast<T>(~static_cast<T>(0)/15*3));
	t = (t + (t>>4)) & static_cast<T>(~static_cast<T>(0))/255*15;
	return static_cast<T>(t * (static_cast<T>(~static_cast<T>(0))/255))
		>> (sizeof(T)-1) * BITTER_BITS_PER_BYTE;
}

#endif // __GNUG__


// parity
template<typename T>
inline
typename std::enable_if<std::is_signed<T>::value, size_t>::type
parity(T t) noexcept
{
	return count(t) & 1;
}

#if BITTER_USE_BUILTIN && defined(__GNUG__) && __GNUG__ >= 4

inline
bool
parity(unsigned int t) noexcept
{
	return __builtin_parity(t);
}

inline
bool
parity(unsigned long t) noexcept
{
	return __builtin_parityl(t);
}

inline
bool
parity(unsigned long long t) noexcept
{
	return __builtin_parityll(t);
}

template<typename T>
inline
typename std::enable_if<std::is_unsigned<T>::value, bool>::type
parity(T t) noexcept
{
	return parity(static_cast<unsigned long long>(t));
}

#else

template<typename T>
inline
typename std::enable_if<std::is_unsigned<T>::value, bool>::type
parity(T t) noexcept
{
	size_t d = sizeof(T)*4;
	for (; d>2; d >>= 1)
		t ^= t >>  d;
	return (0x6996>>(t & 0xf)) & 1;
}

#endif // __GNUG__


// all
template<typename T>
inline
BITTER_IF_INTEGRAL(T, bool)
all(T t) noexcept
{
	return !static_cast<T>(~t);
}


// any
template<typename T>
inline
BITTER_IF_INTEGRAL(T, bool)
any(T t) noexcept
{
	return t != 0x0;
}


// none
template<typename T>
inline
BITTER_IF_INTEGRAL(T, bool)
none(T t) noexcept
{
	return !t;
}


// reverse
template<typename T>
inline
BITTER_IF_INTEGRAL(T, T)
reverse(T t) noexcept
{
	size_t s = sizeof(T)*BITTER_BITS_PER_BYTE;
	typename std::make_unsigned<T>::type mask = ~0;
	while (s >>= 1) {
		mask ^= (mask << s);
		t = ((t >> s) & mask) | ((t << s) & ~mask);
	}
	return t;
}

} // namespace bit

#undef BITTER_IF_INTEGRAL
