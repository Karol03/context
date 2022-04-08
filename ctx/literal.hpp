/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <utility>


#define MAX_CONTEXT_NAME_SIZE 32


namespace ctx
{

struct StringLiteral final
{
public:
    template <std::size_t N>
    constexpr StringLiteral(const char (&input)[N]) noexcept
    {
        static_assert((N < MAX_CONTEXT_NAME_SIZE) &&
                "Name of literal is too long!");
        for (std::size_t i{0}; i < N; ++i)
        {
            value[i] = input[i];
        }
    }

    constexpr bool operator==(const StringLiteral& rhs) const noexcept
    {
        for (auto i = 0u; i < MAX_CONTEXT_NAME_SIZE; ++i)
        {
            if (value[i] != rhs.value[i])
            {
                return false;
            }
        }
        return true;
    }

public:
    char value[MAX_CONTEXT_NAME_SIZE] = {};
};

}  // namespace ctx
