// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#include "test/test.h"

#include <bitter/bitset.h>
#include <bitter/exception.h>

#define RANGE_CHECK(T, LEN, FUNC) \
	do { \
		for (size_t ii=0; ii<LEN; ++ii) \
			ASSERT_NO_THROW(FUNC(a, ii)); \
		ASSERT_THROW(FUNC(a, LEN+1), bitter_exception); \
	} while(false)


using namespace bit;

TEST(Bitset, Size)
{
	ASSERT_EQ(   0u, size(b<   0>()));
	ASSERT_EQ( 123u, size(b< 123>()));
	ASSERT_EQ(1337u, size(b<1337>()));
}

TEST(Bitset, Set)
{
	for (size_t ii=0; ii<42; ++ii)
	{
		for (size_t jj=0; jj<42; ++jj)
		{
			if (ii == jj)
				ASSERT_TRUE(test(set(b<42>(0), ii), jj));
			else
				ASSERT_FALSE(test(set(b<42>(0), ii), jj));
		}
	}

	ASSERT_EQ(b<42>(7), set(b<42>(0), 0,1,2));
	ASSERT_EQ(b<42>(0), set(b<42>(1), 0, false));

	{ // test range checking
		size_t const len = 444;
		b<len> a(std::string(len, '1'));
		RANGE_CHECK(a, len, set);
	}
}

TEST(Bitset, Reset)
{
	ASSERT_EQ(b<42>(0), reset(b<42>(1), 0));
	ASSERT_EQ(b<42>(0), reset(b<42>(7), 0,1,2));

	b<42> n = 0x87;
	n = reset(n, 7);

	ASSERT_EQ(b<42>(0x07), n);
	ASSERT_EQ(n, flip(b<42>(0x87), 7));

	{ // test range checking
		size_t const len = 444;
		b<len> a(std::string(len, '1'));
		RANGE_CHECK(a, len, reset);
	}

	REPEAT(100,
		auto const r = random<259>();
		for (size_t ii=0; ii<size(r); ++ii)
		{
			if (r[ii]) {
				ASSERT_NE(test(r, ii), test(reset(r, ii), ii));
				ASSERT_EQ(test(r, ii), test(set(reset(r, ii), ii), ii));
			} else {
				ASSERT_EQ(test(r, ii), test(reset(r, ii), ii));
			}
		}
	);
}

TEST(Bitset, Test)
{
	ASSERT_TRUE( test(b<42>(7), 0));
	ASSERT_TRUE( test(b<42>(7), 1));
	ASSERT_TRUE( test(b<42>(7), 2));
	ASSERT_FALSE(test(b<42>(7), 3));
	ASSERT_FALSE(test(b<42>(7), 4));

	{ // test range checking
		size_t const len = 444;
		b<len> a(std::string(len, '1'));
		RANGE_CHECK(a, len, test);
	}

	REPEAT(100,
		auto const r = random<149>();
		for(size_t ii=0; ii<size(r); ++ii)
			ASSERT_EQ(r[ii], test(r, ii));
	);
}

TEST(Bitset, Mask)
{
	ASSERT_EQ(b<42>(0x0),      mask(b<42>(0x00ff00), b<42>(0xff00ff)));
	ASSERT_EQ(b<42>(0x0f0f0f), mask(b<42>(0xffffff), b<42>(0x0f0f0f)));

	REPEAT(10000,
		auto const v = random<97>();
		auto const m = random<97>();
		ASSERT_EQ(v & m, mask(v, m));
	);
}

TEST(Bitset, Crop)
{
	ASSERT_EQ(b<8>(0x00),   crop<8>(b<42>(0x00ff00)));
	ASSERT_EQ(b<8>(0xff),   crop<8>(b<42>(0x00ff00), 8));
	ASSERT_EQ(b<16>(0x8001), crop<16>(b<42>(0x180010), 4));

	{ // test cropping out of bitset range
		std::string n(128, '1');
		ASSERT_EQ(128u, n.size());
		b<128> a(n);
		ASSERT_TRUE(test(a, 127));
		ASSERT_EQ(b<16>(0x0), crop<16>(a, 128));
	}

	REPEAT(1000,
		auto const r = random<73>();
		ASSERT_EQ(b<37>((r>>12).to_ullong()), crop<37>(r, 12));
	);
}

std::string gen_string(b<4> const& pattern, size_t repetitions)
{
	std::string s;
	while (repetitions) {
		s += pattern.to_string();
		repetitions--;
	}
	return s;
}

// regression test for issue #2
TEST(Bitset, CropRegression2)
{
	std::string n(128, '1');
	ASSERT_EQ(128u, n.size());
	ASSERT_EQ(b<8>(0xff), crop<8>(b<128>(n)));
	ASSERT_EQ(b<256>(n), crop<256>(b<128>(n)));

	for (size_t offset = 0; offset<128; ++offset)
	{
		ASSERT_EQ(b<256>(n, 0, 128-offset), crop<256>(b<128>(n, offset)));
	}

	 ASSERT_EQ(b<256>(gen_string(b<4>("0101"), 31)),
			   crop<256>(b<128>(gen_string(b<4>("0101"), 32), 4)));
}

TEST(Bitset, Flip)
{
	REPEAT(10000,
		auto const r = random<123>();
		ASSERT_EQ(b<123>(r).flip(), flip(r));

		for (size_t ii=0; ii<size(r); ++ii)
			ASSERT_NE(r[ii], flip(r, ii)[ii]);
	);
}

TEST(Bitset, Count)
{
	ASSERT_EQ(4u, count(b<42>(0x1111)));

	REPEAT(10000,
		auto const r = random<215>();
		ASSERT_EQ(_count(r), count(r));
	);
}

TEST(Bitset, Parity)
{
	ASSERT_TRUE( parity(b<42>(0x0000001)));
	ASSERT_TRUE( parity(b<42>(0x1111111)));
	ASSERT_FALSE(parity(b<42>(0x0000000)));
	ASSERT_FALSE(parity(b<42>(0x1010101)));

	REPEAT(10000,
		auto const r = random<215>();
		ASSERT_EQ(_parity(r), parity(r));
	);
}

TEST(Bitset, All)
{
	REPEAT(10000,
		auto const r = random<259>();
		ASSERT_EQ(r.all(), all(r));
	);
}

TEST(Bitset, Any)
{
	ASSERT_TRUE(any(b<42>(0x00010000)));
	ASSERT_FALSE(any(b<42>(0x0)));

	REPEAT(10000,
		auto const r = random<182>();
		ASSERT_EQ(r.any(), any(r));
	);
}

TEST(Bitset, None)
{
	ASSERT_TRUE(none(b<42>(0x0)));
	ASSERT_FALSE(none(b<42>(0x000100000)));

	REPEAT(10000,
		auto const r = random<162>();
		ASSERT_EQ(r.none(), none(r));
	);
}

TEST(Bitset, Reverse)
{
	ASSERT_EQ(b<42>(0), reverse(b<42>(0)));
	ASSERT_EQ(b<42>(0x2a800000000), reverse(b<42>(0x55)));
	ASSERT_EQ(b<42>(0xaaaaaaaaaaaaaaaa), reverse(b<42>(0x5555555555555555)));
	ASSERT_EQ(b<8>(0xaa), reverse(b<8>(0x55)));

	REPEAT(100,
		auto const i = random<113>();
		auto const r = reverse(i);

		ASSERT_EQ(i, reverse(reverse(i)));

		for(size_t ii=0; ii<size(i); ++ii)
			ASSERT_EQ(i[ii], r[size(i)-1-ii]);
	);
}

#include "bitter/util.h"

TEST(Bitset, Conversion)
{
	ASSERT_EQ(std::string(sizeof(int)*8, '0'), str(b<sizeof(int)*8>(0)));
	ASSERT_EQ(std::string(8, '1'), str<8>(b<sizeof(int)*8>(0xfeff)));
	ASSERT_EQ(0ull, convert(b<sizeof(int)*8>(0)));
	ASSERT_EQ((unsigned long long)0xff, convert(b<sizeof(int)*8>(0xff)));
	ASSERT_EQ((unsigned char)0, convert<unsigned char>(b<sizeof(char)*8>(0)));

	ASSERT_EQ(b<8>(0xff), convert<b<8>>(b<42>(0xff)));

	ASSERT_NO_THROW(convert(b<sizeof(unsigned long long)*8>(0x0)));
	// the following mustn't compile
	//convert(b<sizeof(unsigned long long)*8+1>(0x0)),
}

TEST(Bitset, Concat)
{
	REPEAT(10000,
		auto const r = random<size_t>(64);
		ASSERT_EQ(b<16>(r), concat(b<3>(r<<13), b<8>(r<<5), b<5>(r)));
		ASSERT_EQ(b<33>(r), concat(b<11>(r<<22), concat(b<11>(r<<11), b<11>(r))));
	);
}

TEST(Bitset, Extend)
{
	REPEAT(10000,
		size_t const r = rand();

		ASSERT_EQ(b< 16>(b< 8>(r).to_ulong()), extend< 16>(b<8>(r)));
		ASSERT_EQ(b< 32>(b<13>(r).to_ulong()), extend< 32>(b<13>(r)));
		ASSERT_EQ(b<128>(b<42>(r).to_ulong()), extend<128>(b<42>(r)));

		std::string s = random_bit_string(222);
		ASSERT_EQ(b<333>(s), extend<333>(b<223>(s)));
	);
}
