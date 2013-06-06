#include <gtest/gtest.h>

#include <bitter/bitset.h>

using namespace bit;

typedef std::bitset<42> bt42;
typedef std::bitset<8>  bt8;

TEST(Bitset, Set)
{
	bt42 n(0);

	n = set(n, 1);

	for (size_t ii=0; ii < bit::size(n); ++ii) {
		if (ii == 1)
			ASSERT_TRUE(test(n, ii));
		else
			ASSERT_FALSE(test(n, ii));
	}

	ASSERT_EQ(bt42(7), set(bt42(0), 0,1,2));
	ASSERT_EQ(bt42(0), set(bt42(1), 0, false));
}

TEST(Bitset, Reset)
{
	ASSERT_EQ(bt42(0), reset(bt42(1), 0));
	ASSERT_EQ(bt42(0), reset(bt42(7), 0,1,2));

	bt42 n = 0x87;
	n    = reset(n, 7);

	ASSERT_EQ(bt42(0x07), n);
	ASSERT_EQ(n, flip(bt42(0x87), 7));
}

TEST(Bitset, Test)
{
	ASSERT_TRUE( test(bt42(7), 0));
	ASSERT_TRUE( test(bt42(7), 1));
	ASSERT_TRUE( test(bt42(7), 2));
	ASSERT_FALSE(test(bt42(7), 3));
	ASSERT_FALSE(test(bt42(7), 4));
}

TEST(Bitset, Mask)
{
	ASSERT_EQ(bt42(0x0),      mask(bt42(0x00ff00), bt42(0xff00ff)));
	ASSERT_EQ(bt42(0x0f0f0f), mask(bt42(0xffffff), bt42(0x0f0f0f)));
}

TEST(Bitset, Crop)
{
	ASSERT_EQ(bt8(0x00),   crop<8>(bt42(0x00ff00)));
	ASSERT_EQ(bt8(0xff),   crop<8>(bt42(0x00ff00), 8));
	ASSERT_EQ(bitset<16>(0x8001), crop<16>(bt42(0x180010), 4));
}

std::string gen_string(std::bitset<4> const& pattern, size_t repetitions)
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
	ASSERT_EQ(128, n.size());
	ASSERT_EQ(bitset<8>(0xff), crop<8>(bitset<128>(n)));
	ASSERT_EQ(bitset<256>(n), crop<256>(bitset<128>(n)));

	for (size_t offset = 0; offset<128; ++offset)
	{
		ASSERT_EQ(bitset<256>(n, 0, 128-offset), crop<256>(bitset<128>(n, offset)));
	}

	typedef std::bitset<4> bt4;
	 ASSERT_EQ(bitset<256>(gen_string(bt4("0101"), 31)),
			   crop<256>(bitset<128>(gen_string(bt4("0101"), 32), 4)));
}

TEST(Bitset, Flip)
{
	bt42 x;

	ulong i = 0xffffffffffffff;

	x = flip(bt42(0));
	ASSERT_EQ(bt42(i), x);

	bt8 x8 = flip(bt8(1));
	ASSERT_EQ(bt8(0xff-1), x8);

	x = flip(bt42(i));
	ASSERT_EQ(bt42(0), x);
}

TEST(Bitset, Count)
{
	ASSERT_EQ(4, count(bt42(0x1111)));
}

TEST(Bitset, Parity)
{
	ASSERT_TRUE( parity(bt42(0x0000001)));
	ASSERT_TRUE( parity(bt42(0x1111111)));
	ASSERT_FALSE(parity(bt42(0x0000000)));
	ASSERT_FALSE(parity(bt42(0x1010101)));
}

TEST(Bitset, All)
{
	ASSERT_TRUE(all(bt8(0xff)));
	ASSERT_FALSE(all(bt42(0xff)));
}

TEST(Bitset, Any)
{
	ASSERT_TRUE(any(bt42(0x00010000)));
	ASSERT_FALSE(any(bt42(0x0)));
}

TEST(Bitset, None)
{
	ASSERT_TRUE(none(bt42(0x0)));
	ASSERT_FALSE(none(bt42(0x000100000)));
}

TEST(Bitset, Reverse)
{
	ASSERT_EQ(bt42(0), reverse(bt42(0)));
	ASSERT_EQ(bt42(0x2a800000000), reverse(bt42(0x55)));
	ASSERT_EQ(bt42(0xaaaaaaaaaaaaaaaa), reverse(bt42(0x5555555555555555)));
	ASSERT_EQ(bt8(0xaa), reverse(bt8(0x55)));
}

#include "bitter/util.h"

TEST(Bitset, Conversion)
{
	ASSERT_EQ(std::string(sizeof(int)*8, '0'), str(std::bitset<sizeof(int)*8>(0)));
	ASSERT_EQ(std::string(8, '1'), str<8>(std::bitset<sizeof(int)*8>(0xfeff)));
	ASSERT_EQ(0,    convert(std::bitset<sizeof(int)*8>(0)));
	ASSERT_EQ(0xff, convert(std::bitset<sizeof(int)*8>(0xff)));
	ASSERT_EQ((unsigned char)0, convert<unsigned char>(std::bitset<sizeof(char)*8>(0)));

	ASSERT_EQ(bt8(0xff), convert<bt8>(bt42(0xff)));

	ASSERT_NO_THROW(convert(std::bitset<sizeof(unsigned long long)*8>(0x0)));
	// the following mustn't compile
	//convert(std::bitset<sizeof(unsigned long long)*8+1>(0x0)),
}

TEST(Bitset, Concat)
{
	ASSERT_EQ(bitset<16>(0x1155), concat(bt8(0x11), bt8(0x55)));
	ASSERT_EQ(bitset<32>(0x11551155), concat(bt8(0x11), bt8(0x55), concat(bt8(0x11), bt8(0x55))));
}
