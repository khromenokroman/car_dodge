#pragma once
#include <string_view>

constexpr static float WIDTH = 800;  // x
constexpr static float HEIGHT = 600; // y
constexpr static float CAR_WIDTH = 50;
constexpr static float CAR_HEIGHT = 100;
constexpr static float CAR_SPEED = 5;
constexpr static float BARRIER_WIDTH = 50;
constexpr static float BARRIER_HEIGHT = 100;
constexpr static float BARRIER_SPEED = 3;
constexpr static float ROAD_WIDTH = 600;
constexpr static int FPS = 60;
constexpr static std::string_view TITLE = "Car Dodge";
constexpr static std::string_view ASSETS_DIR = "/usr/share/car_dodge/images/";
enum class GAME_STATE { PLAYING, GAME_OVER};