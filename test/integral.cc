#include <gtest/gtest.h>
#include <iostream>

#include "bitter/bitter.h"

using namespace bit;

TEST(Integral, Set)
{
	int n = 0;

	n = set(n, 1);

	for (size_t ii=0; ii < bit::size(n); ++ii) {
		if (ii == 1)
			ASSERT_TRUE(test(n, ii));
		else
			ASSERT_FALSE(test(n, ii));
	}

	ASSERT_EQ(7, set(0, 0,1,2));
	ASSERT_EQ(0, set(1, 0, false));
}

TEST(Integral, Reset)
{
	ASSERT_EQ(0, reset(1, 0));
	ASSERT_EQ(0, reset(7, 0,1,2));

	unsigned char n = 0x87;

	n = reset(n, 7);

	ASSERT_EQ((unsigned char)0x07, n);
	ASSERT_EQ(n, flip(0x87, 7));
}

TEST(Integral, Test)
{
	ASSERT_TRUE(test(7, 0));
	ASSERT_TRUE(test(7, 1));
	ASSERT_TRUE(test(7, 2));
	ASSERT_FALSE(test(7, 3));
	ASSERT_FALSE(test(7, 4));
}

TEST(Integral, Flip)
{
	unsigned char x;

	x = flip(0);
	ASSERT_EQ((unsigned char)0xff, x);

	x = flip(1);
	ASSERT_EQ((unsigned char)0xff - 1, x);

	x = flip((char)0xff);
	ASSERT_EQ(0, x);
}

TEST(Integral, Count)
{
	ASSERT_EQ(4, count(0x1111));
}

TEST(Integral, All)
{
	ASSERT_TRUE(all((unsigned char)0xff));
	ASSERT_FALSE(all(0xff));
}

TEST(Integral, Any)
{
	ASSERT_TRUE(any(0x00010000));
	ASSERT_FALSE(any(0x0));
}

TEST(Integral, None)
{
	ASSERT_TRUE(none(0x0));
	ASSERT_FALSE(none(0x000100000));
}
