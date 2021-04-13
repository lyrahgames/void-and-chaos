#pragma once
#include <chrono>
#include <random>
#include <vector>
//
#include <SFML/Graphics.hpp>
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
  void init_player();

  std::chrono::time_point<std::chrono::high_resolution_clock> old_time =
      std::chrono::high_resolution_clock::now();
  std::mt19937 rng{std::random_device{}()};

  struct character {
    float32x2 p{};
    float32x2 v{};
    float32x2 a{};
    float32x2 size{1, 2};
    sf::Texture texture;
    sf::IntRect texture_rect;
    bool right = true;
    bool ground_contact = false;
    bool once = false;
    float time = 0;
    float frame_time = 0.1f;
    size_t frame = 0;
    std::vector<std::pair<int, int>>  //
        idle_animation{{0, 0}, {0, 1}, {0, 2}, {0, 3}};
    std::vector<std::pair<int, int>>  //
        run_animation{{1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}};
    std::vector<std::pair<int, int>>  //
        jump_animation{{2, 0}, {2, 1}, {2, 2}, {2, 3}};
    std::vector<std::pair<int, int>>  //
        fall_animation{{3, 1}, {3, 2}};

    std::vector<std::pair<int, int>>* current_animation = &idle_animation;
    void animate(float dt);
  } player;
  std::vector<aabb> platforms;
};