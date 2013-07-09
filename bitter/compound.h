// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#pragma once

#include "bitter/bitset.h"
#include "bitter/util.h"

#ifndef BOOST_PP_VARIADICS
#define BOOST_PP_VARIADICS
#endif // BOOST_PP_VARIADICS

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/first_n.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>

#define BITTER_COMPOUND_IF_EMPTY(...) 0

#define BITTER_COMPOUND_CAST(z, n, data) static_cast<data>(n)

#define BITTER_COMPOUND_ADD(s, state, x) \
	BOOST_PP_ADD(state, BOOST_PP_TUPLE_ELEM(1, x))

#define BITTER_COMPOUND_N(seq) BOOST_PP_SEQ_FOLD_LEFT(BITTER_COMPOUND_ADD, 0, seq)

#define BITTER_COMPOUND_TYPE(seq) std::bitset<BITTER_COMPOUND_N(seq)>

#define BITTER_COMPOUND_NAME(elem) BOOST_PP_TUPLE_ELEM(0, elem)
#define BITTER_COMPOUND_SIZE(elem) BOOST_PP_TUPLE_ELEM(1, elem)

#define BITTER_COMPOUND_MEMBER(data) BOOST_PP_TUPLE_ELEM(0, data)
#define BITTER_COMPOUND_SEQ(data) BOOST_PP_TUPLE_ELEM(1, data)

#define BITTER_COMPOUND_SHIFT(i, seq) \
	BOOST_PP_IF(i, BOOST_PP_SEQ_FOLD_LEFT, BITTER_COMPOUND_IF_EMPTY)( \
		BITTER_COMPOUND_ADD, 0, BOOST_PP_SEQ_FIRST_N(i, seq))

#define BITTER_COMPOUND_GEN_MASK(i, size, seq) \
	~(bit::set(BITTER_COMPOUND_TYPE(seq) () \
			BOOST_PP_ENUM_TRAILING(size, BITTER_COMPOUND_CAST, size_t)) \
		<< BITTER_COMPOUND_SHIFT(i, seq))

#define BITTER_COMPOUND_GETTER(data, i, elem) \
	std::bitset<BITTER_COMPOUND_SIZE(elem)> \
		BOOST_PP_CAT(get, BITTER_COMPOUND_NAME(elem)) () const \
	{ \
		return bit::crop<BITTER_COMPOUND_SIZE(elem)> ( \
			BITTER_COMPOUND_MEMBER(data), \
			BITTER_COMPOUND_SHIFT(i, BITTER_COMPOUND_SEQ(data))); \
	}

#define BITTER_COMPOUND_SETTER(data, i, elem) \
	void BOOST_PP_CAT(set, BOOST_PP_TUPLE_ELEM(0, elem)) \
		(std::bitset<BOOST_PP_TUPLE_ELEM(1, elem)> const& v) \
	{ \
		/* generate static mask for this part of the bitset */ \
		static const auto m = BITTER_COMPOUND_GEN_MASK( \
			i, BITTER_COMPOUND_SIZE(elem), BITTER_COMPOUND_SEQ(data)); \
		\
		/* actually set the corresponding bits */ \
		BOOST_PP_TUPLE_ELEM(0, data) = \
			/* mask out previous bit pattern */ \
			bit::mask(BITTER_COMPOUND_MEMBER(data), m) | \
			/* substitute new bits */ \
			bit::resize<BITTER_COMPOUND_N(BITTER_COMPOUND_SEQ(data))>(v) \
				<< BITTER_COMPOUND_SHIFT(i, BITTER_COMPOUND_SEQ(data)); \
	}

#define BITTER_COMPOUND_ACCESSORS(r, data, i, elem) \
	static_assert(BOOST_PP_TUPLE_ELEM(1, elem), "no zero sized fields"); \
	BITTER_COMPOUND_GETTER(data, i, elem) \
	BITTER_COMPOUND_SETTER(data, i, elem)

#define BITTER_COMPOUND(name, ...) \
	class name \
	{ \
	private: \
		typedef BITTER_COMPOUND_TYPE(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) __t; \
		__t __value; \
	public: \
		constexpr name(__t const& v) : \
			__value(v) {} \
		constexpr name(name const& v) = default; \
		constexpr name() noexcept : __value() {}; \
		name& operator= (name const& v) = default; \
		\
		/* setter and getter functions */ \
		BOOST_PP_SEQ_FOR_EACH_I(BITTER_COMPOUND_ACCESSORS, \
			(__value, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)), \
			BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
		\
		__t const& get() const { return __value; } \
	}
