#ifndef OBJECT_H
#define OBJECT_H

#include <cmath>
#include <memory>
#include <exception>
#include <vector>
#include <type_traits>

namespace mpxjpp {
namespace common {

struct any;

struct bad_any_cast
{ };

namespace anyimpl {
    /**
     * is_small type trait
     *
     * Checks if T is small. Enables stack memmory inside any object
     */
    template <typename T> constexpr bool is_small = (sizeof(T) <= sizeof(void *));

    /**
     * compare_to type trait
     *
     * Calls the appropriate compare functions.
     */
    template<typename T, typename = void>
    struct compare_to {
        static constexpr int type = 0;
        int operator()(const T &, const T &) const {
            throw std::invalid_argument("should not be called for this type");
        }
    };

    template<typename T>
    struct compare_to<T, std::enable_if_t<std::is_arithmetic<T>::value>> {
        static constexpr int type = 1;
        int operator()(T a, T b) const noexcept {
            return a - b;
        }
    };

    template<> struct compare_to<std::string> {
        static constexpr int type = 2;
        int operator()(const std::string &a, const std::string &b) const noexcept {
            return a.compare(b);
        }
    };

    template <typename T>
    struct has_compareTo_method
    {
        template <typename C> static auto test(T * p) -> decltype(std::declval<C>().compareTo(*p), std::true_type());

        template <typename> static std::false_type test(...);

        typedef decltype(test<T>(nullptr)) type;
        static constexpr bool value = std::is_same<std::true_type, decltype(test<T>(nullptr))>::value;
    };
    template<typename T>
    struct compare_to<T, std::enable_if_t<has_compareTo_method<T>::value>> {
        static constexpr int type = 3;
        int operator()(const T &a, const T &b) const {
            return a.compareTo(b);
        }
    };

    /**
     * is_movable type trait
     *
     * Checks if IS_MOVABLE is defined and is a true value.
     * This flag enables the any object to memcpy the memmory of the object.
     */
    template<typename T, typename = void>
    constexpr bool is_movable = std::is_trivial<T>::value;

    template <typename T>
    constexpr bool is_movable<T, std::enable_if_t<T::IS_MOVABLE>> = true;

    struct base_any_policy {
        virtual void static_delete(void** x) const = 0;
        virtual void copy_from_value(void const* src, void** dest) const = 0;
        virtual void clone(void* const* src, void** dest) const = 0;
        virtual void* get_value(void** src) const = 0;
        virtual int compareTo(void* const* x, void* const* y) const = 0;
    };

    template<typename T>
    struct small_any_policy final : base_any_policy {
        static_assert(sizeof(T) <= sizeof(void *), "not appropriate for small_any_policy");
        static constexpr bool is_small_policy = true;

        virtual void static_delete(void**) const override { }
        virtual void copy_from_value(void const* src, void** dest) const override
            { new(dest) T(*reinterpret_cast<T const*>(src)); }
        virtual void clone(void* const* src, void** dest) const override { *dest = *src; }
        virtual void* get_value(void** src) const override { return reinterpret_cast<void*>(src); }
        virtual int compareTo(void* const* x, void* const* y) const override
            { return compare_to<T>()(*(reinterpret_cast<T const*>(x)), *(reinterpret_cast<T const*>(y))); }
    };

    template<typename T>
    struct big_any_policy final : base_any_policy {
        static constexpr bool is_small_policy = false;

        virtual void static_delete(void** x) const override { if (*x) {
            delete(*reinterpret_cast<T**>(x)); } *x = NULL; }
        virtual void copy_from_value(void const* src, void** dest) const override {
           *dest = new T(*reinterpret_cast<T const*>(src)); }
        virtual void clone(void* const* src, void** dest) const override {
           *dest = new T(**reinterpret_cast<T* const*>(src)); }
        virtual void* get_value(void** src) const override { return *src; }
        virtual int compareTo(void* const* x, void* const* y) const override
            { return compare_to<T>()(*(reinterpret_cast<T const*>(*x)), *(reinterpret_cast<T const*>(*y))); }
    };

    template<typename T>
    struct choose_policy {
        using __cleaned_type = std::remove_cv_t<std::remove_reference_t<T>>;
        using type = typename std::conditional_t<std::is_pointer<T>::value, small_any_policy<void *>,
                              std::conditional_t<is_small<T>, small_any_policy<__cleaned_type>,
                                                                big_any_policy<__cleaned_type>>>;
    };


    /// Choosing the policy for an any type is illegal, but should never happen.
    /// This is designed to throw a compiler error.
    template<>
    struct choose_policy<any> {
        using type = void;
    };

    static_assert(choose_policy<int>::type::is_small_policy, "a");
    static_assert(!choose_policy<std::string>::type::is_small_policy, "a");
    static_assert(choose_policy<const char *>::type::is_small_policy &&
                  std::is_same<choose_policy<const char *>::type, small_any_policy<void *>>::value, "a");

    /// This function will return a different policy for each type.
    template<typename T>
    const base_any_policy* get_policy() noexcept {
        static const typename choose_policy<T>::type policy;
        return &policy;
    }
}

struct any final
{
private:
    struct empty_any
    { int compareTo(const empty_any &) const { return 0; } };
    static_assert(anyimpl::compare_to<empty_any>::type == 3, "a");
    // fields
    const anyimpl::base_any_policy* policy = anyimpl::get_policy<empty_any>();
    void* object = nullptr;

public:
    template <typename T, std::size_t N>
    any(const std::array<T, N>& x) {
        assign(std::vector<T>(x.begin(), x.end()));
    }

    /// Initializing constructor.
    template <typename T>
    any(const T& x) {
        assign(x);
    }

    /// Empty constructor.
    any() noexcept
    { }

    /// Special initializing constructor for string literals.
    any(const char* x) {
        assign(x);
    }

    /// Copy constructor.
    any(const any& x) :
        policy(x.policy) {
        policy->clone(&x.object, &object);
    }

    /// Move constructor.
    any(any&& x) noexcept {
        swap(x);
    }

    /// Destructor.
    ~any() {
        policy->static_delete(&object);
    }

    /// Assignment function from another any.
    any& assign(const any& x) {
        reset();
        policy = x.policy;
        policy->clone(&x.object, &object);
        return *this;
    }

    /// Assignment function from another move any.
    any& assign(any&& x) noexcept {
        return swap(x);
    }

    /// Assignment function.
    template <typename T>
    any& assign(const T& x) {
        static_assert(!std::is_same<T, any>::value, "this function shouldn't be called with any");
        reset();
        policy = anyimpl::get_policy<T>();
        policy->copy_from_value(&x, &object);
        return *this;
    }

    /// Assignment operator.
    template<typename T, std::enable_if_t<!std::is_same<T, any>::value>>
    any& operator=(const T& x) {
        return assign<T>(x);
    }

    /// Assignment operator.
    any& operator=(const any &x) {
        return assign(x);
    }
    any& operator=(any &&x) noexcept {
        return swap(x);
    }

    /// Assignment operator, specialed for literal strings.
    /// They have types like const char [6] which don't work as expected.
    any& operator=(const char* x) {
        return assign(x);
    }

    /// Utility functions
    any& swap(any& x) noexcept {
        std::swap(policy, x.policy);
        std::swap(object, x.object);
        return *this;
    }

    /// Cast operator. You can only cast to the original type.
    template<typename T>
    T& cast() {
        if (policy != anyimpl::get_policy<T>())
            throw bad_any_cast();
        T* r = reinterpret_cast<T*>(policy->get_value(&object));
        return *r;
    }

    template<typename T>
    const T& cast() const {
        if (policy != anyimpl::get_policy<T>())
            throw bad_any_cast();
        T* r = reinterpret_cast<T*>(policy->get_value(const_cast<void**>(&object)));
        return *r;
    }

    template<typename T>
    const T& cast(const T &def) const {
        if (empty())
            return def;
        return cast<T>();
    }

    /// Returns true if the any contains no value.
    bool empty() const noexcept {
        return policy == anyimpl::get_policy<empty_any>();
    }

    /// Frees any allocated memory, and sets the value to NULL.
    void reset() {
        policy->static_delete(&object);
        policy = anyimpl::get_policy<empty_any>();
    }

    /// Returns true if the two types are the same.
    bool compatible(const any& x) const noexcept {
        return policy == x.policy;
    }

    int compareTo(const any &x) const {
        if (policy != x.policy)
            throw bad_any_cast();
        return policy->compareTo(&object, &x.object);
    }

    template<typename T>
    bool isType() const noexcept {
        return policy == anyimpl::get_policy<T>();
    }

    bool operator ==(const any &x) const {
        return !compareTo(x);
    }
};

template<typename T, typename = void>
struct any_type_cast {
    using type = T;
    using castType = std::remove_cv_t<std::remove_reference_t<T>>;

    static type get(const any &a, std::add_lvalue_reference_t<std::add_const_t<castType>> def) {
        return static_cast<type>(a.cast<castType>(def));
    }

    static void set(any &a, type var) {
        a.assign<castType>(static_cast<castType>(var));
    }
};

template<typename T>
struct any_type_cast<T, std::enable_if_t<std::is_enum<T>::value ||
        ((anyimpl::is_small<T>) && std::is_arithmetic<T>::value) >> {
    using type = T;
    using castType = std::conditional_t<std::is_signed<T>::value,intptr_t,uintptr_t>;

    static_assert(sizeof(T) <= sizeof(castType), "test here for sizes");

    static type get(const any &a, castType def) {
        return static_cast<type>(a.cast<castType>(def));
    }

    static void set(any &a, type var) {
        a.assign<castType>(static_cast<castType>(var));
    }
};

template<>
struct any_type_cast<any> {
    using type = void;
    using castType = void;

    using get = void;
    using set = void;
};

}
}

#endif // OBJECT_H
