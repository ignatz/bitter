#pragma once

#include <type_traits>

namespace bit {

using std::size_t;

// size
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
size_t size(T) noexcept
{
	return sizeof(T) * 8;
}


// flip
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T flip(T t, size_t pos) noexcept
{
	return t ^ (1 << pos);
}

template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T flip(T t) noexcept
{
	for(size_t ii=0; ii<size(t); ++ii)
		t = flip(t, ii);

	return t;
}



// set
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
void _set(T& t, size_t pos) noexcept
{
	t |= (1 << pos);
}

template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T set(T t, size_t pos) noexcept
{
	_set(t, pos);
	return t;
}

template<typename T, typename ... Pos, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T set(T t, size_t pos0, size_t pos1, Pos ... pos) noexcept
{
	_set(t, pos0);
	return set(t, pos1, pos...);
}

template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T set(T t, size_t pos, bool value) noexcept
{
	return value ? set(t, pos) : t & flip(1 << pos);
}



// reset
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
void _reset(T& t, size_t pos) noexcept
{
	t &= flip(t, pos);
}

template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T reset(T t, size_t pos) noexcept
{
	_reset(t, pos);
	return t;
}

template<typename T, typename ... Pos, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T reset(T t, size_t pos0, Pos ... pos) noexcept
{
	_reset(t, pos0);
	return reset(t, pos...);
}



// test
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
bool test(T t, size_t pos) noexcept
{
	return t & (1 << pos);
}


// mask
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
T mask(T t, size_t begin, size_t end) noexcept
{
	T r (0);
	for (size_t ii = begin; ii<end; ++ii)
		test(t, ii) ? _set(r, ii-begin) : _reset(r, ii-begin);
	return r;
}


template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
size_t count(T t) noexcept
{
	size_t cnt (0);
	for(size_t ii=0; ii<size(t); ++ii)
		cnt += test(t, ii);
	return cnt;
}


// all
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
bool all(T t) noexcept
{
	for(size_t ii=0; ii<size(t); ++ii)
		if (!test(t, ii))
			return false;
	return true;
}


// any
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
bool any(T t) noexcept
{
	return t != 0x0;
}


// none
template<typename T, typename =
	typename std::enable_if<std::is_integral<T>::value>::type>
bool none(T t) noexcept
{
	return t == 0x0;
}

} // namespace bit
