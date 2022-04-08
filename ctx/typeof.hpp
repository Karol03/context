/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <utility>


namespace ctx
{

struct ITypeOf
{
protected:
    static inline std::size_t generate() noexcept
    {
        static std::size_t i = 0;
        return ++i;
    }
};


template <typename T>
struct TypeOf final : public ITypeOf
{
public:
    constexpr TypeOf() noexcept
    {
        if (!m_type)
            m_type = ITypeOf::generate();
    }

    constexpr inline auto type() noexcept
    {
        return m_type;
    }

private:
    static inline std::size_t m_type = 0;
};

}  // namespace ctx
