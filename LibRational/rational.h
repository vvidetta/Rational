#ifndef LIBRATIONAL_RATIONAL_H
#define LIBRATIONAL_RATIONAL_H

#include <cstdint>
#include <numeric>

template <
    typename Integer,
    typename = std::enable_if_t<std::is_integral_v<Integer>>
>
class rational
{
public:

    using integer_type = Integer;

    constexpr rational() = default;

    constexpr rational(integer_type n)
        : m_num{ n },
        m_den{ 1 }
    {}

    constexpr rational(integer_type num, integer_type den)
        : m_num{ common_sign(num, den) * std::abs(num / std::gcd(num, den)) },
        m_den{ std::abs(den / std::gcd(num, den)) }
    {}

    template <
        typename To,
        typename = std::enable_if_t<std::is_arithmetic_v<To>>
    >
    constexpr operator To() const
    {
        return static_cast<To>(m_num) / static_cast<To>(m_den);
    }

    template<
        typename ToInteger,
        typename = std::enable_if_t<std::is_integral_v<ToInteger>>
    >
    constexpr operator rational<ToInteger>() const
    {
        return rational<ToInteger>{ static_cast<ToInteger>(m_num), static_cast<ToInteger>(m_den) };
    }

    friend constexpr rational operator+(rational a, rational b)
    {
        return rational{ (a.m_num * b.m_den + b.m_num * a.m_den), a.m_den * b.m_den };
    }

    friend constexpr rational operator-(rational a, rational b)
    {
        return rational{ (a.m_num * b.m_den - b.m_num * a.m_den), a.m_den * b.m_den };
    }

    friend constexpr rational<integer_type> operator*(rational a, rational b)
    {
        return rational{ a.m_num * b.m_num, a.m_den * b.m_den };
    }

    friend constexpr rational<integer_type> operator/(rational a, rational b)
    {
        return rational{ a.m_num * b.m_den, a.m_den * b.m_num };
    }

    friend constexpr bool operator==(rational a, rational b)
    {
        // No need to do cross-multiplication here, since rationals are normalised on construction.
        return (a.m_num == b.m_num) && (a.m_den == b.m_den);
    }

    friend constexpr bool operator!=(rational a, rational b)
    {
        return !(a == b);
    }

    friend constexpr bool operator<(rational a, rational b)
    {
        return a.m_num * b.m_den < b.m_num * a.m_den;
    }

    friend constexpr bool operator>=(rational a, rational b)
    {
        return !(a < b);
    }

    friend constexpr bool operator>(rational a, rational b)
    {
        return (b < a);
    }

    friend constexpr bool operator<=(rational a, rational b)
    {
        return !(a > b);
    }

    friend constexpr rational operator+(rational a)
    {
        return a;
    }

    friend constexpr rational operator-(rational a)
    {
        return rational{ -a.m_num, a.m_den };
    }

private:
    integer_type m_num = 0;
    integer_type m_den = 1;

    static constexpr integer_type common_sign(integer_type a, integer_type b)
    {
        return (a == 0 || b == 0) ? 0
            : ((a > 0 && b >0 ) || (a < 0 && b < 0)) ? integer_type{ 1 }
            : integer_type{ -1 };
    }
};

using rational8_t = rational<std::int8_t>;

using rational16_t = rational<std::int16_t>;

using rational32_t = rational<std::int32_t>;

using rational64_t = rational<std::int64_t>;

constexpr rational<unsigned long long> operator"" _q8(unsigned long long n)
{
    return rational{ n };
}

#endif // LIBRATIONAL_RATIONAL_H
