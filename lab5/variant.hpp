#ifndef __VARIANT_HPP__
#define __VARIANT_HPP__

#include <map>
#include <memory>
#include <variant>
#include <vector>
#include <string>
#include <type_traits>

//{ number
using number = std::variant<int, float>;
//}

//{ array
using array = std::vector<number>;
//}

//{ recursive_array
// Uses std::shared_ptr for recursion
struct recursive_array;

using recursive_array_value = std::variant<
    number,
    std::shared_ptr<recursive_array>
>;

struct recursive_array : std::vector<recursive_array_value>
{
    using std::vector<recursive_array_value>::vector;
};
//}

//{ recursive_array2
// Uses boost::recursive_wrapper for recursion
#include <boost/variant/recursive_wrapper.hpp>

struct recursive_array2;

using recursive_array2_value = std::variant<
    number,
    boost::recursive_wrapper<recursive_array2>
>;

struct recursive_array2 : std::vector<recursive_array2_value>
{
    using std::vector<recursive_array2_value>::vector;
};
//}

//{ variant_decorator
template<typename... Ts>
struct variant_decorator
{
    using variant_type = std::variant<Ts...>;
    variant_type var;

    constexpr variant_decorator() = default;

    template<typename U, typename = std::enable_if_t<
        std::disjunction_v<std::is_constructible<Ts, U>...>
    >>
    constexpr variant_decorator(U&& value) : var(std::forward<U>(value)) {}

    // Non-const and const overloads via common private helper (Meyers-style)
private:
    template<typename T>
    constexpr T& _get_as() & {
        return std::get<T>(var);
    }

    template<typename T>
    constexpr const T& _get_as() const& {
        return std::get<T>(var);
    }

public:
    template<typename T>
    constexpr T& as() & {
        return _get_as<T>();
    }

    template<typename T>
    constexpr const T& as() const& {
        return _get_as<T>();
    }
};
//}

//{ recursive_map
// Must be a map from string to a variant that supports .as<T>()
// We define a variant type that includes all leaf types + recursive containers
struct recursive_map;

using recursive_map_value = variant_decorator<
    int,
    float,
    bool,
    std::string,
    boost::recursive_wrapper<recursive_array2>,
    boost::recursive_wrapper<recursive_map>
>;

struct recursive_map : std::map<std::string, recursive_map_value>
{
    using std::map<std::string, recursive_map_value>::map;
};
//}

#endif // __VARIANT_HPP__