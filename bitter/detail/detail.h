#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

namespace bit{
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

} // namespace detail
} // namespace bit
