// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#pragma once

namespace bit {
namespace detail {

template<size_t ... Ns>
struct sum;

template<size_t N, size_t ... Ns>
struct sum<N, Ns...>
{
	enum : size_t { value = N + sum<Ns...>::value };
};

template<>
struct sum<>
{
	enum : size_t { value = 0 };
};


template<size_t N, size_t M>
struct min
{
	enum : size_t { value = N<M ? N : M };
};

} // namespace detail
} // namespace bit
