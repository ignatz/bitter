// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#pragma once

#if !defined(BITTER_BITS_PER_BYTE)
	#define BITTER_BITS_PER_BYTE 8
#endif

#if !defined(BITTER_USE_BUILTIN)
	#define BITTER_USE_BUILTIN 1
#endif

#if defined(__GNUG__) && __GNUG__ >= 4
	#define BITTER_LIKELY(x) (__builtin_expect((x), 1))
	#define BITTER_UNLIKELY(x) (__builtin_expect((x), 0))
#else
	#define BITTER_LIKELY(x) (x)
	#define BITTER_UNLIKELY(x) (x)
#endif

#if !defined(BITTER_NO_RANGE_CHECK)
	#include <bitter/exception.h>
	#define BITTER_RANGE_CHECK(N, POS) \
		if BITTER_UNLIKELY(POS >= N) \
			throw bitter_exception("out of range");
#else
	#define BITTER_RANGE_CHECK(N, POS)
#endif
