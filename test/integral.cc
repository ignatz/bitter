// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

// test custom functions rather than compiler builtin implementations
#define BITTER_USE_BUILTIN 0

#include "test/test.h"

#include <bitter/integral.h>
#include <bitter/exception.h>

#define RANGE_CHECK(T, LEN, FUNC) \
	do { \
		for (size_t ii=0; ii<LEN; ++ii) \
			ASSERT_NO_THROW(FUNC(a, ii)); \
		ASSERT_THROW(FUNC(a, LEN+1), bitter_exception); \
	} while(false)


using namespace bit;

TEST(Integral, Size)
{
	ASSERT_EQ(BITTER_BITS_PER_BYTE*sizeof(char), size(char()));
	ASSERT_EQ(BITTER_BITS_PER_BYTE*sizeof(short), size(short()));
	ASSERT_EQ(BITTER_BITS_PER_BYTE*sizeof(int), size(int()));
	ASSERT_EQ(BITTER_BITS_PER_BYTE*sizeof(size_t), size(size_t()));
}

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

	{ // test range checking
		uint64_t a=0xff;
		RANGE_CHECK(a, sizeof(a)*BITTER_BITS_PER_BYTE, set);
	}
}

TEST(Integral, Reset)
{
	ASSERT_EQ(0, reset(1, 0));
	ASSERT_EQ(0, reset(7, 0,1,2));

	unsigned char n = 0x87;

	n = reset(n, 7);

	ASSERT_EQ((unsigned char)0x07, n);
	ASSERT_EQ(n, flip(0x87, 7));

	{ // test range checking
		uint64_t a=0xff;
		RANGE_CHECK(a, sizeof(a)*BITTER_BITS_PER_BYTE, reset);
	}

	REPEAT(100,
		size_t const r = random();
		for (size_t ii=0; ii<size(r); ++ii)
		{
			if (test(r, ii)) {
				ASSERT_NE(test(r, ii), test(reset(r, ii), ii));
				ASSERT_EQ(test(r, ii), test(set(reset(r, ii), ii), ii));
			} else {
				ASSERT_EQ(test(r, ii), test(reset(r, ii), ii));
			}
		}
	);
}

TEST(Integral, Test)
{
	ASSERT_TRUE(test(7, 0));
	ASSERT_TRUE(test(7, 1));
	ASSERT_TRUE(test(7, 2));
	ASSERT_FALSE(test(7, 3));
	ASSERT_FALSE(test(7, 4));

	{ // test range checking
		uint64_t a=0xff;
		RANGE_CHECK(a, sizeof(a)*BITTER_BITS_PER_BYTE, test);
	}

	REPEAT(100,
		size_t const r = random();
		for(size_t ii=0; ii<size(r); ++ii)
			ASSERT_EQ((r & (1<<ii)) > 0, test(r, ii));
	);
}

TEST(Integral, Mask)
{
	ASSERT_EQ(0x0,      mask(0x00ff00, 0xff00ff));
	ASSERT_EQ(0x0f0f0f, mask(0xffffff, 0x0f0f0f));

	REPEAT(10000,
		auto const v = random<size_t>();
		auto const m = random<size_t>();
		ASSERT_EQ(v & m, mask(v, m));
	);
}

TEST(Integral, Crop)
{
	ASSERT_EQ(0x00,   crop<8>(0x00ff00));
	ASSERT_EQ(0xff,   crop<8>(0x00ff00, 8));
	ASSERT_EQ(0x8001, crop<16>(0x180010, 4));

	REPEAT(1000,
		auto const r = random<uint64_t>();
		ASSERT_EQ(b<23>(r>>27).to_ullong(), crop<23>(r, 27));
	);
}

TEST(Integral, Flip)
{
	REPEAT(10000,
		uint64_t const r = random<uint64_t>();
		ASSERT_EQ(b<64>(r).flip(), flip(r));

		for (size_t ii=0; ii<size(r); ++ii)
			ASSERT_NE(test(r, ii), test(flip(r, ii), ii));
	);
}

TEST(Integral, Count)
{
	ASSERT_EQ(4u, count(0x1111));
	ASSERT_EQ(4u, count((size_t)0x1111));
	ASSERT_EQ(4u, count((unsigned int)0x1111));

	// test custom functions (builtins only: u, ul, ull)
	ASSERT_EQ(2u, count((unsigned char)0x11));

	REPEAT(10000,
		   short const r = random<short>();
		   ASSERT_EQ(_count(r), count(r));
	);
}

TEST(Integral, Parity)
{
	ASSERT_TRUE( parity(0x0000001));
	ASSERT_TRUE( parity(0x1111111));
	ASSERT_FALSE(parity(0x0000000));

	ASSERT_FALSE(parity((uint64_t)0x0101000001010101));
	ASSERT_FALSE(parity((uint64_t)0x0101000001010101));

	// test custom functions (builtins only: u, ul, ull)
	REPEAT(10000,
		auto const s = random<short>();
		ASSERT_EQ(_parity(s), parity(s));

		auto const l = random<long>();
		ASSERT_EQ(_parity(l), parity(l));
	);
}

TEST(Integral, All)
{
	ASSERT_TRUE(all((unsigned char)0xff));
	ASSERT_FALSE(all(0xff));

	REPEAT(10000,
		auto const s = random<unsigned short>();
		ASSERT_EQ(b<sizeof(short)*8>(s).all(), all(s));

		auto const l = random<long>();
		ASSERT_EQ(b<sizeof(long)*8>(l).all(), all(l));
	);
}

TEST(Integral, Any)
{
	ASSERT_TRUE(any(0x00010000));
	ASSERT_FALSE(any(0x0));

	REPEAT(10000,
		auto const s = random<unsigned short>();
		ASSERT_EQ(b<sizeof(short)*8>(s).any(), any(s));

		auto const l = random<long>();
		ASSERT_EQ(b<sizeof(long)*8>(l).any(), any(l));
	);
}

TEST(Integral, None)
{
	ASSERT_TRUE(none(0x0));
	ASSERT_FALSE(none(0x000100000));

	REPEAT(10000,
		auto const s = random<unsigned short>();
		ASSERT_EQ(b<sizeof(short)*8>(s).none(), none(s));

		auto const l = random<long>();
		ASSERT_EQ(b<sizeof(long)*8>(l).none(), none(l));
	);
}

TEST(Integral, Reverse)
{
	ASSERT_EQ(0, reverse(0x0));
	ASSERT_EQ((size_t)0xaa00000000000000, reverse((size_t)0x55));
	ASSERT_EQ((int64_t)0x0aa0000000000000, reverse((int64_t)0x550));
	ASSERT_EQ((int64_t)0xaaa0000000000000, reverse((int64_t)0x555));
	ASSERT_EQ((char)0xaa, reverse((char)0x55));
	ASSERT_EQ((unsigned char)0xaa, reverse((unsigned char)0x55));

	REPEAT(1000,
		std::string const s = random_bit_string(64);
		uint64_t const i = b<64>(s).to_ullong();
		b<64> const ri = reverse(i);

		ASSERT_EQ(i, reverse(reverse(i)));

		for(size_t ii=0; ii<64; ++ii)
			ASSERT_EQ(s[ii], ri[ii] ? '1' : '0');
	);
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
