#pragma once

#include <sol/force_inline.h>

#include <cstddef>

#if defined(__ARM_NEON) || defined(__ARM_NEON__) || defined(__ARM_NEON_FP)
#define SOL_ARM_NEON_AVAILABLE 1
#else
#define SOL_ARM_NEON_AVAILABLE 0
#endif

#if SOL_ARM_NEON_AVAILABLE
#include <arm_neon.h>
#endif

namespace sol {
    template <class T, size_t N>
    using Simd = T __attribute__((ext_vector_type(N)));

    using Int8x8 = Simd<int8_t, 8>;
    using Int8x16 = Simd<int8_t, 16>;
    using Int16x4 = Simd<int16_t, 4>;
    using Int16x8 = Simd<int16_t, 8>;
    using Int32x2 = Simd<int32_t, 2>;
    using Int32x4 = Simd<int32_t, 4>;
    using Int64x1 = Simd<int64_t, 1>;
    using Int64x2 = Simd<int64_t, 2>;

    using UInt8x8 = Simd<uint8_t, 8>;
    using UInt8x16 = Simd<uint8_t, 16>;
    using UInt16x4 = Simd<uint16_t, 4>;
    using UInt16x8 = Simd<uint16_t, 8>;
    using UInt32x2 = Simd<uint32_t, 2>;
    using UInt32x4 = Simd<uint32_t, 4>;
    using UInt64x1 = Simd<uint64_t, 1>;
    using UInt64x2 = Simd<uint64_t, 2>;

    using Float32x2 = Simd<float, 2>;
    using Float32x4 = Simd<float, 4>;
    using Float64x1 = Simd<double, 1>;
    using Float64x2 = Simd<double, 2>;

    #if SOL_ARM_NEON_AVAILABLE

    constexpr inline size_t TARGET_VECTOR_WIDTH = 16;

    #define SOL_UNARY_NEON_UFUNC(T, func, impl) \
        [[nodiscard]] SOL_FORCE_INLINE inline T func(T v) { \
            return impl(v); \
        }

    SOL_UNARY_NEON_UFUNC(Int8x8, abs, vabs_s8);
    SOL_UNARY_NEON_UFUNC(Int8x16, abs, vabsq_s8);
    SOL_UNARY_NEON_UFUNC(Int16x4, abs, vabs_s16);
    SOL_UNARY_NEON_UFUNC(Int16x8, abs, vabsq_s16);
    SOL_UNARY_NEON_UFUNC(Int32x2, abs, vabs_s32);
    SOL_UNARY_NEON_UFUNC(Int32x4, abs, vabsq_s32);
    SOL_UNARY_NEON_UFUNC(Int64x1, abs, vabs_s64);
    SOL_UNARY_NEON_UFUNC(Int64x2, abs, vabsq_s64);
    SOL_UNARY_NEON_UFUNC(Float32x2, abs, vabs_f32);
    SOL_UNARY_NEON_UFUNC(Float32x4, abs, vabsq_f32);
    SOL_UNARY_NEON_UFUNC(Float64x1, abs, vabs_f64);
    SOL_UNARY_NEON_UFUNC(Float64x2, abs, vabsq_f64);

    SOL_UNARY_NEON_UFUNC(Float32x2, trunc, vrnd_f32);
    SOL_UNARY_NEON_UFUNC(Float32x4, trunc, vrndq_f32);
    SOL_UNARY_NEON_UFUNC(Float64x1, trunc, vrnd_f64);
    SOL_UNARY_NEON_UFUNC(Float64x2, trunc, vrndq_f64);

    SOL_UNARY_NEON_UFUNC(Float32x2, floor, vrndm_f32);
    SOL_UNARY_NEON_UFUNC(Float32x4, floor, vrndmq_f32);
    SOL_UNARY_NEON_UFUNC(Float64x1, floor, vrndm_f64);
    SOL_UNARY_NEON_UFUNC(Float64x2, floor, vrndmq_f64);

    SOL_UNARY_NEON_UFUNC(Float32x2, ceil, vrndp_f32);
    SOL_UNARY_NEON_UFUNC(Float32x4, ceil, vrndpq_f32);
    SOL_UNARY_NEON_UFUNC(Float64x1, ceil, vrndp_f64);
    SOL_UNARY_NEON_UFUNC(Float64x2, ceil, vrndpq_f64);

    SOL_UNARY_NEON_UFUNC(Float32x2, round, vrndn_f32);
    SOL_UNARY_NEON_UFUNC(Float32x4, round, vrndnq_f32);
    SOL_UNARY_NEON_UFUNC(Float64x1, round, vrndn_f64);
    SOL_UNARY_NEON_UFUNC(Float64x2, round, vrndnq_f64);

    SOL_UNARY_NEON_UFUNC(Float32x2, sqrt, vsqrt_f32);
    SOL_UNARY_NEON_UFUNC(Float32x4, sqrt, vsqrtq_f32);
    SOL_UNARY_NEON_UFUNC(Float64x1, sqrt, vsqrt_f64);
    SOL_UNARY_NEON_UFUNC(Float64x2, sqrt, vsqrtq_f64);

    #undef SOL_UNARY_NEON_UFUNC

    #define SOL_BINARY_NEON_UFUNC(T, func, impl) \
        [[nodiscard]] SOL_FORCE_INLINE inline T func(T lhs, T rhs) { \
            return impl(lhs, rhs); \
        }

    SOL_BINARY_NEON_UFUNC(Int8x8, min, vmin_s8);
    SOL_BINARY_NEON_UFUNC(Int8x16, min, vminq_s8);
    SOL_BINARY_NEON_UFUNC(Int16x4, min, vmin_s16);
    SOL_BINARY_NEON_UFUNC(Int16x8, min, vminq_s16);
    SOL_BINARY_NEON_UFUNC(Int32x2, min, vmin_s32);
    SOL_BINARY_NEON_UFUNC(Int32x4, min, vminq_s32);
    SOL_BINARY_NEON_UFUNC(UInt8x8, min, vmin_u8);
    SOL_BINARY_NEON_UFUNC(UInt8x16, min, vminq_u8);
    SOL_BINARY_NEON_UFUNC(UInt16x4, min, vmin_u16);
    SOL_BINARY_NEON_UFUNC(UInt16x8, min, vminq_u16);
    SOL_BINARY_NEON_UFUNC(UInt32x2, min, vmin_u32);
    SOL_BINARY_NEON_UFUNC(UInt32x4, min, vminq_u32);
    SOL_BINARY_NEON_UFUNC(Float32x2, min, vmin_f32);
    SOL_BINARY_NEON_UFUNC(Float32x4, min, vminq_f32);
    SOL_BINARY_NEON_UFUNC(Float64x1, min, vmin_f64);
    SOL_BINARY_NEON_UFUNC(Float64x2, min, vminq_f64);

    SOL_BINARY_NEON_UFUNC(Int8x8, max, vmax_s8);
    SOL_BINARY_NEON_UFUNC(Int8x16, max, vmaxq_s8);
    SOL_BINARY_NEON_UFUNC(Int16x4, max, vmax_s16);
    SOL_BINARY_NEON_UFUNC(Int16x8, max, vmaxq_s16);
    SOL_BINARY_NEON_UFUNC(Int32x2, max, vmax_s32);
    SOL_BINARY_NEON_UFUNC(Int32x4, max, vmaxq_s32);
    SOL_BINARY_NEON_UFUNC(UInt8x8, max, vmax_u8);
    SOL_BINARY_NEON_UFUNC(UInt8x16, max, vmaxq_u8);
    SOL_BINARY_NEON_UFUNC(UInt16x4, max, vmax_u16);
    SOL_BINARY_NEON_UFUNC(UInt16x8, max, vmaxq_u16);
    SOL_BINARY_NEON_UFUNC(UInt32x2, max, vmax_u32);
    SOL_BINARY_NEON_UFUNC(UInt32x4, max, vmaxq_u32);
    SOL_BINARY_NEON_UFUNC(Float32x2, max, vmax_f32);
    SOL_BINARY_NEON_UFUNC(Float32x4, max, vmaxq_f32);
    SOL_BINARY_NEON_UFUNC(Float64x1, max, vmax_f64);
    SOL_BINARY_NEON_UFUNC(Float64x2, max, vmaxq_f64);

    #undef SOL_BINARY_NEON_UFUNC

    #endif
}
