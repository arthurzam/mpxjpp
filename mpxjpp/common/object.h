#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <exception>
#include <vector>

namespace mpxjpp {
namespace common {

namespace anyimpl {
	template<typename T>
	struct compare_to_func {
		int operator()(const T &a, const T &b) {
			return a.compareTo(b);
		}
	};
	template<>
	struct compare_to_func<std::string> {
		int operator()(const std::string &a, const std::string &b) {
			return a.compare(b);
		}
	};
	template<typename T>
	struct compare_to_func<std::vector<T>> {
		int operator()(const std::vector<T> &a, const std::vector<T> &b) {
			return !(a == b);
		}
	};
	template<>
	struct compare_to_func<double> {
		int operator()(double a, double b) {
			return a - b;
		}
	};

	struct bad_any_cast
	{ };

	struct empty_any
	{ int compareTo(const empty_any &) const { return 0; } };

	struct base_any_policy {
		virtual void static_delete(void** x) = 0;
		virtual void copy_from_value(void const* src, void** dest) = 0;
		virtual void clone(void* const* src, void** dest) = 0;
		virtual void move(void* const* src, void** dest) = 0;
		virtual void* get_value(void** src) = 0;
		virtual size_t get_size() = 0;
		virtual int compareTo(void* const* x, void* const* y) = 0;
	};

	template<typename T>
	struct typed_base_any_policy : base_any_policy {
		virtual size_t get_size() { return sizeof(T); }
	};

	template<typename T>
	struct small_any_policy final : typed_base_any_policy<T> {
		virtual void static_delete(void**) override { }
		virtual void copy_from_value(void const* src, void** dest) override
			{ new(dest) T(*reinterpret_cast<T const*>(src)); }
		virtual void clone(void* const* src, void** dest) override { *dest = *src; }
		virtual void move(void* const* src, void** dest) override { *dest = *src; }
		virtual void* get_value(void** src) override { return reinterpret_cast<void*>(src); }
		virtual int compareTo(void* const* x, void* const* y) override
			{ return *(reinterpret_cast<T const*>(x)) - *(reinterpret_cast<T const*>(y)); }
	};

	template<typename T>
	struct big_any_policy final : typed_base_any_policy<T> {
		virtual void static_delete(void** x) override { if (*x) {
			delete(*reinterpret_cast<T**>(x)); } *x = NULL; }
		virtual void copy_from_value(void const* src, void** dest) override {
		   *dest = new T(*reinterpret_cast<T const*>(src)); }
		virtual void clone(void* const* src, void** dest) override {
		   *dest = new T(**reinterpret_cast<T* const*>(src)); }
		virtual void move(void* const* src, void** dest) override {
		  (*reinterpret_cast<T**>(dest))->~T();
		  **reinterpret_cast<T**>(dest) = **reinterpret_cast<T* const*>(src); }
		virtual void* get_value(void** src) override { return *src; }
		virtual int compareTo(void* const* x, void* const* y) override
			{ return compare_to_func<T>()(*(reinterpret_cast<T const*>(x)), *(reinterpret_cast<T const*>(y))); }
	};

	template<typename T>
	struct choose_policy {
		typedef big_any_policy<T> type;
	};

	template<typename T>
	struct choose_policy<T*> {
		typedef small_any_policy<T*> type;
	};

	struct any;

	/// Choosing the policy for an any type is illegal, but should never happen.
	/// This is designed to throw a compiler error.
	template<>
	struct choose_policy<any> {
		typedef void type;
	};

	/// Specializations for small types.
	#define SMALL_POLICY(TYPE) template<> struct \
	   choose_policy<TYPE> { typedef small_any_policy<TYPE> type; };

	SMALL_POLICY(signed char)
	SMALL_POLICY(unsigned char)
	SMALL_POLICY(signed short)
	SMALL_POLICY(unsigned short)
	SMALL_POLICY(signed int)
	SMALL_POLICY(unsigned int)
	SMALL_POLICY(signed long)
	SMALL_POLICY(unsigned long)
	SMALL_POLICY(float)
	SMALL_POLICY(bool)

	#undef SMALL_POLICY

	/// This function will return a different policy for each type.
	template<typename T>
	base_any_policy* get_policy() {
		static typename choose_policy<T>::type policy;
		return &policy;
	}
}

struct any
{
private:
	// fields
	anyimpl::base_any_policy* policy;
	void* object = nullptr;

public:
	template <typename T, std::size_t N>
	any(const std::array<T, N>& x)
		: policy(anyimpl::get_policy<anyimpl::empty_any>())
	{
		assign(std::vector<T>(x.begin(), x.end()));
	}

	/// Initializing constructor.
	template <typename T>
	any(const T& x)
		: policy(anyimpl::get_policy<anyimpl::empty_any>())
	{
		assign(x);
	}

	/// Empty constructor.
	any()
		: policy(anyimpl::get_policy<anyimpl::empty_any>())
	{ }

	/// Special initializing constructor for string literals.
	any(const char* x)
		: policy(anyimpl::get_policy<anyimpl::empty_any>())
	{
		assign(x);
	}

	/// Copy constructor.
	any(const any& x)
		: policy(anyimpl::get_policy<anyimpl::empty_any>())
	{
		assign(x);
	}

	/// Move constructor.
	any(any&& x)
		: policy(anyimpl::get_policy<anyimpl::empty_any>())
	{
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
	any& assign(any&& x) {
		return swap(x);
	}

	/// Assignment function.
	template <typename T>
	any& assign(const T& x) {
		reset();
		policy = anyimpl::get_policy<T>();
		policy->copy_from_value(&x, &object);
		return *this;
	}

	/// Assignment operator.
	template<typename T>
	any& operator=(const T& x) {
		return assign(x);
	}

	/// Assignment operator.
	any& operator=(any&& x) {
		return swap(x);
	}
	any& operator=(const any& x) {
		return assign(x);
	}

	/// Assignment operator, specialed for literal strings.
	/// They have types like const char [6] which don't work as expected.
	any& operator=(const char* x) {
		return assign(x);
	}

	/// Utility functions
	any& swap(any& x) {
		std::swap(policy, x.policy);
		std::swap(object, x.object);
		return *this;
	}

	/// Cast operator. You can only cast to the original type.
	template<typename T>
	T& cast() {
		if (policy != anyimpl::get_policy<T>())
			throw anyimpl::bad_any_cast();
		T* r = reinterpret_cast<T*>(policy->get_value(&object));
		return *r;
	}

	template<typename T>
	const T& cast() const {
		if (policy != anyimpl::get_policy<T>())
			throw anyimpl::bad_any_cast();
		T* r = reinterpret_cast<T*>(policy->get_value(const_cast<void**>(&object)));
		return *r;
	}

	/// Returns true if the any contains no value.
	bool empty() const {
		return policy == anyimpl::get_policy<anyimpl::empty_any>();
	}

	/// Frees any allocated memory, and sets the value to NULL.
	void reset() {
		policy->static_delete(&object);
		policy = anyimpl::get_policy<anyimpl::empty_any>();
	}

	/// Returns true if the two types are the same.
	bool compatible(const any& x) const {
		return policy == x.policy;
	}

	int compareTo(const any &x) const {
		if (policy != x.policy)
			throw anyimpl::bad_any_cast();
		return policy->compareTo(&object, &x.object);
	}

	template<typename T>
	bool isType() const {
		return policy == anyimpl::get_policy<T>();
	}

	bool operator ==(const any &x) const {
		return !compareTo(x);
	}
};

}
}

#endif // OBJECT_H
