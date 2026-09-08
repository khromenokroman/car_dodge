#pragma once
#include "constant.hpp"

#include <memory>
#include <raylib.h>
#include <vector>

struct Barrier {
  Barrier(float x, float y, float width, float height);
  explicit Barrier(Rectangle rec, int index);
  ~Barrier() = default;

  Rectangle m_rec;
  int m_index;
};

class GameData {
public:
  GameData();
  ~GameData() = default;

  GameData(GameData const &) = delete;
  GameData(GameData &&) = delete;
  GameData &operator=(GameData const &) = delete;
  GameData &operator=(GameData &&) = delete;

  void update();
  void render();
  void reset_game();

private:
  void update_car();
  void update_barriers();
  void update_road();
  void check_conflicts();
  static void texture_deleter(Texture2D *textura);

  std::vector<Barrier> m_barriers;
  std::vector<std::vector<Rectangle>> m_road;
  std::vector<std::unique_ptr<Texture2D, void (*)(Texture2D *)>>
      m_texture_barriers;
  std::vector<std::unique_ptr<Texture2D, void (*)(Texture2D *)>>
      m_texture_road;
  std::unique_ptr<Texture2D, void (*)(Texture2D *)> m_texture_player;
  Rectangle m_car{};
  double m_last_time{};
  std::size_t m_score{};
  GAME_STATE m_state{GAME_STATE::PLAYING};
};