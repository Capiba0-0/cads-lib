#pragma once

#include <initializer_list>
#include <cstddef>
#include <iterator>

namespace cads
{

template<typename ValType>
class List // Bidirectional linked List
{
private:
    // Declaration
    struct Node;

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
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = ValType;
        using difference_type   = std::ptrdiff_t;
        using pointer           = ValType*;
        using reference         = ValType&;

        friend class ConstIterator;
        friend class List;

        explicit Iterator(Node* node = nullptr) : m_node(node) {}

        Iterator(const Iterator&) = default;
        Iterator(Iterator&&) noexcept = default;
        Iterator& operator=(const Iterator&) = default;
        Iterator& operator=(Iterator&&) noexcept = default;

        ~Iterator() = default;

        ValType& operator*() const { return m_node->data; }
        ValType* operator->() const noexcept { return &m_node->data; }

        Iterator& operator++() { m_node = m_node->next; return *this; }
        Iterator operator++(int) { auto temp = *this; m_node = m_node->next; return temp;}
        Iterator& operator--() { m_node = m_node->prev; return *this; }
        Iterator operator--(int) { auto temp = *this; m_node = m_node->prev; return temp;}

        bool operator==(const Iterator& other) const { return m_node == other.m_node; }
        bool operator!=(const Iterator& other) const { return m_node != other.m_node; }

    private:
        Node* m_node;
    };
    class ConstIterator
    {
    public:
        // For integration with STL algorithms
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = ValType;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const ValType*;
        using reference         = const ValType&;

        friend class List;

        explicit ConstIterator(const Node* node = nullptr) : m_node(node) {}

        ConstIterator(const ConstIterator&) = default;
        ConstIterator(ConstIterator&&) noexcept = default;
        ConstIterator& operator=(const ConstIterator&) = default;
        ConstIterator& operator=(ConstIterator&&) noexcept = default;

        ConstIterator(const Iterator& it) : m_node(it.m_node) {}

        ~ConstIterator() = default;

        const ValType& operator*() const { return m_node->data; }
        const ValType* operator->() const noexcept { return &m_node->data; }

        ConstIterator& operator++() { m_node = m_node->next; return *this; }
        ConstIterator operator++(int) { auto temp = *this; m_node = m_node->next; return temp;}
        ConstIterator& operator--() { m_node = m_node->prev; return *this; }
        ConstIterator operator--(int) { auto temp = *this; m_node = m_node->prev; return temp;}

        bool operator==(const ConstIterator& other) const { return m_node == other.m_node; }
        bool operator!=(const ConstIterator& other) const { return m_node != other.m_node; }

    private:
        const Node* m_node;
    };

    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

    // -- Constructors --
    List();
    explicit List(size_t size, const ValType& value = ValType{});
    List(std::initializer_list<ValType> list);
    List(const List& other);
    List(List&& other) noexcept;
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;
    List& operator=(std::initializer_list<ValType> list);

    // -- Destructor --
    ~List();

    // -- Operators --
    bool operator==(const List& other) const;
    bool operator!=(const List& other) const;

    // -- Methods --
    // - Access -
    ValType& front();
    const ValType& front() const;
    ValType& back();
    const ValType& back() const;

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

    // - Size -
    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    // - Modifiers -
    Iterator insert(ConstIterator pos, const ValType& value);

    void pushBack(const ValType& value);
    void pushBack(ValType&& value);
    void pushFront(const ValType& value);
    void pushFront(ValType&& value);

    void popFront();
    void popBack();

    Iterator erase(ConstIterator pos);
    Iterator erase(ConstIterator first, ConstIterator last);
    void remove(const ValType& value); // +
    void clear() noexcept;

    void swap(List& other) noexcept;
    void reverse();

    void splice(ConstIterator pos, List& other, ConstIterator first, ConstIterator last);

private:
    struct Node
    {
        ValType data;
        Node* prev;
        Node* next;

        Node() : data{}, prev(this), next(this) {}

        explicit Node(const ValType& value, Node* p = nullptr, Node* n = nullptr)
            : data(value), prev(p), next(n) {}
        explicit Node(ValType&& value, Node* p = nullptr, Node* n = nullptr)
            : data(std::move(value)), prev(p), next(n) {}

        Node(const Node&) = default;
        Node(Node&&) noexcept = default;
    };

    Node* m_sentinel;
    size_t m_size;

    Node* _initWithValues(Node* currTail, const ValType& value);
};

} // namespace cads

#include "cads/list.tpp"