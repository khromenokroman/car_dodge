#include "game_data.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <ranges>
#include <string>

namespace {
std::string asset_path(std::string_view filename) {
  return std::string(ASSETS_DIR) + std::string(filename);
}
std::string music_path(std::string_view filename) {
  return std::string(MUSIC_DIR) + std::string(filename);
}
} // namespace

Barrier::Barrier(float x, float y, float width, float height)
    : m_rec{x, y, width, height}, m_index{0} {}
Barrier::Barrier(Rectangle rec, int index) : m_rec{rec}, m_index{index} {}

GameData::GameData()
    : m_texture_player(
          new Texture2D(LoadTexture(asset_path("player_car.png").c_str())),
          texture_deleter),
      m_sound(new Music(LoadMusicStream(music_path("arcade_song.ogg").c_str())),
              music_deleter) {
  PlayMusicStream(*m_sound);
  m_texture_barriers.emplace_back(
      new Texture2D(LoadTexture(asset_path("motorcycle_blue.png").c_str())),
      texture_deleter);
  m_texture_barriers.emplace_back(
      new Texture2D(LoadTexture(asset_path("motorcycle_green.png").c_str())),
      texture_deleter);
  m_texture_barriers.emplace_back(
      new Texture2D(LoadTexture(asset_path("motorcycle_red.png").c_str())),
      texture_deleter);
  m_texture_barriers.emplace_back(
      new Texture2D(LoadTexture(asset_path("motorcycle_yellow.png").c_str())),
      texture_deleter);
  m_texture_barriers.emplace_back(
      new Texture2D(LoadTexture(asset_path("car_yellow_3.png").c_str())),
      texture_deleter);
  m_texture_barriers.emplace_back(
      new Texture2D(LoadTexture(asset_path("car_green_small_1.png").c_str())),
      texture_deleter);
  m_texture_barriers.emplace_back(
      new Texture2D(LoadTexture(asset_path("car_blue_5.png").c_str())),
      texture_deleter);
  m_texture_road.emplace_back(
      new Texture2D(LoadTexture(asset_path("road_left.png").c_str())),
      texture_deleter);
  m_texture_road.emplace_back(
      new Texture2D(LoadTexture(asset_path("road.png").c_str())),
      texture_deleter);
  m_texture_road.emplace_back(
      new Texture2D(LoadTexture(asset_path("road_right.png").c_str())),
      texture_deleter);
  reset_game();
}

void GameData::update() {
  UpdateMusicStream(*m_sound);
  if (m_state == GAME_STATE::PLAYING) {
    update_car();
    update_barriers();
    update_road();
    check_conflicts();
  }
}
void GameData::render() {
  if (m_state == GAME_STATE::PLAYING) {
    // score
    auto const text_score = TextFormat("Score: %lu", m_score);
    auto const text_size = MeasureText(text_score, 32);
    DrawText(text_score, (int)WIDTH - text_size, 0, 32, WHITE);
    // road
    Rectangle source_road{.x = 0,
                          .y = 0,
                          .width = (float)m_texture_road[0]->width,
                          .height = (float)m_texture_road[0]->height};
    for (int col = 0; col < m_road.size(); ++col) {
      int textureIndex;
      if (col == 0) {
        textureIndex = 0; // левый край
      } else if (col == m_road.size() - 1) {
        textureIndex = 2; // правый край
      } else {
        textureIndex = 1; // середина
      }

      for (auto const &row : m_road[col]) {
        DrawTexturePro(*m_texture_road[textureIndex], source_road, row,
                       {.x = 0, .y = 0}, 0.0f, WHITE);
      }
    }
    // car
    Rectangle source_car{.x = 0,
                         .y = 0,
                         .width = (float)m_texture_player->width,
                         .height = (float)m_texture_player->height};
    DrawTexturePro(*m_texture_player, source_car, m_car, {.x = 0, .y = 0}, 0.0f,
                   WHITE);
    // barier
    for (auto const &barrier : m_barriers) {
      Rectangle source_barrier{
          .x = 0,
          .y = 0,
          .width = (float)m_texture_barriers[barrier.m_index]->width,
          .height = (float)m_texture_barriers[barrier.m_index]->height};
      float center_x = barrier.m_rec.x + BARRIER_WIDTH / 2;
      float center_y = barrier.m_rec.y + BARRIER_HEIGHT / 2;
      Rectangle dest_barrier{center_x, center_y, BARRIER_WIDTH, BARRIER_HEIGHT};
      DrawTexturePro(*m_texture_barriers[barrier.m_index], source_barrier,
                     dest_barrier, {BARRIER_WIDTH / 2, BARRIER_HEIGHT / 2},
                     180.0f, WHITE);
    }
  } else if (m_state == GAME_STATE::GAME_OVER) {
    auto const text_score = TextFormat("Score: %lu", m_score);
    auto const text_size = MeasureText(text_score, 48);
    DrawText(text_score, (WIDTH - text_size)/2, HEIGHT/2, 48, WHITE);
    DrawText("Press Enter the new game", 310, HEIGHT - 40, 36, WHITE);
    if (IsKeyPressed(KEY_ENTER)) {
      reset_game();
    }
  }
}
void GameData::reset_game() {
  m_state = GAME_STATE::PLAYING;
  m_car.x = ROAD_WIDTH / 2;
  m_car.y = HEIGHT - CAR_HEIGHT;
  m_car.width = CAR_WIDTH;
  m_car.height = CAR_HEIGHT;
  m_score = 0;
  m_barriers.clear();
}
void GameData::update_car() {
  if (IsKeyDown(KEY_RIGHT)) {
    m_car.x += CAR_SPEED;
  } else if (IsKeyDown(KEY_LEFT)) {
    m_car.x -= CAR_SPEED;
  } else if (IsKeyDown(KEY_UP)) {
    m_car.y -= CAR_SPEED;
  } else if (IsKeyDown(KEY_DOWN)) {
    m_car.y += CAR_SPEED;
  }
  m_car.x = std::clamp(m_car.x, static_cast<float>(0), ROAD_WIDTH - CAR_WIDTH);
  m_car.y = std::clamp(m_car.y, static_cast<float>(0), HEIGHT - CAR_HEIGHT);
}
void GameData::update_barriers() {
  auto cur_time = GetTime();
  if (cur_time - m_last_time > 0.8) {
    while (true) {
      auto i1 = GetRandomValue(0, (int)m_texture_barriers.size() - 1);
      auto i2 = GetRandomValue(0, (int)m_texture_barriers.size() - 1);
      auto pos1_x = (float)GetRandomValue(0, ROAD_WIDTH - BARRIER_WIDTH);
      auto pos2_x = (float)GetRandomValue(0, ROAD_WIDTH - BARRIER_WIDTH);
      Rectangle rec1{pos1_x, 0 - BARRIER_HEIGHT, BARRIER_WIDTH, BARRIER_HEIGHT};
      Rectangle rec2{pos2_x, 0 - BARRIER_HEIGHT, BARRIER_WIDTH, BARRIER_HEIGHT};
      Barrier b1(rec1, i1);
      Barrier b2(rec2, i2);

      if (!CheckCollisionRecs(rec1, rec2)) {
        m_barriers.emplace_back(b1);
        m_barriers.emplace_back(b2);
        break;
      }
    }
    m_last_time = cur_time;
  }
  for (auto &barrier : m_barriers) {
    barrier.m_rec.y += BARRIER_SPEED;
  }
  m_score += std::erase_if(m_barriers, [](Barrier const &barrier) {
    return barrier.m_rec.y > HEIGHT;
  });
}
void GameData::update_road() {
  int cols = ROAD_WIDTH / 150;
  int rows = HEIGHT / 150;
  m_road.clear();
  for (int col = 0; col < cols; ++col) {
    std::vector<Rectangle> column;
    for (int row = 0; row < rows; ++row) {
      column.emplace_back(col * 150, row * 150, 150, 150);
    }
    m_road.emplace_back(column);
  }
}
void GameData::check_conflicts() {
  for (auto const &barrier : m_barriers) {
    if (CheckCollisionRecs(barrier.m_rec, m_car)) {
      m_state = GAME_STATE::GAME_OVER;
    }
  }
}
void GameData::texture_deleter(Texture2D *textura) {
  if (textura && textura->id != 0) {
    UnloadTexture(*textura);
  }
  delete textura;
}
void GameData::music_deleter(Music *music) {
  if (music) {
    UnloadMusicStream(*music);
  }
  delete music;
}
