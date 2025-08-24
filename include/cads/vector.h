#pragma once

#include <initializer_list>
#include <cstddef>
#include <iterator>

namespace cads
{

template<typename ValType>
class Vector
{
public:
    class Iterator;
    class ConstIterator;

    using value_type      = ValType;
    using size_type       = std::size_t;
    using reference       = ValType&;
    using const_reference = const ValType&;
    using pointer         = ValType*;
    using const_pointer   = const ValType*;
    using iterator        = Iterator;
    using const_iterator  = ConstIterator;

    // -- Iterators --
    class Iterator
    {
    public:
        // For integration with STL algorithms
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = ValType;
        using difference_type   = std::ptrdiff_t;
        using pointer           = ValType*;
        using reference         = ValType&;

        friend class ConstIterator;

        explicit Iterator(ValType* ptr = nullptr) : m_ptr(ptr) {}

        Iterator(const Iterator&) = default;
        Iterator(Iterator&&) noexcept = default;
        Iterator& operator=(const Iterator&) = default;
        Iterator& operator=(Iterator&&) noexcept = default;

        ~Iterator() = default;

        ValType& operator*() const { return *m_ptr; }
        ValType* operator->() const noexcept { return m_ptr; }

        Iterator& operator++() { ++m_ptr; return *this; }
        Iterator operator++(int) { auto temp = *this; ++m_ptr; return temp;}
        Iterator& operator--() { --m_ptr; return *this; }
        Iterator operator--(int) { auto temp = *this; --m_ptr; return temp;}

        Iterator& operator+=(std::ptrdiff_t n) { m_ptr += n; return *this; }
        Iterator& operator-=(std::ptrdiff_t n) { m_ptr -= n; return *this; }

        Iterator operator+(std::ptrdiff_t n) const { return Iterator(m_ptr + n); }
        Iterator operator-(std::ptrdiff_t n) const { return Iterator(m_ptr - n); }
        std::ptrdiff_t operator-(const Iterator& other) const { return m_ptr - other.m_ptr; }

        ValType& operator[](std::ptrdiff_t n) const { return m_ptr[n]; }

        auto operator<=>(const Iterator& other) const = default;
        bool operator==(const Iterator& other) const = default;

    private:
        ValType* m_ptr;
    };

    class ConstIterator
    {
    public:
        // For integration with STL algorithms
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = ValType;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const ValType*;
        using reference         = const ValType&;

        explicit ConstIterator(const ValType* ptr = nullptr) : m_ptr(ptr) {}

        ConstIterator(const ConstIterator&) = default;
        ConstIterator(ConstIterator&&) noexcept = default;
        ConstIterator& operator=(const ConstIterator&) = default;
        ConstIterator& operator=(ConstIterator&&) noexcept = default;

        ConstIterator(const Iterator& it) : m_ptr(it.m_ptr) {}

        ~ConstIterator() = default;

        const ValType& operator*() const { return *m_ptr; }
        const ValType* operator->() const noexcept { return m_ptr; }

        ConstIterator& operator++() { ++m_ptr; return *this; }
        ConstIterator operator++(int) { auto temp = *this; ++m_ptr; return temp;}
        ConstIterator& operator--() { --m_ptr; return *this; }
        ConstIterator operator--(int) { auto temp = *this; --m_ptr; return temp;}

        ConstIterator& operator+=(std::ptrdiff_t n) { m_ptr += n; return *this; }
        ConstIterator& operator-=(std::ptrdiff_t n) { m_ptr -= n; return *this; }

        ConstIterator operator+(std::ptrdiff_t n) const { return ConstIterator(m_ptr + n); }
        ConstIterator operator-(std::ptrdiff_t n) const { return ConstIterator(m_ptr - n); }
        std::ptrdiff_t operator-(const ConstIterator& other) const { return m_ptr - other.m_ptr; }

        const ValType& operator[](std::ptrdiff_t n) const { return m_ptr[n]; }

        bool operator==(const ConstIterator& other) const = default;
        auto operator<=>(const ConstIterator& other) const = default;

    private:
        const ValType* m_ptr;
    };

    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

    // -- Constructors --
    Vector();
    explicit Vector(size_t size, const ValType& value = ValType{});
    Vector(std::initializer_list<ValType> list);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
    Vector& operator=(std::initializer_list<ValType> list);

    // -- Destructor --
    ~Vector();

    // -- Methods --
    // - Access -
    ValType& operator[](size_t index);
    const ValType& operator[](size_t index) const;
    ValType& at(size_t index);
    const ValType& at(size_t index) const;

    ValType& front();
    const ValType& front() const;
    ValType& back();
    const ValType& back() const;

    ValType* data() noexcept;
    const ValType* data() const noexcept;

    // - Iterator methods -
    Iterator begin() noexcept;
    ConstIterator begin() const noexcept;
    Iterator end() noexcept;
    ConstIterator end() const noexcept;

    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

    ReverseIterator rbegin() noexcept;
    ConstReverseIterator rbegin() const noexcept;
    ReverseIterator rend() noexcept;
    ConstReverseIterator rend() const noexcept;

    ConstReverseIterator crbegin() const noexcept;
    ConstReverseIterator crend() const noexcept;

    // - Capacity -
    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] size_t capacity() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    void reserve(size_t newCapacity);
    void resize(size_t newSize);
    void resize(size_t newSize, const ValType& value);
    void shrinkToFit();

    // - Modifiers -
    Iterator insert(ConstIterator pos, const ValType& value);

    void pushBack(const ValType& value);
    void pushBack(ValType&& value);
    void popBack();
    void clear() noexcept;

    Iterator erase(ConstIterator pos);
    Iterator erase(ConstIterator first, ConstIterator last);

    void swap(Vector& other) noexcept;

private:
    ValType* m_data;
    size_t m_size;
    size_t m_capacity;

    void _reallocate(size_t newCapacity);
};

} // namespace cads

#include "cads/vector.tpp"