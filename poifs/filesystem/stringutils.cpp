#include "stringutils.h"

#include <iostream>
#include <codecvt>
#include <string>
#include <locale>
#include <iomanip>

using namespace poifs::filesystem;

using std::string;
using std::u16string;

static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> g_convert_UTF16_UTF8;

u16string string_utils::getFromUnicodeLE(bytestream::bytestream &stream, std::size_t len) {
    u16string res(len, '\0');
    if (stream.available() < 2 * len)
        throw std::invalid_argument("Illegal length");
    stream.read(const_cast<char16_t *>(res.data()), 2 * len);
    return res;
}

std::string string_utils::getFromCompressedUnicode(bytestream::bytestream &stream, std::size_t len) {
    string res(len, '\0');
    if (stream.available() < len)
        throw std::invalid_argument("Illegal length");
    stream.read(const_cast<char *>(res.data()), len);
    return res;
}

std::u16string string_utils::to_utf16(const std::string &str)
{
    return g_convert_UTF16_UTF8.from_bytes(str);
}

std::string string_utils::to_utf8(const std::u16string &str)
{
    return g_convert_UTF16_UTF8.to_bytes(str);
}
