#pragma once

#include <sol/force_inline.h>

#include <cmath>

namespace sol {
    #define SOL_INTEGRAL_MINMAX(T) \
        [[nodiscard]] SOL_FORCE_INLINE \
        inline T min(T lhs, T rhs) { \
            return (lhs < rhs) ? lhs : rhs; \
        } \
        [[nodiscard]] SOL_FORCE_INLINE \
        inline T max(T lhs, T rhs) { \
            return (lhs < rhs) ? rhs : lhs; \
        }

    SOL_INTEGRAL_MINMAX(char);
    SOL_INTEGRAL_MINMAX(signed char);
    SOL_INTEGRAL_MINMAX(unsigned char);
    SOL_INTEGRAL_MINMAX(short);
    SOL_INTEGRAL_MINMAX(unsigned short);
    SOL_INTEGRAL_MINMAX(int);
    SOL_INTEGRAL_MINMAX(unsigned int);
    SOL_INTEGRAL_MINMAX(long);
    SOL_INTEGRAL_MINMAX(unsigned long);
    SOL_INTEGRAL_MINMAX(long long);
    SOL_INTEGRAL_MINMAX(unsigned long long);

    #undef SOL_INTEGRAL_MINMAX

    #define SOL_UNARY_STDLIB_UFUNC(T, func, impl) \
        [[nodiscard]] SOL_FORCE_INLINE \
        inline T func(T v) { \
            return std::impl(v); \
        }

    SOL_UNARY_STDLIB_UFUNC(float, abs, abs);
    SOL_UNARY_STDLIB_UFUNC(double, abs, abs);

    SOL_UNARY_STDLIB_UFUNC(float, floor, floor);
    SOL_UNARY_STDLIB_UFUNC(double, floor, floor);

    SOL_UNARY_STDLIB_UFUNC(float, ceil, ceil);
    SOL_UNARY_STDLIB_UFUNC(double, ceil, ceil);

    SOL_UNARY_STDLIB_UFUNC(float, trunc, trunc);
    SOL_UNARY_STDLIB_UFUNC(double, trunc, trunc);

    SOL_UNARY_STDLIB_UFUNC(float, round, rint);
    SOL_UNARY_STDLIB_UFUNC(double, round, rint);

    SOL_UNARY_STDLIB_UFUNC(float, sqrt, sqrt);
    SOL_UNARY_STDLIB_UFUNC(double, sqrt, sqrt);

    #undef SOL_UNARY_STDLIB_UFUNC

    #define SOL_BINARY_STDLIB_UFUNC(T, func, impl) \
        [[nodiscard]] SOL_FORCE_INLINE \
        inline T func(T a, T b) { \
            return std::impl(a, b); \
        }

    SOL_BINARY_STDLIB_UFUNC(float, min, fmin);
    SOL_BINARY_STDLIB_UFUNC(double, min, fmin);

    SOL_BINARY_STDLIB_UFUNC(float, max, fmax);
    SOL_BINARY_STDLIB_UFUNC(double, max, fmax);

    #undef SOL_BINARY_STDLIB_UFUNC
}
