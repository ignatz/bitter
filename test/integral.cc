// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#include <gtest/gtest.h>

// test custom functions rather than compiler builtin implementations
#define BITTER_USE_BUILTIN 0

#include <bitter/integral.h>

using namespace bit;

TEST(Integral, Defines)
{
	ASSERT_EQ(8, BITTER_BITS_PER_BYTE);
}

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

TEST(Integral, Mask)
{
	ASSERT_EQ(0x0,      mask(0x00ff00, 0xff00ff));
	ASSERT_EQ(0x0f0f0f, mask(0xffffff, 0x0f0f0f));
}

TEST(Integral, Crop)
{
	ASSERT_EQ(0x00,   crop<8>(0x00ff00));
	ASSERT_EQ(0xff,   crop<8>(0x00ff00, 8));
	ASSERT_EQ(0x8001, crop<16>(0x180010, 4));
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
	ASSERT_EQ(4, count((size_t)0x1111));
	ASSERT_EQ(4, count((unsigned int)0x1111));

	// test custom functions (builtins only: u, ul, ull)
	//ASSERT_EQ(2, count((unsigned char)0x11));
	ASSERT_EQ(4, count((unsigned short)0x1111));

	ASSERT_EQ(2, count((int)0x11));
	ASSERT_EQ(4, count((long)0x1111));
}

TEST(Integral, Parity)
{
	ASSERT_TRUE( parity(0x0000001));
	ASSERT_TRUE( parity(0x1111111));
	ASSERT_FALSE(parity(0x0000000));

	ASSERT_FALSE(parity((uint64_t)0x0101000001010101));
	ASSERT_FALSE(parity((uint64_t)0x0101000001010101));

	for (size_t ii = 0; ii<64; ++ii)
		ASSERT_TRUE(parity(set((uint64_t)0x0, ii)));

	for (size_t ii = 0; ii<64-1; ++ii)
		ASSERT_FALSE(parity(set(set((uint64_t)0x0, ii), ii+1)));

	// test custom functions (builtins only: u, ul, ull)
	ASSERT_TRUE(parity((unsigned char)0x01));
	ASSERT_TRUE(parity((unsigned short)0x01));
	ASSERT_FALSE(parity((unsigned char)0x11));
	ASSERT_FALSE(parity((unsigned short)0x1111));

	ASSERT_FALSE(parity((int)0x11));
	ASSERT_FALSE(parity((long)0x1111));
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

TEST(Integral, Reverse)
{
	ASSERT_EQ(0, reverse(0x0));
	ASSERT_EQ((size_t)0xaa00000000000000, reverse((size_t)0x55));
	ASSERT_EQ((int64_t)0x0aa0000000000000, reverse((int64_t)0x550));
	ASSERT_EQ((int64_t)0xaaa0000000000000, reverse((int64_t)0x555));
	ASSERT_EQ((char)0xaa, reverse((char)0x55));
	ASSERT_EQ((unsigned char)0xaa, reverse((unsigned char)0x55));
}

#include "bitter/util.h"

TEST(Integral, Conversion)
{
	ASSERT_EQ(std::string(sizeof(int)*8, '0'), str(0));
	ASSERT_EQ(std::string(8, '1'), str<8>(0xfeff));
	ASSERT_EQ(std::bitset<sizeof(int)*8>(0), convert(0));
	ASSERT_EQ(std::bitset<sizeof(int)*8>(0xff), convert(0xff));
	ASSERT_EQ(std::bitset<sizeof(char)*8>(0), convert((unsigned char)0));
}
