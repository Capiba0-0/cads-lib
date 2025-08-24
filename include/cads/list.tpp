#pragma once

#include <cassert>
#include <utility>
#include <iterator>

// -- Constructors --
template <typename ValType>
cads::List<ValType>::List()
    : m_size{0}
{
    m_sentinel = new Node{};
}

template <typename ValType>
cads::List<ValType>::List(const size_t size, const ValType& value)
    : m_size(size)
{
    m_sentinel = new Node{};

    Node* currTail = m_sentinel;
    for (size_t i = 0; i < m_size; ++i)
    {
        currTail = _initWithValues(currTail, value);
    }
}

template <typename ValType>
cads::List<ValType>::List(std::initializer_list<ValType> list)
    : m_size(list.size())
{
    m_sentinel = new Node{};

    Node* currTail = m_sentinel;
    for (const ValType& item : list)
    {
        currTail = _initWithValues(currTail, item);
    }
}

template <typename ValType>
cads::List<ValType>::List(const List& other)
    : m_size(other.m_size)
{
    m_sentinel = new Node{};

    Node* currTail = m_sentinel;
    for (const ValType& item : other)
    {
        currTail = _initWithValues(currTail, item);
    }
}

template <typename ValType>
cads::List<ValType>::List(List&& other) noexcept
    : m_sentinel(other.m_sentinel)
    , m_size(other.size())
{
    other.m_sentinel = new Node{};
    other.m_size = 0;
}

template <typename ValType>
cads::List<ValType>& cads::List<ValType>::operator=(const List& other)
{
    if (this != &other)
    {
        List temp{other};
        swap(temp);
    }
    return *this;
}

template <typename ValType>
cads::List<ValType>& cads::List<ValType>::operator=(List&& other) noexcept
{
    if (this != &other)
    {
        swap(other);
    }
    return *this;
}

template <typename ValType>
cads::List<ValType>& cads::List<ValType>::operator=(std::initializer_list<ValType> list)
{
    List temp{list};
    swap(temp);

    return *this;
}

// -- Destructor --
template <typename ValType>
cads::List<ValType>::~List()
{
    clear();
    delete m_sentinel;
}

// -- Operators --
template <typename ValType>
bool cads::List<ValType>::operator==(const List& other) const
{
    return m_size == other.m_size && m_sentinel == other.m_sentinel;
}

template <typename ValType>
bool cads::List<ValType>::operator!=(const List& other) const
{
    return m_size != other.m_size && m_sentinel != other.m_sentinel;
}


// -- Methods --
// - Access -
template <typename ValType>
ValType& cads::List<ValType>::front()
{
    assert(!empty() && "front() called on empty List");
    return m_sentinel->next->data;
}

template <typename ValType>
const ValType& cads::List<ValType>::front() const
{
    assert(!empty() && "front() called on empty List");
    return m_sentinel->next->data;
}

template <typename ValType>
ValType& cads::List<ValType>::back()
{
    assert(!empty() && "back() called on empty List");
    return m_sentinel->prev->data;
}

template <typename ValType>
const ValType& cads::List<ValType>::back() const
{
    assert(!empty() && "back() called on empty List");
    return m_sentinel->prev->data;
}

// - Iterator methods -
template <typename ValType>
typename cads::List<ValType>::Iterator cads::List<ValType>::begin() noexcept
{
    return Iterator{m_sentinel->next};
}

template <typename ValType>
typename cads::List<ValType>::ConstIterator cads::List<ValType>::begin() const noexcept
{
    return ConstIterator{m_sentinel->next};
}

template <typename ValType>
typename cads::List<ValType>::Iterator cads::List<ValType>::end() noexcept
{
    return Iterator{m_sentinel};
}

template <typename ValType>
typename cads::List<ValType>::ConstIterator cads::List<ValType>::end() const noexcept
{
    return ConstIterator{m_sentinel};
}

template <typename ValType>
typename cads::List<ValType>::ConstIterator cads::List<ValType>::cbegin() const noexcept
{
    return ConstIterator{m_sentinel->next};
}

template <typename ValType>
typename cads::List<ValType>::ConstIterator cads::List<ValType>::cend() const noexcept
{
    return ConstIterator{m_sentinel};
}

template <typename ValType>
typename cads::List<ValType>::ReverseIterator cads::List<ValType>::rbegin() noexcept
{
    return ReverseIterator{end()};
}

template <typename ValType>
typename cads::List<ValType>::ConstReverseIterator cads::List<ValType>::rbegin() const noexcept
{
    return ConstReverseIterator{end()};
}

template <typename ValType>
typename cads::List<ValType>::ReverseIterator cads::List<ValType>::rend() noexcept
{
    return ReverseIterator{begin()};
}

template <typename ValType>
typename cads::List<ValType>::ConstReverseIterator cads::List<ValType>::rend() const noexcept
{
    return ConstReverseIterator{begin()};
}

template <typename ValType>
typename cads::List<ValType>::ConstReverseIterator cads::List<ValType>::crbegin() const noexcept
{
    return ConstReverseIterator{end()};
}

template <typename ValType>
typename cads::List<ValType>::ConstReverseIterator cads::List<ValType>::crend() const noexcept
{
    return ConstReverseIterator{begin()};
}

// - Size -
template <typename ValType>
size_t cads::List<ValType>::size() const noexcept
{
    return m_size;
}

template <typename ValType>
bool cads::List<ValType>::empty() const noexcept
{
    return m_size == 0;
}

// - Modifiers -
template <typename ValType>
typename cads::List<ValType>::Iterator cads::List<ValType>::insert(ConstIterator pos, const ValType& value)
{
    Node* nodeAfter = const_cast<Node*>(pos.m_node);
    Node* nodeBefore = nodeAfter->prev;

    Node* newNode = new Node{value, nodeBefore, nodeAfter};

    nodeBefore->next = newNode;
    nodeAfter->prev = newNode;

    ++m_size;

    return Iterator{newNode};
}


template <typename ValType>
void cads::List<ValType>::pushFront(const ValType& value)
{
    Node* lastFront = m_sentinel->next;

    Node* newNode = new Node{ value, m_sentinel, lastFront };

    lastFront->prev = newNode;
    m_sentinel->next = newNode;

    ++m_size;
}

template <typename ValType>
void cads::List<ValType>::pushFront(ValType&& value)
{
    Node* lastFront = m_sentinel->next;

    Node* newNode = new Node{ std::move(value), m_sentinel, lastFront };

    lastFront->prev = newNode;
    m_sentinel->next = newNode;

    ++m_size;
}

template <typename ValType>
void cads::List<ValType>::pushBack(const ValType& value)
{
    Node* lastBack = m_sentinel->prev;

    Node* newNode = new Node{ value, lastBack, m_sentinel };

    lastBack->next = newNode;
    m_sentinel->prev = newNode;

    ++m_size;
}

template <typename ValType>
void cads::List<ValType>::pushBack(ValType&& value)
{
    Node* lastBack = m_sentinel->prev;

    Node* newNode = new Node{ std::move(value), lastBack, m_sentinel };

    lastBack->next = newNode;
    m_sentinel->prev = newNode;

    ++m_size;
}

template <typename ValType>
void cads::List<ValType>::popFront()
{
    if (empty()) return;

    Node* frontToPop = m_sentinel->next;
    Node* newFront = frontToPop->next;

    m_sentinel->next = newFront;
    newFront->prev = m_sentinel;

    delete frontToPop;

    --m_size;
}

template <typename ValType>
void cads::List<ValType>::popBack()
{
    if (empty()) return;

    Node* backToPop = m_sentinel->prev;
    Node* newBack = backToPop->prev;

    m_sentinel->prev = newBack;
    newBack->next = m_sentinel;

    delete backToPop;

    --m_size;
}

template <typename ValType>
typename cads::List<ValType>::Iterator cads::List<ValType>::erase(ConstIterator pos)
{
    auto last = pos;
    ++last;

    return erase(pos, last);
}

template <typename ValType>
typename cads::List<ValType>::Iterator cads::List<ValType>::erase(ConstIterator first, ConstIterator last)
{
    Node* firstNode = const_cast<Node*>(first.m_node);
    Node* lastNode = const_cast<Node*>(last.m_node);

    if (first == last) return Iterator{ lastNode };

    size_t count = 0;

    firstNode->prev->next = lastNode;
    lastNode->prev = firstNode->prev;

    Node* curr = firstNode;
    while (curr != lastNode)
    {
        Node* next = curr->next;

        delete curr;
        ++count;

        curr = next;
    }

    m_size -= count;

    return Iterator{ lastNode };
}

template <typename ValType>
void cads::List<ValType>::remove(const ValType& value)
{
    auto currIt = begin();

    while (currIt != end())
    {
        if (*currIt == value)
        {
            currIt = erase(currIt);
        }
        else
            ++currIt;
    }
}



template <typename ValType>
void cads::List<ValType>::clear() noexcept
{
    Node* curr = m_sentinel->next;
    while (curr != m_sentinel)
    {
        Node* next = curr->next;
        delete curr;
        curr = next;
    }

    m_sentinel->next = m_sentinel;
    m_sentinel->prev = m_sentinel;
    m_size = 0;
}


template <typename ValType>
void cads::List<ValType>::swap(List& other) noexcept
{
    std::swap(m_sentinel, other.m_sentinel);
    std::swap(m_size, other.m_size);
}

template <typename ValType>
void cads::List<ValType>::reverse()
{
    Node* curr = m_sentinel->next;

    std::swap(m_sentinel->next, m_sentinel->prev);

    while (curr != m_sentinel)
    {
        Node* next = curr->next;

        std::swap(curr->next, curr->prev);

        curr = next;
    }
}


template <typename ValType>
void cads::List<ValType>::splice(ConstIterator pos, List& other, ConstIterator first, ConstIterator last)
{
    if (first == last) return;

    Node* posNode = const_cast<Node*>(pos.m_node);
    Node* firstNode = const_cast<Node*>(first.m_node);
    Node* lastNode = const_cast<Node*>(last.m_node);

    size_t count = 0;
    if (this != &other)
        count = std::distance(first, last);

    Node* rangeFirst = firstNode;
    Node* rangeLast = lastNode->prev;

    firstNode->prev->next = lastNode;
    lastNode->prev = firstNode->prev;

    posNode->prev->next = rangeFirst;
    rangeFirst->prev = posNode->prev;

    rangeLast->next = posNode;
    posNode->prev = rangeLast;

    m_size += count;
    other.m_size -= count;
}


// -- Private methods --
template <typename ValType>
typename cads::List<ValType>::Node* cads::List<ValType>::_initWithValues(Node* currTail, const ValType& value)
{
    Node* newNode = new Node{value, currTail, m_sentinel};

    currTail->next = newNode;
    m_sentinel->prev = newNode;

    return newNode;
}