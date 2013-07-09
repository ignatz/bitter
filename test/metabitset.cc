// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#include "test/test.h"

#include <bitter/metabitset.h>
#include <boost/preprocessor/cat.hpp>

#define TEST_MBS(mbs, name, size) \
	std::bitset<size> name = random<size>(); \
	mbs. BOOST_PP_CAT(set_, name) (name); \
	ASSERT_EQ(name, mbs. BOOST_PP_CAT(get_, name) ());

using namespace bit;

struct A
{
	BITTER_METABITSET_MEMBER(a, (_x, 7), (_y, 42), (_z, 67));
};

struct B
{
	BITTER_METABITSET_MEMBER(b, (_x, 3), (_y, 23), (_z, 5));
};

TEST(MetaBitset, Member)
{
	A a;
	REPEAT(10000,
		TEST_MBS(a, x,  7);
		TEST_MBS(a, y, 42);
		TEST_MBS(a, z, 67);

		ASSERT_EQ(bit::concat(z, y, x), a.a);
	);

	B b;
	REPEAT(10000,
		TEST_MBS(b, x,  3);
		TEST_MBS(b, y, 23);
		TEST_MBS(b, z,  5);

		ASSERT_EQ(bit::concat(z, y, x), b.b);
	);
}

BITTER_METABITSET_CLASS(C, (_x, 7), (_y, 42), (_z, 67))

TEST(MetaBitset, Class)
{
	C c;
	REPEAT(10000,
		TEST_MBS(c, x,  7);
		TEST_MBS(c, y, 42);
		TEST_MBS(c, z, 67);
	);
}
