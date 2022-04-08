/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include "literal.hpp"
#include "valuestorage.hpp"


namespace ctx
{

class StaticHandler
{
public:
    virtual ~StaticHandler() = default;

    template <typename T>
    inline T* get(StringLiteral&& name) noexcept
    {
        auto* value = getValue(std::move(name));
        return value ? value->get<T>() : nullptr;
    }

    template <typename T>
    inline const T* get_const(StringLiteral&& name) const noexcept
    {
        auto* value = getValue(std::move(name));
        return value ? value->get_const<T>() : nullptr;
    }

    inline const char* get_const_char(StringLiteral&& name) const noexcept
    {
        auto* value = getValue(std::move(name));
        return value ? value->get_const_char() : nullptr;
    }

    template <typename T>
    inline void set(StringLiteral&& name, T&& value) { setValue(std::move(name), ValueStorage{std::forward<T>(value)}); }

protected:
    virtual ValueStorage* getValue(StringLiteral&& name) noexcept = 0;
    virtual const ValueStorage* getValue(StringLiteral&& name) const noexcept = 0;
    virtual void setValue(StringLiteral&& name, ValueStorage value) = 0;
};


template <StringLiteral... Names>
class StaticHandlerImpl : public StaticHandler
{
public:
    template <typename... Values>
    constexpr StaticHandlerImpl(Values&&... values)
        : m_array{Names...}
        , m_values{std::forward<Values>(values)...}
    {
        static_assert((sizeof...(Names) >= sizeof...(values)) &&
                "ERROR: Too many arguments!");
    }

protected:
    inline ValueStorage* getValue(StringLiteral&& name) noexcept override
    {
        for (auto i = std::size_t{}; i < m_array.size(); ++i)
        {
            if (m_array[i] == name)
            {
                return &m_values[i];
            }
        }
        return nullptr;
    }

    inline const ValueStorage* getValue(StringLiteral&& name) const noexcept override
    {
        for (auto i = std::size_t{}; i < m_array.size(); ++i)
        {
            if (m_array[i] == name)
            {
                return &m_values[i];
            }
        }
        return nullptr;
    }

    inline void setValue(StringLiteral&& name, ValueStorage value) override
    {
        for (auto i = std::size_t{}; i < m_array.size(); ++i)
        {
            if (m_array[i] == name)
            {
                m_values[i] = std::move(value);
                return;
            }
        }
    }

private:
    std::array<StringLiteral, sizeof...(Names)> m_array;
    std::array<ValueStorage, sizeof...(Names)> m_values;
};


class Context
{
public:
    class Builder
    {
    public:
        explicit Builder(Context& context)
            : m_context{context}
        {}

        template <typename Value>
        inline Builder& set(StringLiteral&& name, Value&& value)
        {
            m_context.set(std::move(name), std::forward<Value>(value));
            return *this;
        }

    private:
        Context& m_context;
    };

private:
    explicit Context() = default;

public:
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&) = default;
    Context& operator=(Context&&) = default;

    template <StringLiteral... Names>
    static inline auto create()
    {
        auto result = Context();
        result.m_handler = std::make_unique<StaticHandlerImpl<Names...>>();
        return result;
    }

    template <typename T>
    inline T* get(StringLiteral&& name)
    {
        return m_handler->get<T>(std::move(name));
    }

    template <typename T>
    inline const T* get(StringLiteral&& name) const
    {
        return m_handler->get_const<T>(std::move(name));
    }

    inline const char* get(StringLiteral&& name) const
    {
        return m_handler->get_const_char(std::move(name));
    }

    template <typename T>
    inline std::enable_if_t<!std::is_same_v<T, const char*>,
    const T*> get_const(StringLiteral&& name) const
    {
        return m_handler->get_const<T>(std::move(name));
    }

    template <typename T>
    inline std::enable_if_t<std::is_same_v<T, const char*>,
    const char*> get_const(StringLiteral&& name) const
    {
        return m_handler->get_const_char(std::move(name));
    }

    template <typename Value>
    inline void set(StringLiteral&& name, Value&& value)
    {
        m_handler->set(std::move(name), std::forward<Value>(value));
    }

private:
    std::unique_ptr<StaticHandler> m_handler;
};

}  // namespace ctx
