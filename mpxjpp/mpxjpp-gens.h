#ifndef MPXJPPGENS_H
#define MPXJPPGENS_H

#define MPXJPP_GETTER(varName, type) \
    type varName() const { \
        return m_##varName; \
    }

#define MPXJPP_SETTER(varName, type) \
    void set_##varName(type varName) { \
        m_##varName = varName; \
    }

#define MPXJPP_GETTER_SETTER(varName, type) \
    MPXJPP_GETTER(varName, type) \
    MPXJPP_SETTER(varName, type)

#define MPXJPP_GETTER_BITS(fieldName, host, mask) \
    bool fieldName() const { \
        return (host & mask) != 0; \
    }

#define MPXJPP_SETTER_BITS(fieldName, host, mask) \
    void set_##fieldName(bool fieldName) { \
        if (fieldName) \
            host |= mask; \
        else \
            host &= ~mask; \
    }

#define MPXJPP_GETTER_SETTER_BITS(fieldName, host, mask) \
    MPXJPP_GETTER_BITS(fieldName, host, mask) \
    MPXJPP_SETTER_BITS(fieldName, host, mask)

#define MPXJPP_GEN_ENUM(name, ...) name
#define MPXJPP_GEN_VALUE(name, ...) {__VA_ARGS__}
#define MPXJPP_GEN_VALUE_STR(name, ...) {#name, __VA_ARGS__}

#endif // MPXJPPGENS_H
