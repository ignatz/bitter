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

template<size_t N>
void assert_eq(std::bitset<N> const& a, std::bitset<N> const& b)
{
	ASSERT_EQ(a, b);
}

template<typename T, typename Set>
Set verify(T& c, Set (T::*getter) () const, void (T::*setter) (Set const&))
{
	// make sure all bits are zero
	assert_eq({0}, (c.*getter)());

	// fill with random bit permutation
	auto const rand = random<bitset_size<Set>::value>();
	(c.*setter)(rand);

	// and check
	assert_eq(rand, (c.*getter)());

	return rand;
}

template<typename T>
class CompoundTest : public ::testing::Test
{};

BITTER_COMPOUND(A, (_x,   7), (_y,  42), (_z,  67));
BITTER_COMPOUND(B, (_x,   3), (_y,  23), (_z,   5));
BITTER_COMPOUND(C, (_x,   7), (_y, 180), (_z,  62));
BITTER_COMPOUND(D, (_x, 180), (_y, 113), (_z, 256));


typedef ::testing::Types<A, B, C, D> CompoundTypes;

TYPED_TEST_CASE(CompoundTest, CompoundTypes);

TYPED_TEST(CompoundTest, Size)
{
	TypeParam a;
	ASSERT_EQ(a.get_x().size() + a.get_y().size() + a.get_z().size(),
		a.get().size());
}

TYPED_TEST(CompoundTest, Assignment)
{
	typedef TypeParam type;

	REPEAT(10000,
		type a;
		auto const x = verify(a, &type::get_x, &type::set_x);
		auto const y = verify(a, &type::get_y, &type::set_y);
		auto const z = verify(a, &type::get_z, &type::set_z);

		ASSERT_EQ(bit::concat(z, y, x), a.get());
	);
}

TYPED_TEST(CompoundTest, GetterSetter)
{
	typedef typename std::decay<decltype(TypeParam().get())>::type type;
	static size_t const N = bitset_size<type>::value;

	REPEAT(10000,
		type const rand = random<N>();
		TypeParam a;
		a.set(rand);
		ASSERT_EQ(rand, a.get());
	);
}
