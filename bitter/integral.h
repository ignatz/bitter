#pragma once

#include <type_traits>

#include "ztl/enable_if.h"
#include "ztl/pack.h"

namespace bit {

using std::size_t;

// size
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
size_t size(T)
{
	return sizeof(T) * 8;
}


// flip
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T flip(T t, size_t pos)
{
	return t ^ (1 << pos);
}

template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T flip(T t)
{
	for(size_t ii=0; ii<size(t); ++ii)
		t = flip(t, ii);

	return t;
}



// set
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
void _set(T& t, size_t pos)
{
	t |= (1 << pos);
}

template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T set(T t, size_t pos)
{
	_set(t, pos);
	return t;
}

template<typename T, typename ... Pos, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T set(T t, size_t pos0, size_t pos1, Pos ... pos)
{
	_set(t, pos0);
	return set(t, pos1, pos...);
}

template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T set(T t, size_t pos, bool value)
{
	return value ? set(t, pos) : t & flip(1 << pos);
}



// reset
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
void _reset(T& t, size_t pos)
{
	t &= flip(t, pos);
}

template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T reset(T t, size_t pos)
{
	_reset(t, pos);
	return t;
}

template<typename T, typename ... Pos, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T reset(T t, size_t pos0, Pos ... pos)
{
	_reset(t, pos0);
	return reset(t, pos...);
}



// test
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
bool test(T t, size_t pos)
{
	return t & (1 << pos);
}


// mask
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T mask(T t, size_t begin, size_t end)
{
	T ret = 0;
	for (size_t ii = begin; ii<end; ++ii)
		test(t, ii-begin) ? _set(ret, ii-begin) : _reset(ret, ii-begin);
	return ret;
}


template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
size_t count(T t)
{
	size_t cnt = 0;
	for(size_t ii=0; ii<size(t); ++ii)
		cnt += test(t, ii);
	return cnt;
}


// all
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
bool all(T t)
{
	for(size_t ii=0; ii<size(t); ++ii)
		if (!test(t, ii))
			return false;
	return true;
}


// any
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
bool any(T t)
{
	return t != 0x0;
}


// none
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
bool none(T t)
{
	return t == 0x0;
}

} // namespace bit
