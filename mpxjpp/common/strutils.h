#ifndef STRUTILS_H
#define STRUTILS_H

#include <string>
#include <algorithm>

namespace mpxjpp {
namespace common {
namespace strutils {

inline std::string str_toupper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::toupper(c); } // correct
                  );
    return s;
}

}
}
}
#endif // STRUTILS_H
