#ifndef __VARIANT_HPP__
#define __VARIANT_HPP__

#include <map>
#include <memory>
#include <variant>
#include <vector>
#include <string>
#include <type_traits>

#include <boost/variant.hpp>
#include <boost/recursive_wrapper.hpp>

//{ number - вариантное число (int или float)
using number = std::variant<int, float>;
//}

//{ array - массив чисел
using array = std::vector<number>;
//}

//{ recursive_array - рекурсивный массив (версия с shared_ptr)
struct recursive_array : public std::variant<
    number,
    std::shared_ptr<recursive_array>
> {
    using base = std::variant<number, std::shared_ptr<recursive_array>>;
    using base::base;
    using base::operator=;
    
    // Оператор индексирования для тестов
    recursive_array& operator[](size_t index) {
        if (auto* arr = std::get_if<std::shared_ptr<recursive_array>>(this)) {
            return (*arr)->operator[](index);
        }
        throw std::bad_variant_access();
    }
    
    const recursive_array& operator[](size_t index) const {
        if (auto* arr = std::get_if<std::shared_ptr<recursive_array>>(this)) {
            return (*arr)->operator[](index);
        }
        throw std::bad_variant_access();
    }
};
//}

//{ recursive_array2 - рекурсивный массив (версия с recursive_wrapper)
struct recursive_array2 : public boost::variant<
    number,
    boost::recursive_wrapper<recursive_array2>
> {
    using base = boost::variant<number, boost::recursive_wrapper<recursive_array2>>;
    using base::base;
    using base::operator=;
    
    // Оператор индексирования
    recursive_array2& operator[](size_t index) {
        if (auto* arr = boost::get<boost::recursive_wrapper<recursive_array2>>(this)) {
            return arr->get()[index];
        }
        throw std::bad_variant_access();
    }
    
    const recursive_array2& operator[](size_t index) const {
        if (auto* arr = boost::get<boost::recursive_wrapper<recursive_array2>>(this)) {
            return arr->get()[index];
        }
        throw std::bad_variant_access();
    }
};
//}

//{ variant_decorator - декоратор для variant
template<typename... Types>
struct variant_decorator : public std::variant<Types...> {
    using base = std::variant<Types...>;
    using base::base;
    using base::operator=;
    
    // Метод as() для безопасного получения значения
    template<typename T>
    const T& as() const {
        return std::get<T>(*this);
    }
    
    template<typename T>
    T& as() {
        return std::get<T>(*this);
    }
};
//}

//{ recursive_map - рекурсивная карта (ассоциативный массив)
struct recursive_map : public std::map<
    std::string,
    boost::variant<
        int,
        float,
        std::string,
        bool,
        boost::recursive_wrapper<recursive_map>
    >
> {
    using value_type = boost::variant<
        int,
        float,
        std::string,
        bool,
        boost::recursive_wrapper<recursive_map>
    >;
    using base = std::map<std::string, value_type>;
    using base::base;
    using base::operator=;
    
    // Метод as() для удобного доступа
    template<typename T>
    const T& as(const std::string& key) const {
        return boost::get<T>(this->at(key));
    }
    
    template<typename T>
    T& as(const std::string& key) {
        return boost::get<T>(this->at(key));
    }
};
//}

#endif // __VARIANT_HPP__