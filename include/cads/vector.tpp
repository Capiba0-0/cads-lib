#pragma once

#include <algorithm>
#include <new>
#include <type_traits>
#include <memory>
#include <stdexcept>
#include <utility>

// -- Constructors --
template <typename ValType>
cads::Vector<ValType>::Vector()
    : m_data{nullptr}
    , m_size{0}
    , m_capacity{0}
{ }

template <typename ValType>
cads::Vector<ValType>::Vector(const size_t size, const ValType& value)
    : m_data{nullptr}
    , m_size{size}
    , m_capacity{size}
{
    if (size > 0)
    {
        m_data = static_cast<ValType*>(operator new(m_capacity * sizeof(ValType)));

        for (size_t i = 0; i < size; ++i)
            new (m_data + i) ValType{value};
    }
}


template <typename ValType>
cads::Vector<ValType>::Vector(std::initializer_list<ValType> list)
    : m_data{nullptr}, m_size{list.size()}, m_capacity{list.size()}
{
    if (m_size > 0)
    {
        m_data = static_cast<ValType*>(operator new(m_capacity * sizeof(ValType)));

        size_t i = 0;
        for (const ValType& item : list)
        {
            new (m_data + i) ValType(item);
            ++i;
        }
    }
}

template <typename ValType>
cads::Vector<ValType>::Vector(const Vector& other)
    : m_size{other.m_size}
    , m_capacity{other.m_capacity}
{
    if (other.m_capacity > 0)
    {
        m_data = static_cast<ValType*>(operator new(other.m_capacity * sizeof(ValType)));

        for (size_t i = 0; i < m_size; ++i) {
            new (m_data + i) ValType(other.m_data[i]);
        }
    }
    else {
        m_data = nullptr;
    }
}

template <typename ValType>
cads::Vector<ValType>::Vector(Vector&& other) noexcept
    : m_data{other.m_data}
    , m_size{other.m_size}
    , m_capacity{other.m_capacity}
{
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

template <typename ValType>
cads::Vector<ValType>& cads::Vector<ValType>::operator=(const Vector& other)
{
    if (this != &other)
    {
        Vector temp{other};
        swap(temp);
    }
    return *this;
}

template <typename ValType>
cads::Vector<ValType>& cads::Vector<ValType>::operator=(Vector&& other) noexcept
{
    if (this != &other)
    {
        clear();
        operator delete(m_data);

        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }
    return *this;
}

template <typename ValType>
cads::Vector<ValType>& cads::Vector<ValType>::operator=(std::initializer_list<ValType> list)
{
    Vector temp{list};
    swap(temp);

    return *this;
}


// -- Destructor --
template <typename ValType>
cads::Vector<ValType>::~Vector()
{
    clear();
    operator delete(m_data);
}

// -- Methods --
// - Access -
template <typename ValType>
ValType& cads::Vector<ValType>::operator[](size_t index)
{
    return m_data[index];
}

template <typename ValType>
const ValType& cads::Vector<ValType>::operator[](size_t index) const
{
    return m_data[index];
}

template <typename ValType>
ValType& cads::Vector<ValType>::at(const size_t index)
{
    if (index >= m_size)
        throw std::out_of_range("Vector::at: index out of range");

    return m_data[index];
}

template <typename ValType>
const ValType& cads::Vector<ValType>::at(size_t index) const
{
    if (index >= m_size)
        throw std::out_of_range("Vector::at: index out of range");

    return m_data[index];
}


template <typename ValType>
ValType& cads::Vector<ValType>::front()
{
    return m_data[0];
}

template <typename ValType>
const ValType& cads::Vector<ValType>::front() const
{
    return m_data[0];
}

template <typename ValType>
ValType& cads::Vector<ValType>::back()
{
    return m_data[m_size - 1];
}

template <typename ValType>
const ValType& cads::Vector<ValType>::back() const
{
    return m_data[m_size - 1];
}


template <typename ValType>
ValType* cads::Vector<ValType>::data() noexcept
{
    return m_data;
}

template <typename ValType>
const ValType* cads::Vector<ValType>::data() const noexcept
{
    return m_data;
}


// - Iterator methods -
template <typename ValType>
typename cads::Vector<ValType>::Iterator cads::Vector<ValType>::begin() noexcept
{
    return Iterator{m_data};
}
template <typename ValType>

typename cads::Vector<ValType>::ConstIterator cads::Vector<ValType>::begin() const noexcept
{
    return ConstIterator{m_data};
}

template <typename ValType>
typename cads::Vector<ValType>::Iterator cads::Vector<ValType>::end() noexcept
{
    return Iterator{m_data + m_size};
}

template <typename ValType>
typename cads::Vector<ValType>::ConstIterator cads::Vector<ValType>::end() const noexcept
{
    return ConstIterator{m_data + m_size};
}

template <typename ValType>
typename cads::Vector<ValType>::ConstIterator cads::Vector<ValType>::cbegin() const noexcept
{
    return ConstIterator{m_data};
}

template <typename ValType>
typename cads::Vector<ValType>::ConstIterator cads::Vector<ValType>::cend() const noexcept
{
    return ConstIterator{m_data + m_size};
}

template <typename ValType>
typename cads::Vector<ValType>::ReverseIterator cads::Vector<ValType>::rbegin() noexcept
{
    return ReverseIterator{end()};
}

template <typename ValType>
typename cads::Vector<ValType>::ConstReverseIterator cads::Vector<ValType>::rbegin() const noexcept
{
    return ConstReverseIterator{end()};
}

template <typename ValType>
typename cads::Vector<ValType>::ReverseIterator cads::Vector<ValType>::rend() noexcept
{
    return ReverseIterator{begin()};
}

template <typename ValType>
typename cads::Vector<ValType>::ConstReverseIterator cads::Vector<ValType>::rend() const noexcept
{
    return ConstReverseIterator{begin()};
}

template <typename ValType>
typename cads::Vector<ValType>::ConstReverseIterator cads::Vector<ValType>::crbegin() const noexcept
{
    return ConstReverseIterator{end()};
}

template <typename ValType>
typename cads::Vector<ValType>::ConstReverseIterator cads::Vector<ValType>::crend() const noexcept
{
    return ConstReverseIterator{begin()};
}


// - Capacity -
template <typename ValType>
size_t cads::Vector<ValType>::size() const noexcept
{
    return m_size;
}

template <typename ValType>
size_t cads::Vector<ValType>::capacity() const noexcept
{
    return m_capacity;
}

template <typename ValType>
bool cads::Vector<ValType>::empty() const noexcept
{
    return m_size == 0;
}

template <typename ValType>
void cads::Vector<ValType>::reserve(const size_t newCapacity)
{
    if (newCapacity <= m_capacity)
        return;

    _reallocate(newCapacity);
}

template <typename ValType>
void cads::Vector<ValType>::resize(const size_t newSize)
{
    resize(newSize, ValType{});
}

template <typename ValType>
void cads::Vector<ValType>::resize(const size_t newSize, const ValType& value)
{
    if (newSize < m_size)
    {
        if constexpr (!std::is_trivially_destructible_v<ValType>)
            for (size_t i = newSize; i < m_size; ++i)
                m_data[i].~ValType();
    }
    else if (newSize > m_size)
    {
        if (newSize > m_capacity)
            reserve(newSize);

        for (size_t i = m_size; i < newSize; ++i)
        {
            new (m_data + i) ValType(value);
        }
    }

    m_size = newSize;
}

template <typename ValType>
void cads::Vector<ValType>::shrinkToFit()
{
    if (m_capacity <= m_size)
        return;

    _reallocate(m_size);
    m_capacity = m_size;
}

// - Modifiers -
template <typename ValType>
typename cads::Vector<ValType>::Iterator cads::Vector<ValType>::insert(ConstIterator pos, const ValType& value)
{
    const auto index = std::distance(cbegin(), pos);

    if (m_size < m_capacity) {
        if (index < m_size) {
            new (m_data + m_size) ValType(std::move_if_noexcept(m_data[m_size - 1]));

            for (size_t i = m_size - 1; i > index; --i) {
                m_data[i] = std::move_if_noexcept(m_data[i - 1]);
            }

            m_data[index] = value;
        }
        else
            new (m_data + m_size) ValType(value);

        ++m_size;
        return begin() + index;
    }

    const size_t oldSize = m_size;
    const size_t newCapacity = (m_capacity == 0) ? 1 : m_capacity * 2;

    auto deleter = [](ValType* ptr) { operator delete(ptr); };
    auto newDataOwner = std::unique_ptr<ValType, decltype(deleter)>(
        static_cast<ValType*>(operator new(newCapacity * sizeof(ValType))),
        deleter
    );
    ValType* newData = newDataOwner.get();

    for (size_t i = 0; i < index; ++i)
        new (newData + i) ValType(std::move_if_noexcept(m_data[i]));

    new (newData + index) ValType(value);

    for (size_t i = index; i < oldSize; ++i)
        new (newData + i + 1) ValType(std::move_if_noexcept(m_data[i]));

    if constexpr (!std::is_trivially_destructible_v<ValType>) {
        for (size_t i = 0; i < oldSize; ++i) {
            m_data[i].~T();
        }
    }
    operator delete(m_data);

    m_data = newDataOwner.release();
    m_size = oldSize + 1;
    m_capacity = newCapacity;

    return begin() + index;
}


template <typename ValType>
void cads::Vector<ValType>::pushBack(const ValType& value)
{
    if (m_size == m_capacity)
        reserve(m_capacity == 0 ? 1 : m_capacity * 2);

    new (m_data + m_size) ValType{value};
    ++m_size;
}

template <typename ValType>
void cads::Vector<ValType>::pushBack(ValType&& value)
{
    if (m_size == m_capacity)
        reserve(m_capacity == 0 ? 1 : m_capacity * 2);

    new (m_data + m_size) ValType{std::move(value)};
    ++m_size;
}

template <typename ValType>
void cads::Vector<ValType>::popBack()
{
    if (empty())
        return;

    if constexpr (!std::is_trivially_destructible_v<ValType>)
        m_data[m_size - 1].~ValType();
    --m_size;
}


template <typename ValType>
void cads::Vector<ValType>::clear() noexcept
{
    if constexpr (!std::is_trivially_destructible_v<ValType>) {
        for (size_t i = 0; i < m_size; ++i)
            m_data[i].~ValType();
    }

    m_size = 0;
}

template <typename ValType>
typename cads::Vector<ValType>::Iterator cads::Vector<ValType>::erase(ConstIterator pos)
{
    return erase(pos, pos + 1);
}


template <typename ValType>
typename cads::Vector<ValType>::Iterator cads::Vector<ValType>::erase(ConstIterator first, ConstIterator last)
{
    const auto firstIndex = std::distance(cbegin(), first);
    const auto lastIndex = std::distance(cbegin(), last);
    const auto countToErase = std::distance(first, last);

    if (countToErase <= 0)
        return begin() + firstIndex;

    Iterator writePos = begin() + firstIndex;
    ConstIterator readPos = begin() + lastIndex;

    while (readPos != end())
    {
        *writePos = std::move_if_noexcept(*readPos);
        ++writePos;
        ++readPos;
    }

    const size_t newSize = m_size - countToErase;

    if constexpr (!std::is_trivially_destructible_v<ValType>) {
        for (size_t i = newSize; i < m_size; ++i)
            m_data[i].~ValType();
    }

    m_size = newSize;
    return begin() + firstIndex;
}


template <typename ValType>
void cads::Vector<ValType>::swap(Vector& other) noexcept
{
    std::swap(m_data, other.m_data);
    std::swap(m_size, other.m_size);
    std::swap(m_capacity, other.m_capacity);
}

// - Private Methods -
template <typename ValType>
void cads::Vector<ValType>::_reallocate(const size_t newCapacity)
{
    auto deleter = [](ValType* ptr) { operator delete(ptr); };

    auto newData = std::unique_ptr<ValType, decltype(deleter)>(
        static_cast<ValType*>(operator new(newCapacity * sizeof(ValType))),
        deleter
    );


    for (size_t i = 0; i < m_size; ++i)
        new (newData.get() + i) ValType(std::move_if_noexcept(m_data[i]));

    if constexpr (!std::is_trivially_destructible_v<ValType>)
    {
        for (size_t i = 0; i < m_size; ++i)
            m_data[i].~ValType();
    }
    operator delete(m_data);

    m_data = newData.release();
    m_capacity = newCapacity;
}