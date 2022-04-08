/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <memory>

#include "typeof.hpp"


namespace ctx
{

struct Value
{
public:
    virtual ~Value() = default;
    virtual void* get() noexcept = 0;
    virtual const void* get_const() const noexcept = 0;
    virtual std::size_t uuid() const noexcept = 0;
};


template <typename T>
struct ValueRef : public Value
{
public:
    ValueRef(T& value)
        : m_value{value}
    {}

    virtual void* get() noexcept override { return &m_value; };
    virtual const void* get_const() const noexcept override { return &m_value; };
    virtual std::size_t uuid() const noexcept override { return TypeOf<T>().type(); };

private:
    T& m_value;
};


template <typename T>
struct ValueConstRef : public Value
{
public:
    ValueConstRef(const T& value)
        : m_value{value}
    {}

    virtual void* get() noexcept override { return nullptr; };
    virtual const void* get_const() const noexcept override { return &m_value; };
    virtual std::size_t uuid() const noexcept override { return TypeOf<T>().type(); };

private:
    const T& m_value;
};


template <typename T>
struct ValuePtr : public Value
{
public:
    ValuePtr(T* value)
        : m_value{value}
    {}

    ~ValuePtr()
    {
        if (m_value)
            delete m_value;
    }

    virtual void* get() noexcept override { return &(*m_value); };
    virtual const void* get_const() const noexcept override { return &(*m_value); };
    virtual std::size_t uuid() const noexcept override { return TypeOf<T>().type(); };

private:
    T* m_value;
};


template <typename T>
struct ValueConstPtr : public Value
{
public:
    ValueConstPtr(const T* value)
        : m_value{value}
    {}

    ~ValueConstPtr()
    {
        if (m_value)
            delete m_value;
    }

    virtual void* get() noexcept override { return nullptr; };
    virtual const void* get_const() const noexcept override { return &(*m_value); };
    virtual std::size_t uuid() const noexcept override { return TypeOf<T>().type(); };

private:
    const T* m_value;
};


struct ValueConstCharPtr : public Value
{
public:
    ValueConstCharPtr(const char* value)
        : m_value{value}
    {}

    virtual void* get() noexcept override { return nullptr; };
    virtual const void* get_const() const noexcept override { return m_value; };
    virtual std::size_t uuid() const noexcept override { return TypeOf<const char*>().type(); };

private:
    const char* m_value;
};


template <typename T>
struct ValueOwn : public Value
{
public:
    ValueOwn(T&& value)
        : m_value{std::move(value)}
    {}

    virtual void* get() noexcept override { return &m_value; };
    virtual const void* get_const() const noexcept override { return &m_value; };
    virtual std::size_t uuid() const noexcept override { return TypeOf<T>().type(); };

private:
    T m_value;
};


struct ValueStorage final
{
public:
    ValueStorage() = default;

    ValueStorage(const char* value)
        : m_value{std::make_unique<ValueConstCharPtr>(value)}
    {}

    template <typename T>
    ValueStorage(T&& value)
        : m_value{std::make_unique<ValueOwn<T>>(std::move(value))}
    {}

    template <typename T>
    ValueStorage(T& value)
        : m_value{std::make_unique<ValueRef<T>>(value)}
    {}

    template <typename T>
    ValueStorage(const T& value)
        : m_value{std::make_unique<ValueConstRef<T>>(value)}
    {}

    template <typename T>
    ValueStorage(T* value)
        : m_value{std::make_unique<ValuePtr<T>>(std::move(value))}
    {}

    template <typename T>
    ValueStorage(const T* value)
        : m_value{std::make_unique<ValueConstPtr<T>>(value)}
    {}

    ValueStorage(const ValueStorage& value) = delete;
    ValueStorage& operator=(const ValueStorage& value) = delete;

    ValueStorage(ValueStorage&& value) = default;
    ValueStorage& operator=(ValueStorage&& value) = default;

    inline const char* get_const_char() const noexcept
    {
        if (m_value && m_value->get_const() && m_value->uuid() == TypeOf<const char*>().type())
            return reinterpret_cast<const char*>(m_value->get_const());
        return nullptr;
    }

    template <typename T>
    inline const T* get_const() const noexcept
    {
        if (m_value && m_value->get_const() && m_value->uuid() == TypeOf<T>().type())
            return reinterpret_cast<const T*>(m_value->get_const());
        return nullptr;
    }

    template <typename T>
    inline T* get() noexcept
    {
        if (m_value && m_value->get() && m_value->uuid() == TypeOf<T>().type())
            return reinterpret_cast<T*>(m_value->get());
        return nullptr;
    }

    template <typename T>
    inline void set(T&& value)
    {
        auto newStorage = ValueStorage(std::forward<T>(value));
        std::swap(this->m_value, newStorage.m_value);
    }

private:
    std::unique_ptr<Value> m_value;
};

}  // namespace ctx
