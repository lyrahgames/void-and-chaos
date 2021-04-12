#include <game.hpp>
//
#include <iostream>

using namespace std;

game::game() { init_platforms(); }

game::~game() {}

void game::up() { player.v.y -= 20; }

void game::down() { player.v.y += 20; }

void game::right() { player.v.x += 20; }

void game::left() { player.v.x += -20; }

void game::jump() { player.v.y -= 80; }

void game::move(float x) { player.v.x += x; }

void game::update() {
  // Compute time step.
  const auto new_time = chrono::high_resolution_clock::now();
  const auto dt = chrono::duration<float>(new_time - old_time).count();
  old_time = new_time;

  // Apply gravity.
  player.a.y += 200;

  // Update velocity.
  player.v += dt * player.a;

  // Detect and resolve platform collisions.
  auto new_p = player.p + dt * player.v;
  const aabb new_hit_box{new_p - 0.5f * player.size,
                         new_p + 0.5f * player.size};
  const aabb hit_box{player.p - 0.5f * player.size,
                     player.p + 0.5f * player.size};

  for (const auto& p : platforms) {
    // if (is_overlapping(p, new_hit_box)) {
    const auto cache = intersection(p, hit_box, player.v);

    if (!((cache.tmin <= cache.tmax) && (cache.tmax >= 0) &&
          (cache.tmin <= dt)))
      continue;

    const auto p_scale = dot(player.v, cache.normal);
    new_p += (1e-2f - (dt - cache.tmin) * p_scale) * cache.normal;
    const auto v_scale = min(0.0f, p_scale);
    player.v -= v_scale * cache.normal;
    // }
  }

  // Update position.
  player.p = new_p;

  // Apply simple friction.
  player.v.x = 0;
  player.v.y *= 0.98f;

  // Update acceleration.
  player.a = float32x2{};
}

void game::init_platforms() {
  platforms.push_back({-1000, 10, 1000, 1000});
  platforms.push_back({2, -5, 10, -4});
  platforms.push_back({-10, 2, -5, 3});

  uniform_real_distribution<float> uniform{-100, 100};

  for (size_t i = 0; i < 100; ++i) {
    const auto x = uniform(rng);
    const auto y = uniform(rng);
    platforms.push_back({x - 5, y - 1, x + 5, y + 1});
  }
}