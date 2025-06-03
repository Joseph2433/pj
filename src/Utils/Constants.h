#pragma once

#include <string>
#include <SFML/System/Time.hpp> // For sf::Time

// --- Window & Game ---
#define WINDOW_TITLE "植物大战僵尸 克隆版"
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 700;
const float TARGET_FPS = 60.0f;
const sf::Time TIME_PER_FRAME = sf::seconds(1.f / TARGET_FPS);

// --- Grid ---
const int GRID_ROWS = 5;
const int GRID_COLS = 9;
const float GRID_CELL_WIDTH = 90.f;
const float GRID_CELL_HEIGHT = 90.f;
const float GRID_START_X = 250.f;
const float GRID_START_Y = 150.f;
const int GRID_LINE_COLOR_R = 100;
const int GRID_LINE_COLOR_G = 100;
const int GRID_LINE_COLOR_B = 100;
const int GRID_LINE_COLOR_A = 128;

// --- Sun ---
const int INITIAL_SUN_AMOUNT = 150;
const int SUN_VALUE_DEFAULT = 25;
const float SKY_SUN_FALL_SPEED = 70.f;
const float SKY_SUN_LIFESPAN_ON_GROUND = 8.0f;
const float PLANT_SUN_LIFESPAN = 7.0f;
const float SUN_COLLECT_RADIUS = 30.f; // 稍微增大点击半径
const std::string SUN_TEXTURE_KEY = "sun_texture";
const float PLANT_SUN_SPAWN_VELOCITY_Y = -90.f;
const float PLANT_SUN_SPAWN_VELOCITY_X_MAX_OFFSET = 25.f;
const float PLANT_SUN_GRAVITY = 280.f;
const float PLANT_SUN_TARGET_Y_OFFSET = 25.f;

// --- Plants ---
// Sunflower
const int SUNFLOWER_HEALTH = 80;
const int SUNFLOWER_COST = 50;
const float SUNFLOWER_COOLDOWN_TIME = 7.5f;
const float SUNFLOWER_SUN_PRODUCTION_INTERVAL = 10.0f; // 向日葵生产阳光间隔
const std::string SUNFLOWER_TEXTURE_KEY = "sunflower";
const std::string SUNFLOWER_ICON_TEXTURE_KEY = "sunflower_icon";

// Peashooter
const int PEASHOOTER_HEALTH = 100;
const int PEASHOOTER_COST = 100;
const float PEASHOOTER_COOLDOWN_TIME = 7.5f;
const std::string PEASHOOTER_TEXTURE_KEY = "peashooter";
const std::string PEASHOOTER_ICON_TEXTURE_KEY = "peashooter_icon";

// (Add other plants here)

// --- UI: SeedManager (Seed Packet Bar) & SeedPackets ---
const float SEED_PACKET_UI_START_X = 20.f;
const float SEED_PACKET_UI_START_Y = 10.f;
const float SEED_PACKET_WIDTH = 65.f;
const float SEED_PACKET_HEIGHT = 90.f;
const float SEED_PACKET_SPACING = 8.f;
const unsigned int SEED_PACKET_COST_FONT_SIZE = 16;
const unsigned int SEED_PACKET_COOLDOWN_FONT_SIZE = 14;
const float SEED_PACKET_ICON_SCALE_FACTOR = 0.75f; // 图标缩放比例
const float SEED_PACKET_COOLDOWN_OVERLAY_ALPHA = 170.f;

// --- UI: HUD (Sun Display) ---
// 根据种子包栏的位置和数量动态计算或预估一个位置
const int APPROX_NUM_SEED_PACKETS_DISPLAYED = 7; // 大约会显示多少个种子包
const float SUN_DISPLAY_X = SEED_PACKET_UI_START_X + (APPROX_NUM_SEED_PACKETS_DISPLAYED * (SEED_PACKET_WIDTH + SEED_PACKET_SPACING)) + 30.f;
const float SUN_DISPLAY_Y = SEED_PACKET_UI_START_Y + 15.f;
const unsigned int SUN_DISPLAY_FONT_SIZE = 24;
const std::string SUN_ICON_HUD_TEXTURE_KEY = "sun_icon_hud"; // HUD上阳光图标的键名

// --- Fonts ---
const std::string FONT_PATH_ARIAL = "C:/Windows/Fonts/arial.ttf";     // Windows默认
const std::string FONT_PATH_VERDANA = "C:/Windows/Fonts/verdana.ttf"; // 另一个Windows默认
// 推荐：将字体文件放在 assets/fonts/ 目录下，并使用相对路径
const std::string FONT_PATH_MAIN = "";
const std::string FONT_PATH_PRIMARY_ASSET = "assets/fonts/your_primary_font.ttf";
const std::string FONT_PATH_SECONDARY_ASSET = "assets/fonts/your_secondary_font.ttf";

const std::string FONT_ID_PRIMARY = "font_primary";
const std::string FONT_ID_SECONDARY = "font_secondary";
