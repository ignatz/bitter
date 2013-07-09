// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#include <gtest/gtest.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <bitset>

#define REPEAT(N, ...) \
	do { \
		srand(time(NULL)); \
		for (size_t __ii = 0; __ii < N; ++__ii) { \
			__VA_ARGS__ \
		} \
	} while(false)

inline
std::string random_bit_string(size_t len)
{
	std::string s(len, '0');
	for (auto& c : s)
		c = rand() % 2 ? '0' : '1';
	return s;
}

template<typename T>
T random(size_t len = sizeof(T)*8)
{
	return rand() % (1<<len ? 1<<len : RAND_MAX);
}

template<size_t N>
std::bitset<N> random()
{
	return std::bitset<N>(random_bit_string(N));
}

template<size_t N>
using b = std::bitset<N>;


inline
size_t _count(std::string const& s)
{
	size_t cnt = 0;
	for (auto const& c : s)
		cnt += (c=='1');
	return cnt;
}

template<size_t N>
size_t _count(std::bitset<N> const& p)
{
	return _count(p.to_string());
}

template<typename T>
size_t _count(T t)
{
	std::bitset<sizeof(T)*8> const p = t;
	return _count(p.to_string());
}


inline
bool _parity(std::string const& s)
{
	return _count(s) % 2;
}

template<size_t N>
bool _parity(std::bitset<N> const& p)
{
	return _parity(p.to_string());
}

template<typename T>
bool _parity(T t)
{
	std::bitset<sizeof(T)*8> const p = t;
	return _parity(p.to_string());
}
