#pragma once

#include <sol/force_inline.h>

#include <array>
#include <concepts>
#include <cstddef>
#include <vector>

namespace sol {
    template <class T>
    struct UniversalAdd;
}

// namespace sol::rtree {
//     class UVec3 {
//     public:
//         SOL_FORCE_INLINE
//         UVec3() = default;
//
//         SOL_FORCE_INLINE
//         UVec3(uint32x4x3_t components): _components(components) {}
//
//         SOL_FORCE_INLINE
//         UVec3(uint32x4_t c): _components{c, c, c} {}
//
//         SOL_FORCE_INLINE
//         UVec3(uint32x4_t x, uint32x4_t y, uint32x4_t z): _components{x, y, z} {}
//
//         SOL_FORCE_INLINE
//         uint32x4x3_t const& components() const {
//             return _components;
//         }
//
//         SOL_FORCE_INLINE
//         uint32x4_t const& x() const {
//             return _components.val[0];
//         }
//
//         SOL_FORCE_INLINE
//         uint32x4_t const& y() const {
//             return _components.val[1];
//         }
//
//         SOL_FORCE_INLINE
//         uint32x4_t const& z() const {
//             return _components.val[2];
//         }
//
//         SOL_FORCE_INLINE
//         uint32x4x3_t& components() {
//             return _components;
//         }
//
//         SOL_FORCE_INLINE
//         uint32x4_t& x() {
//             return _components.val[0];
//         }
//
//         SOL_FORCE_INLINE
//         uint32x4_t& y() {
//             return _components.val[1];
//         }
//
//         SOL_FORCE_INLINE
//         uint32x4_t& z() {
//             return _components.val[2];
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator+(UVec3 rhs) const {
//             return {x() + rhs.x(), y() + rhs.y(), z() + rhs.z()};
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator-(UVec3 rhs) const {
//             return {x() - rhs.x(), y() - rhs.y(), z() - rhs.z()};
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator*(UVec3 rhs) const {
//             return {x() * rhs.x(), y() * rhs.y(), z() * rhs.z()};
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator/(UVec3 rhs) const {
//             return {x() / rhs.x(), y() / rhs.y(), z() / rhs.z()};
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator%(UVec3 rhs) const {
//             return {x() % rhs.x(), y() % rhs.y(), z() % rhs.z()};
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator&(UVec3 rhs) const {
//             return {x() & rhs.x(), y() & rhs.y(), z() & rhs.z()};
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator|(UVec3 rhs) const {
//             return {x() | rhs.x(), y() | rhs.y(), z() | rhs.z()};
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator^(UVec3 rhs) const {
//             return {x() ^ rhs.x(), y() ^ rhs.y(), z() ^ rhs.z()};
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator~() const {
//             return {~x(), ~y(), ~z()};
//         }
//
//     private:
//         uint32x4x3_t _components;
//     };
//
//     class FVec3 {
//     public:
//         SOL_FORCE_INLINE
//         FVec3() = default;
//
//         SOL_FORCE_INLINE
//         FVec3(float32x4x3_t components): _components(components) {}
//
//         SOL_FORCE_INLINE
//         FVec3(float32x4_t c): _components{c, c, c} {}
//
//         SOL_FORCE_INLINE
//         FVec3(float32x4_t x, float32x4_t y, float32x4_t z): _components{x, y, z} {}
//
//         SOL_FORCE_INLINE
//         float32x4x3_t const& components() const {
//             return _components;
//         }
//
//         SOL_FORCE_INLINE
//         float32x4_t const& x() const {
//             return _components.val[0];
//         }
//
//         SOL_FORCE_INLINE
//         float32x4_t const& y() const {
//             return _components.val[1];
//         }
//
//         SOL_FORCE_INLINE
//         float32x4_t const& z() const {
//             return _components.val[2];
//         }
//
//         SOL_FORCE_INLINE
//         float32x4x3_t& components() {
//             return _components;
//         }
//
//         SOL_FORCE_INLINE
//         float32x4_t& x() {
//             return _components.val[0];
//         }
//
//         SOL_FORCE_INLINE
//         float32x4_t& y() {
//             return _components.val[1];
//         }
//
//         SOL_FORCE_INLINE
//         float32x4_t& z() {
//             return _components.val[2];
//         }
//
//         SOL_FORCE_INLINE
//         FVec3 operator+(FVec3 rhs) const {
//             return {x() + rhs.x(), y() + rhs.y(), z() + rhs.z()};
//         }
//
//         SOL_FORCE_INLINE
//         FVec3 operator-(FVec3 rhs) const {
//             return {x() - rhs.x(), y() - rhs.y(), z() - rhs.z()};
//         }
//
//         SOL_FORCE_INLINE
//         FVec3 operator*(FVec3 rhs) const {
//             return {x() * rhs.x(), y() * rhs.y(), z() * rhs.z()};
//         }
//
//         SOL_FORCE_INLINE
//         FVec3 operator/(FVec3 rhs) const {
//             return {x() / rhs.x(), y() / rhs.y(), z() / rhs.z()};
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator==(FVec3 rhs) const {
//             return {vceqq_f32(x(), rhs.x()), vceqq_f32(y(), rhs.y()), vceqq_f32(z(), rhs.z())};
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator!=(FVec3 rhs) const {
//             return ~(*this == rhs);
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator<(FVec3 rhs) const {
//             return {vcltq_f32(x(), rhs.x()), vcltq_f32(y(), rhs.y()), vcltq_f32(z(), rhs.z())};
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator<=(FVec3 rhs) const {
//             return {vcleq_f32(x(), rhs.x()), vcleq_f32(y(), rhs.y()), vcleq_f32(z(), rhs.z())};
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator>(FVec3 rhs) const {
//             return {vcgtq_f32(x(), rhs.x()), vcgtq_f32(y(), rhs.y()), vcgtq_f32(z(), rhs.z())};
//         }
//
//         SOL_FORCE_INLINE
//         UVec3 operator>=(FVec3 rhs) const {
//             return {vcgtq_f32(x(), rhs.x()), vcgtq_f32(y(), rhs.y()), vcgtq_f32(z(), rhs.z())};
//         }
//
//         SOL_FORCE_INLINE
//         FVec3 min(FVec3 rhs) const {
//             return {vminq_f32(x(), rhs.x()), vminq_f32(y(), rhs.y()), vminq_f32(z(), rhs.z())};
//         }
//
//         SOL_FORCE_INLINE
//         FVec3 max(FVec3 rhs) const {
//             return {vmaxq_f32(x(), rhs.x()), vmaxq_f32(y(), rhs.y()), vmaxq_f32(z(), rhs.z())};
//         }
//
//     private:
//         float32x4x3_t _components;
//     };
//
//     class Aabb3 {
//     public:
//         SOL_FORCE_INLINE
//         Aabb3(FVec3 min, FVec3 max): _min(min), _max(max) {}
//
//         SOL_FORCE_INLINE
//         FVec3 const& min() const {
//             return _min;
//         }
//
//         SOL_FORCE_INLINE
//         FVec3 const& max() const {
//             return _max;
//         }
//
//         SOL_FORCE_INLINE
//         FVec3& min() {
//             return _min;
//         }
//
//         SOL_FORCE_INLINE
//         FVec3& max() {
//             return _max;
//         }
//
//         SOL_FORCE_INLINE
//         FVec3 size() const {
//             return _max - _min;
//         }
//
//         SOL_FORCE_INLINE
//         float32x4_t area() const {
//             auto s = size();
//             return s.x() * s.y() * s.z();
//         }
//
//         SOL_FORCE_INLINE
//         Aabb3 union_(Aabb3 const& other) const {
//             return {_min.min(other._min), _max.max(other._max)};
//         }
//
//         // SOL_FORCE_INLINE
//         // uint32x4_t contains(Aabb3 const& other) const {
//         //
//         // }
//
//     private:
//         FVec3 _min;
//         FVec3 _max;
//     };
//
//     struct Child {
//         Aabb3 bounds;
//         uint32x4_t id;
//     };
//
//     template <size_t W>
//     struct InternalNode {
//         std::array<Child, W> children;
//     };
//
//     template <size_t W>
//     class RTree {
//     private:
//         std::vector<InternalNode<W>> _internalNodes;
//     };
// }
