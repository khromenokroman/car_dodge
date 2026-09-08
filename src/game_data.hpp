#pragma once
#include "constant.hpp"

#include <memory>
#include <raylib.h>
#include <vector>

/**
 * @brief Одно препятствие (барьер), падающее сверху вниз по дороге.
 *
 * Хранит прямоугольник для позиции/коллизии и индекс текстуры (мотоцикл
 * или машина), выбранной один раз при создании барьера.
 */
struct Barrier {
  /**
   * @brief Создаёт барьер с текстурой по умолчанию (индекс 0).
   * @param x Координата X левого верхнего угла.
   * @param y Координата Y левого верхнего угла.
   * @param width Ширина барьера.
   * @param height Высота барьера.
   */
  Barrier(float x, float y, float width, float height);

  /**
   * @brief Создаёт барьер с заданным прямоугольником и текстурой.
   * @param rec Прямоугольник позиции/размера барьера.
   * @param index Индекс текстуры в списке текстур барьеров.
   */
  explicit Barrier(Rectangle rec, int index);
  ~Barrier() = default;

  Rectangle m_rec;
  int m_index;
};

/**
 * @brief Хранит и обновляет состояние игры Car Dodge (машинка игрока,
 * барьеры, дорога, счёт).
 *
 * Владеет всеми игровыми данными и текстурами и предоставляет два основных
 * метода — update() и render() — которые вызываются каждый кадр из
 * игрового цикла.
 */
class GameData {
public:
  /**
   * @brief Создаёт игру в начальном состоянии: загружает текстуры машинки,
   * барьеров и дороги, генерирует сетку дороги и сбрасывает состояние
   * игры в PLAYING.
   */
  GameData();
  ~GameData() = default;

  GameData(GameData const &) = delete;
  GameData(GameData &&) = delete;
  GameData &operator=(GameData const &) = delete;
  GameData &operator=(GameData &&) = delete;

  /**
   * @brief Обновляет состояние игры на один кадр.
   *
   * Обрабатывает движение машинки, спавн и движение барьеров, генерацию
   * дороги и проверку столкновений только в состоянии PLAYING.
   */
  void update();

  /**
   * @brief Отрисовывает текущее состояние игры.
   *
   * В состоянии PLAYING рисует счёт, дорогу, машинку игрока и барьеры.
   * В состоянии GAME_OVER рисует сообщение и обрабатывает нажатие
   * KEY_ENTER для перехода в новую игру.
   */
  void render();

  /**
   * @brief Сбрасывает игру к начальному состоянию.
   *
   * Возвращает машинку в исходную позицию, обнуляет счёт и очищает
   * список барьеров.
   */
  void reset_game();

private:
  void update_car();
  void update_barriers();
  void update_road();
  void check_conflicts();
  static void texture_deleter(Texture2D *textura);
  static void music_deleter(Music *music);

  std::vector<Barrier> m_barriers;
  std::vector<std::vector<Rectangle>> m_road;
  std::vector<std::unique_ptr<Texture2D, void (*)(Texture2D *)>>
      m_texture_barriers;
  std::vector<std::unique_ptr<Texture2D, void (*)(Texture2D *)>>
      m_texture_road;
  std::unique_ptr<Texture2D, void (*)(Texture2D *)> m_texture_player;
  std::unique_ptr<Music, void (*)(Music *)> m_sound;
  Rectangle m_car{};
  double m_last_time{};
  std::size_t m_score{};
  GAME_STATE m_state{GAME_STATE::PLAYING};
};