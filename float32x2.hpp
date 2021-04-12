#pragma once
#include <cmath>

struct float32x2 {
  constexpr float32x2() = default;
  constexpr explicit float32x2(float a) : x{a}, y{a} {}
  constexpr float32x2(float xx, float yy) : x{xx}, y{yy} {}

  float& operator[](int index) { return (&x)[index]; }
  const float& operator[](int index) const { return (&x)[index]; }

  float x{};
  float y{};
};

constexpr float32x2 operator+(float32x2 v, float32x2 w) {
  return {v.x + w.x, v.y + w.y};
}

constexpr float32x2& operator+=(float32x2& v, float32x2 w) {
  v.x += w.x;
  v.y += w.y;
  return v;
}

constexpr float32x2 operator-(float32x2 v, float32x2 w) {
  return {v.x - w.x, v.y - w.y};
}

constexpr float32x2& operator-=(float32x2& v, float32x2 w) {
  v.x -= w.x;
  v.y -= w.y;
  return v;
}

constexpr float32x2 operator*(float a, float32x2 w) {
  return {a * w.x, a * w.y};
}

constexpr float32x2 operator*(float32x2 v, float32x2 w) {
  return {v.x * w.x, v.y * w.y};
}

constexpr float32x2 operator/(float32x2 v, float32x2 w) {
  return {v.x / w.x, v.y / w.y};
}

constexpr float dot(float32x2 v, float32x2 w) { return v.x * w.x + v.y * w.y; }

constexpr float sqnorm(float32x2 v) { return dot(v, v); }

constexpr float norm(float32x2 v) { return sqrt(sqnorm(v)); }

constexpr float min(float32x2 v) { return std::min(v.x, v.y); }

constexpr float max(float32x2 v) { return std::max(v.x, v.y); }

constexpr float32x2 min(float32x2 v, float32x2 w) {
  return {std::min(v.x, w.x), std::min(v.y, w.y)};
}

constexpr float32x2 max(float32x2 v, float32x2 w) {
  return {std::max(v.x, w.x), std::max(v.y, w.y)};
}