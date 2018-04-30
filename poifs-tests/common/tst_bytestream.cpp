#include <gtest/gtest.h>

#include "poifs/common/bytestream.h"

using bstream = bytestream::bytestream;
using data = std::vector<unsigned char>;

TEST(ByteStreamTest, Empty) {
    data d;
    bstream stream(d);
    EXPECT_TRUE(stream.eof());
}

TEST(ByteStreamTest, Peek) {
    data d{0x1F};
    bstream stream(d);
    EXPECT_FALSE(stream.eof());
    EXPECT_EQ(stream.peek(), 0x1F);
}

TEST(ByteStreamTest, Available) {
    {
        data d{0x1F};
        bstream stream(d);
        EXPECT_EQ(stream.available(), 1);
    }
    {
        data d{0x1F, 0x00, 0x15, 0xAE};
        bstream stream(d);
        EXPECT_EQ(stream.available(), 4);
    }
}

TEST(ByteStreamTest, Seek_Tell) {
    data d{0x7A, 0x5D, 0x74, 0xEE, 0x3B, 0x73, 0x2C, 0x15, 0x7E, 0x1F};
    bstream stream(d);

    EXPECT_EQ(stream.tellg(), 0);
    EXPECT_EQ(stream.available(), 10);

    EXPECT_EQ(stream.seekg(2).available(), 8);
    EXPECT_EQ(stream.seekg(4).available(), 6);
    EXPECT_EQ(stream.seekg(4, std::ios::cur).available(), 2);
    EXPECT_EQ(stream.seekg(2, std::ios::cur).available(), 0);
    EXPECT_TRUE(stream.eof());

    EXPECT_EQ(stream.seekg(2, std::ios::beg).available(), 8);
    EXPECT_EQ(stream.tellg(), 2);
    EXPECT_TRUE(stream.seekg(10, std::ios::beg).eof());
    EXPECT_EQ(stream.tellg(), 10);
    EXPECT_THROW(stream.seekg(-1, std::ios::beg), std::invalid_argument);
    EXPECT_THROW(stream.seekg(11, std::ios::beg), std::invalid_argument);

    EXPECT_EQ(stream.seekg(-1, std::ios::end).available(), 1);
    EXPECT_EQ(stream.tellg(), 9);
    EXPECT_TRUE(stream.seekg(0, std::ios::end).eof());
    EXPECT_THROW(stream.seekg(1, std::ios::end), std::invalid_argument);
    EXPECT_THROW(stream.seekg(-11, std::ios::end), std::invalid_argument);
}

TEST(ByteStreamTest, Read_gCount) {
    data d{0xB9, 0x3D, 0xC5, 0x47, 0xE8, 0xC7, 0xD5, 0x9B, 0xC5, 0x56,
           0x59, 0x8D, 0x19, 0xD1, 0x12, 0x61, 0xD5, 0x87, 0x30, 0x75};
    data dst(20, '\0');
    bstream stream(d);

    auto equalRange = [&d, &dst] (unsigned count, unsigned reletive = 0) {
        return std::equal(dst.cbegin(), dst.cbegin() + count, d.cbegin() + reletive, d.cbegin() + count + reletive);
    };

    EXPECT_EQ(stream.available(), 20);

    EXPECT_EQ(stream.read(dst.data(), 5).available(), 15);
    EXPECT_TRUE(equalRange(5));
    EXPECT_EQ(stream.gcount(), 5);

    EXPECT_EQ(stream.read(dst.data(), 5).available(), 10);
    EXPECT_TRUE(equalRange(5, 5));
    EXPECT_EQ(stream.gcount(), 5);

    EXPECT_EQ(stream.seekg(1).available(), 19);
    EXPECT_EQ(stream.gcount(), 0);
    EXPECT_EQ(stream.read(dst.data(), 10).available(), 9);
    EXPECT_TRUE(equalRange(10, 1));
    EXPECT_EQ(stream.gcount(), 10);

    EXPECT_EQ(stream.read(dst.data(), 10).available(), 0);
    EXPECT_TRUE(stream.eof());
    EXPECT_EQ(stream.gcount(), 9);
    EXPECT_TRUE(equalRange(9, 11));
}

TEST(ByteStreamTest, StreamOperator) {
    data d{0x59, 0x4C, 0xE6, 0xDB, 0x95, 0x19, 0x8C, 0x27, 0x5C, 0x3F,
           0xDC, 0xEF, 0xF2, 0xB1, 0x13, 0x56, 0x33, 0x33, 0xA3, 0x40,
           0x69, 0xAA, 0x8B, 0x17, 0xF3, 0x00, 0x2D, 0x4F, 0x2E, 0x23,
           0x82, 0xA8, 0xCF, 0x2D, 0xEB, 0x1F, 0x7C, 0xB3, 0xB4, 0x1B};
    bstream stream(d);
    EXPECT_EQ(stream.available(), 40);

    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    float f;

    stream >> u8 >> u8 >> u16 >> u32 >> u64 >> f;
    EXPECT_EQ(u8, 0x4C);
    EXPECT_EQ(u16, 0xDBE6);
    EXPECT_EQ(u32, 0x278C1995);
    EXPECT_EQ(u64, 0x5613B1F2EFDC3F5C);
    EXPECT_EQ(f, 5.1f); // 0x33, 0x33, 0xA3, 0x40

    std::array<unsigned char, 10> arr;
    data vec(10, '\0');
    stream >> arr >> vec;
    EXPECT_TRUE(std::equal(arr.cbegin(), arr.cend(), d.cbegin() + 20));
    EXPECT_TRUE(std::equal(vec.cbegin(), vec.cend(), d.cbegin() + 30));
    EXPECT_TRUE(stream.eof());

    EXPECT_EQ(stream.gcount(), 10);
    stream.unread();
    EXPECT_EQ(stream.tellg(), 30);
}
