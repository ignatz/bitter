// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#include "test/test.h"

#include <type_traits>
#include <bitter/compound.h>

template<typename T>
struct bitset_size;

template<size_t N>
struct bitset_size< std::bitset<N> > :
	public std::integral_constant<size_t, N>
{};

#define TEST_COMPOUND(instance, name) \
	/* first get size of instance */ \
	typedef typename std::decay< \
			decltype( instance. get_ ## name () ) \
		>::type name ## _t; \
	static size_t const name ## _N = bitset_size<name ## _t>::value; \
	\
	/* now do random assignment and verify */ \
	std::bitset<name ## _N> const name = random<name ## _N>(); \
	instance. set_ ## name (name); \
	ASSERT_EQ(name, instance. get_ ## name ()); \
	ASSERT_EQ(name ## _N, instance. get_ ## name ().size());

using namespace bit;

template<typename T>
class CompoundTest : public ::testing::Test
{};

BITTER_COMPOUND(A, (_x,   7), (_y,  42), (_z,  67));
BITTER_COMPOUND(B, (_x,   3), (_y,  23), (_z,   5));
BITTER_COMPOUND(C, (_x,   7), (_y, 180), (_z,  62));
BITTER_COMPOUND(D, (_x, 180), (_y, 113), (_z, 256));


typedef ::testing::Types<A, B, C, D> CompoundTypes;

TYPED_TEST_CASE(CompoundTest, CompoundTypes);

TYPED_TEST(CompoundTest, Random)
{
	TypeParam a;

	ASSERT_EQ(a.get_x().size() + a.get_y().size() + a.get_z().size(),
		a.get().size());

	REPEAT(10000,
		TEST_COMPOUND(a, x);
		TEST_COMPOUND(a, y);
		TEST_COMPOUND(a, z);

		ASSERT_EQ(concat(z, y, x), a.get());
	);
}
