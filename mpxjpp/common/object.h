#ifndef OBJECT_H
#define OBJECT_H

#include <cmath>
#include <memory>
#include <exception>
#include <vector>
#include <type_traits>
#include <typeinfo>

#include "common.h"

namespace mpxjpp {
namespace common {

struct any;

struct bad_any_cast
{ };

namespace anyimpl {
    template<typename _Tp> using remove_ref_cv_t = std::remove_cv_t<std::remove_reference_t<_Tp>>;

    union storage_union {
        using stack_storage_t = typename std::aligned_storage<sizeof(void *), std::alignment_of<void *>::value>::type;

        void           *dynamic;
        stack_storage_t stack;
    };

    /**
     * is_small type trait
     *
     * Checks if T is small. Enables stack memmory inside any object
     */
    template <typename T> constexpr bool is_small = (sizeof(T) <= sizeof(storage_union::stack_storage_t));

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

    template<typename T>
    struct compare_to<T, details::void_t<decltype(std::declval<T>().compareTo(std::declval<T>()))>> {
        static constexpr int type = 3;
        int operator()(const T &a, const T &b) const {
            return a.compareTo(b);
        }
    };

    template<> struct compare_to<std::nullptr_t> {
        static constexpr int type = 4;
        int operator()(std::nullptr_t, std::nullptr_t) const noexcept {
            return 0;
        }
    };

    struct vtable {
        const std::type_info &(*type)();
        void(*destroy)(storage_union &);
        void(*copy)(const storage_union &src, storage_union &dst);
        int (*compareTo)(const storage_union &a, const storage_union &b);

        template <typename T>
        static const std::type_info &get_type() {
            return typeid(T);
        }
    };

    template <typename T> struct vtable_dynamic {
        static void destroy(storage_union &storage) {
            delete reinterpret_cast<T *>(storage.dynamic);
        }

        static void copy(const storage_union &src, storage_union &dst) {
            dst.dynamic = new T(get_value(src));
        }

        static int compareTo(const storage_union &a, const storage_union &b) {
            return compare_to<T>()(get_value(a), get_value(b));
        }

        inline static T &get_value(storage_union &storage) noexcept {
            return *reinterpret_cast<T *>(storage.dynamic);
        }
        inline static const T &get_value(const storage_union &storage) noexcept {
            return *reinterpret_cast<const T *>(storage.dynamic);
        }

        template <typename ValueType>
        inline static void init(storage_union &storage, ValueType &&value) {
            storage.dynamic = new T(std::forward<ValueType>(value));
        }
    };

    template <typename T> struct vtable_stack {
        static_assert (is_small<T>, "T should be a small object");

        static void destroy(storage_union &storage) {
            reinterpret_cast<T *>(&storage.stack)->~T();
        }

        static void copy(const storage_union &src, storage_union &dst) {
            new (&dst.stack) T(get_value(src));
        }

        static int compareTo(const storage_union &a, const storage_union &b) {
            return compare_to<T>()(get_value(a), get_value(b));
        }

        inline static T &get_value(storage_union &storage) noexcept {
            return reinterpret_cast<T &>(storage.stack);
        }
        inline static const T &get_value(const storage_union &storage) noexcept {
            return reinterpret_cast<const T &>(storage.stack);
        }

        template <typename ValueType>
        inline static void init(storage_union &storage, ValueType &&value) {
            new (&storage.stack) T(std::forward<ValueType>(value));
        }
    };

    template<typename T>
    struct choose_policy {
        using __cleaned_type = remove_ref_cv_t<T>;
        using type = typename std::conditional_t<is_small<T>, vtable_stack<__cleaned_type>,
                                                            vtable_dynamic<__cleaned_type>>;
    };

    /// This function will return a different policy for each type.
    template<typename T>
    const vtable* get_policy() noexcept {
        static_assert(!std::is_same<remove_ref_cv_t<T>, any>::value, "T shouldn't be from any type");

        using VType = typename choose_policy<T>::type;
        static const vtable policy = {
            vtable::get_type<T>, VType::destroy,
            VType::copy, VType::compareTo
        };
        return &policy;
    }
}  // namespace anyimpl

struct any final
{
private:
    // fields

    template <typename T>
    using policy_type = typename anyimpl::choose_policy<T>::type;

    const anyimpl::vtable* policy = anyimpl::get_policy<std::nullptr_t>();
    anyimpl::storage_union object;

    template <typename T>
    static constexpr bool is_any = std::is_same<anyimpl::remove_ref_cv_t<T>, any>::value;
public:
    template <typename T, std::size_t N>
    any(const std::array<T, N>& x) {
        assign(std::vector<T>(x.begin(), x.end()));
    }

    /// Initializing constructor.
    template <typename T>
    any(const T& x) {
        static_assert(!is_any<T>, "this function shouldn't be called with any");
        assign(x);
    }

    /// Empty constructor.
    any() noexcept
    { }

    explicit any(std::nullptr_t) noexcept
    { }

    /// Special initializing constructor for string literals.
    any(const char* x) {
        assign(x);
    }

    /// Copy constructor.
    any(const any& x) :
        policy(x.policy) {
        policy->copy(x.object, object);
    }

    /// Move constructor.
    any(any&& x) noexcept {
        swap(x);
    }

    /// Destructor.
    ~any() {
        policy->destroy(object);
    }

    /// Assignment function from another any.
    any& assign(const any& x) {
        policy->destroy(object);
        policy = x.policy;
        policy->copy(x.object, object);
        return *this;
    }

    /// Assignment function from another move any.
    any& assign(any&& x) noexcept {
        return swap(x);
    }

    /// Assignment function.
    template <typename T>
    auto assign(T&& x) -> std::enable_if_t<!is_any<T>, any &> {
        static_assert(!is_any<T>, "this function shouldn't be called with any");
        policy->destroy(object);
        policy = anyimpl::get_policy<anyimpl::remove_ref_cv_t<T>>();
        policy_type<T>::init(object, std::forward<T>(x));
        return *this;
    }

    /// Assignment operator.
    template<typename T, std::enable_if_t<!(is_any<T>)>>
    any& operator=(T&& x) {
        return assign(std::forward<T>(x));
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

    any& operator=(std::nullptr_t) {
        reset();
        return *this;
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
        if (!isType<T>())
            throw bad_any_cast();
        return policy_type<T>::get_value(object);
    }

    template<typename T>
    const T& cast() const {
        if (!isType<T>())
            throw bad_any_cast();
        return policy_type<T>::get_value(object);
    }

    /// Assign if empty and return value. You can only cast to the original type.
    template<typename T>
    T& get_assign(const T &def) {
        if (empty())
            assign(def);
        return cast<T>();
    }

    /// Get value or the default if empty. You can only cast to the original type.
    template<typename T>
    const T& get(const T &def) const {
        if (empty())
            return def;
        return cast<T>();
    }

    /// Returns true if the any contains no value.
    bool empty() const noexcept {
        return isType<std::nullptr_t>();
    }

    /// Frees any allocated memory, and sets the value to NULL.
    void reset() {
        policy->destroy(object);
        policy = anyimpl::get_policy<std::nullptr_t>();
    }

    /// Returns true if the two types are the same.
    bool compatible(const any& x) const noexcept {
        return policy == x.policy;
    }

    int compareTo(const any &x) const {
        if (policy != x.policy)
            throw bad_any_cast();
        return policy->compareTo(object, x.object);
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
    using castType = anyimpl::remove_ref_cv_t<T>;

    static type get(const any &a, std::add_lvalue_reference_t<std::add_const_t<castType>> def) {
        return static_cast<type>(a.get<castType>(def));
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
        return static_cast<type>(a.get<castType>(def));
    }

    static void set(any &a, type var) {
        a.assign<castType>(static_cast<castType>(var));
    }
};

template<typename T>
struct any_type_cast<T, std::enable_if_t<std::is_pointer<T>::value >> {
    using type = T;
    using castType = void *;

    static type get(const any &a, castType def) {
        return static_cast<type>(a.get<castType>(def));
    }

    static void set(any &a, type var) {
        a.assign<castType>(const_cast<void *>(static_cast<const void *>(var)));
    }
};

template<typename T>
struct any_type_cast<T, details::void_t<typename T::ANY_TYPE_CAST>> {
    using type = T;
    using castType = typename T::ANY_TYPE_CAST;

    static type get(const any &a, castType def) {
        return static_cast<type>(a.get<castType>(def));
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

}  // namespace common
}  // namespace mpxjpp

#endif // OBJECT_H
