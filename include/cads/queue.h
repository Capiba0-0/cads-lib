#pragma once

#include "cads/list.h"

namespace cads
{

template <typename ValType, typename Container = List<ValType>>
class Queue
{
public:
    using value_type      = ValType;
    using size_type       = std::size_t;
    using reference       = ValType&;
    using const_reference = const ValType&;
    using pointer         = ValType*;
    using const_pointer   = const ValType*;


    [[nodiscard]] bool empty() const noexcept
    {
        return m_container.empty();
    };

    [[nodiscard]] size_type size() const noexcept
    {
        return m_container.size();
    }


    reference front()
    {
        return m_container.front();
    }

    const_reference front() const
    {
        return m_container.front();
    }

    reference back()
    {
        return m_container.back();
    }

    const_reference back() const
    {
        return m_container.back();
    }


    void push(const ValType& val)
    {
        m_container.pushBack(val);
    }

    void push(ValType&& val)
    {
        m_container.pushBack(std::move(val));
    }


    void pop()
    {
        m_container.popFront();
    }


    void swap(Queue& other) noexcept
    {
        std::swap(m_container, other.m_container);
    }

private:
    Container m_container;
};

} // namespace cads