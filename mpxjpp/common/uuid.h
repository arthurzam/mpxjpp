#ifndef UUID_H
#define UUID_H

#include <cstdint>

namespace mpxjpp {
namespace common {

// Use from link: http://grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/6-b14/java/util/UUID.java

struct UUID final {
public:
    uint64_t mostSigBits = 0;
    uint64_t leastSigBits = 0;

    constexpr UUID() = default;

    constexpr UUID(uint64_t mostSigBits, uint64_t leastSigBits) :
        mostSigBits(mostSigBits), leastSigBits(leastSigBits)
    { }

    constexpr uint8_t operator [] (unsigned index) const {
        index = (index & 0b1111);
        uint64_t bits = (index & 0b1000) ? mostSigBits : leastSigBits;
        uint64_t move = static_cast<uint64_t>(index & 0b0111) << 3;
        return (bits & (static_cast<uint64_t>(0xFF) << move)) >> move;
    }

    constexpr int compareTo(UUID x) const {
        return mostSigBits == x.mostSigBits ? leastSigBits - x.leastSigBits : mostSigBits - x.mostSigBits;
    }
};

}
}

namespace std {
template<>
struct hash<mpxjpp::common::UUID> {
    size_t operator()(const mpxjpp::common::UUID &val) const {
        return static_cast<size_t>(
                    (val.mostSigBits >> 32) ^ val.mostSigBits ^
                    (val.leastSigBits >> 32) ^ val.leastSigBits
                    );
    }
};
}

#endif // UUID_H
