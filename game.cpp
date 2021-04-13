#include <game.hpp>
//
#include <iostream>

using namespace std;

game::game() {
  init_platforms();
  init_player();
}

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

  player.animate(dt);

  // Apply gravity.
  player.a.y += 200;

  // Update velocity.
  player.v += dt * player.a;

  // Detect and resolve platform collisions.
  player.ground_contact = false;
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
    if (!cache.xhit && player.v.y > 0) player.ground_contact = true;

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

void game::init_player() {
  if (!player.texture.loadFromFile("adventurer-sheet.png"))
    throw runtime_error("Could not load given image!");
  player.size.y = 4;
  player.size.x = 1.5;
  player.texture_rect = {0, 0, 50, 37};
}

void game::character::animate(float dt) {
  // Update player looking direction.
  if (v.x > 0)
    right = true;
  else if (v.x < 0)
    right = false;

  once = false;
  if (!ground_contact) {
    if (v.y <= 0) {
      current_animation = &jump_animation;
      once = true;
    } else
      current_animation = &fall_animation;
  } else {
    if (v.x != 0)
      current_animation = &run_animation;
    else
      current_animation = &idle_animation;
  }

  if (frame >= current_animation->size()) {
    if (once)
      frame = current_animation->size() - 1;
    else
      frame = 0;
  }

  // Update texture rect.
  const auto [i, j] = (*current_animation)[frame];
  texture_rect.left = j * texture_rect.width;
  texture_rect.top = i * texture_rect.height;

  time += dt;
  if (time < frame_time) return;
  time = 0;
  ++frame;
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