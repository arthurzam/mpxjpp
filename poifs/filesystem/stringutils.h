#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>

#include "../common/bytestream.h"

namespace poifs {
namespace filesystem {
namespace string_utils {

std::u16string getFromUnicodeLE(bytestream::bytestream &stream, std::size_t len);
std::string getFromCompressedUnicode(bytestream::bytestream &stream, std::size_t len);

std::u16string to_utf16(const std::string &str);
std::string to_utf8(const std::u16string &str);

}
}
}
#endif // STRINGUTILS_H
