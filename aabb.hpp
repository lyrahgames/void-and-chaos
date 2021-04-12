#pragma once
#include <iostream>
//
#include <float32x2.hpp>

struct aabb {
  constexpr aabb() = default;
  constexpr aabb(float32x2 p) : min{p}, max{p} {}
  constexpr aabb(float32x2 p, float32x2 q)
      : min{::min(p, q)}, max{::max(p, q)} {}
  constexpr aabb(float px, float py, float qx, float qy)
      : aabb{float32x2{px, py}, float32x2{qx, qy}} {}

  float32x2 min{};
  float32x2 max{};
};

constexpr bool is_overlapping(const aabb& a, const aabb& b) {
  return (a.max.x >= b.min.x) && (a.min.x <= b.max.x) &&  //
         (a.max.y >= b.min.y) && (a.min.y <= b.max.y);
}

struct intersection_t {
  operator bool() const { return is_intersecting; }
  bool operator<=(float t) { return tmin <= t; }
  bool operator>=(float t) { return tmax >= t; }

  bool is_intersecting;
  float tmin, tmax;
  float32x2 normal;
  bool xhit;
};

constexpr intersection_t intersection(const aabb& a, const aabb& b,
                                      float32x2 d) {
  const auto t1xy = (a.min - b.max) / d;
  const auto t2xy = (a.max - b.min) / d;
  const auto tminxy = min(t1xy, t2xy);
  const auto tmaxxy = max(t1xy, t2xy);
  const auto tmin = max(tminxy);
  const auto tmax = min(tmaxxy);

  intersection_t result;
  // result.is_intersecting = (tmin <= tmax) && (tmax >= 0) && (tmin <= 1);
  result.is_intersecting = (tmin <= tmax);
  result.tmin = tmin;
  result.tmax = tmax;
  result.normal.x = (tminxy.x > tminxy.y) ? ((d.x < 0) ? 1 : -1) : (0);
  result.normal.y = (tminxy.x > tminxy.y) ? (0) : ((d.y < 0) ? 1 : -1);
  result.xhit = tminxy.x > tminxy.y;
  return result;
}