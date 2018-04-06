#ifndef COMMON_H
#define COMMON_H

namespace mpxjpp {
namespace common {
namespace details {

template<typename... Ts> struct make_void { typedef void type;};
template<typename... Ts> using void_t = typename make_void<Ts...>::type;

}
}
}

#endif // COMMON_H
