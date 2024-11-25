/* Image iterator.
 * @file
 * @date 2020
 * @author Anonymous
 */

#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

#include <algorithm>
#include <type_traits>
#include <vector>
#include <utility>

#include <boost/iterator/iterator_adaptor.hpp>

#include <iostream>

// stride is the number of bytes from one row of pixels in memory to the next row
// of pixels in memory
// as if abs distance

// stride is a row number
// it's when we convert 2-D array into 1-D array: [x , y] -> a; a in [0; xy - 1]

template<typename T>
// image_iterator<T> - derived class, T - base class>
class image_iterator : public boost::iterator_adaptor<image_iterator<T>, T>
{
public:
    // w is for width, st is for stride
    image_iterator(T it, size_t w, size_t st)
    : boost::iterator_adaptor<image_iterator<T>, T>(it) // syntax is (Base const &iter)
    , m_start_iterator(it)
    , m_w(w)
    , m_st(st)
    {}

    long distance_to(const image_iterator& it) const
    {
        const auto d1 = std::abs(it.base() - it.m_start_iterator) % ptrdiff_t(m_st);
        const auto d2 = std::abs(this->base() - m_start_iterator) % ptrdiff_t(m_st);

        return (it.base() - d1 - (this->base() - d2)) / ptrdiff_t(m_st) * m_w  + (d1 - d2);
    }

    void advance(const int n)
    {
        if (n > 0) for (int i = 0; i < n; ++i) increment();
        else for (int i = 0; i < -n; ++i) decrement();
    }

    void decrement()
    {
        //if we go to the previous column, make sure we don't get out of bounds
        if (std::abs(this->base() - m_start_iterator) % ptrdiff_t(m_st) <= 0)
            this->base_reference() -= (m_st - m_w + 1);
        else
            --this->base_reference();
    }

    void increment() {
        //same in decrement, make sure we don't get out of bounds
        if (std::abs(this->base() - m_start_iterator) % ptrdiff_t(m_st) + 1 >= long(m_w))
            this->base_reference() += (m_st - m_w + 1);
        else
            ++this->base_reference();
    }

private:
    T m_start_iterator;
    size_t m_w;
    size_t m_st;
};

template<class Container = std::vector<uint8_t>>
class image
{
public:
    image(size_t width, size_t height, size_t stride):
    data(stride * height),
    m_width(width),
    m_stride(stride)
    {}

    auto begin()
    {
        return image_iterator(data.begin(), m_width, m_stride);
    }

    auto end()
    {
        return image_iterator(data.end(), m_width, m_stride);
    }

    auto begin() const
    {
        return image_iterator(data.begin(), m_width, m_stride);
    }

    auto end() const
    {
        return image_iterator(data.end(), m_width, m_stride);
    }

private:
    Container data; // image data including strides. Access by (x, y): data[y * m_stride + x]
    size_t m_width;
    size_t m_stride;
};

#endif // __ITERATOR_HPP__
