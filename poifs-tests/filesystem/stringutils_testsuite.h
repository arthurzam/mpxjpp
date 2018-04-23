#include <gtest/gtest.h>

#include "poifs/filesystem/stringutils.h"

namespace filesystem {

TEST(StringUtilsTest, getFromUnicodeLE) {
    using poifs::filesystem::string_utils::getFromUnicodeLE;

    std::vector<unsigned char> data{
        0x61, 0x00, 0x62, 0x00, 0x63, 0x00, // u"abc"
        0xe9, 0x05, 0xdc, 0x05, 0xd5, 0x05, 0xdd, 0x05, // u"שלום"
                                   };
    bytestream::bytestream stream(data);
    std::u16string res = getFromUnicodeLE(stream, 3);
    EXPECT_EQ(res, u"abc");
    EXPECT_EQ(stream.gcount(), 6);
    res = getFromUnicodeLE(stream, 4);
    EXPECT_EQ(res, u"שלום");
    EXPECT_EQ(stream.gcount(), 8);
    EXPECT_TRUE(stream.eof());
}

TEST(StringUtilsTest, getFromCompressedUnicode) {
    using poifs::filesystem::string_utils::getFromCompressedUnicode;

    std::vector<unsigned char> data{
        0x57, 0x6f, 0x77, // "Wow"
        0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x09, // "Hello World\t"
                                   };
    bytestream::bytestream stream(data);
    std::string res = getFromCompressedUnicode(stream, 3);
    EXPECT_EQ(res, "Wow");
    EXPECT_EQ(stream.gcount(), 3);
    res = getFromCompressedUnicode(stream, 12);
    EXPECT_EQ(res, "Hello World\t");
    EXPECT_EQ(stream.gcount(), 12);
    EXPECT_TRUE(stream.eof());
}

TEST(StringUtilsTest, to_utf16) {
    using poifs::filesystem::string_utils::to_utf16;

    EXPECT_EQ(to_utf16(u8""), u"");
    EXPECT_EQ(to_utf16(u8"Hello Universe"), u"Hello Universe");
    EXPECT_EQ(to_utf16(u8"שלום עולם"), u"שלום עולם");
}

TEST(StringUtilsTest, to_utf8) {
    using poifs::filesystem::string_utils::to_utf8;

    EXPECT_EQ(to_utf8(u""), u8"");
    EXPECT_EQ(to_utf8(u"Hello Universe"), u8"Hello Universe");
    EXPECT_EQ(to_utf8(u"שלום עולם"), u8"שלום עולם");
}

}
