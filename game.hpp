#pragma once
#include <chrono>
#include <random>
#include <vector>
//
#include <aabb.hpp>

struct game {
  game();
  ~game();

  void update();

  void up();
  void down();
  void right();
  void left();
  void move(float x);
  void jump();

  void init_platforms();

  std::chrono::time_point<std::chrono::high_resolution_clock> old_time =
      std::chrono::high_resolution_clock::now();
  std::mt19937 rng{std::random_device{}()};

  struct character {
    float32x2 p{};
    float32x2 v{};
    float32x2 a{};
    float32x2 size{1, 2};
  } player;
  std::vector<aabb> platforms;
};