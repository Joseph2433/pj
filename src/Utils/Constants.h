#pragma once

#define WINDOW_TITLE "Plants vs Zombies"

// 窗口设置
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 700;

// 网格设置 (你的 Grid.cpp 使用 GRID_WIDTH 作为 m_cellWidth, GRID_HEIGHT 作为 m_cellHeight)
const int GRID_ROWS = 5;
const int GRID_COLS = 9;
const int GRID_CELL_WIDTH = 90;  // 单元格宽度 (原 GRID_WIDTH)
const int GRID_CELL_HEIGHT = 90; // 单元格高度 (原 GRID_HEIGHT)
const int GRID_START_X = 250;
const int GRID_START_Y = 150;
const int CELL_PADDING = 2; // 这个常量目前在你的Grid.cpp中没用到，但可以保留

// 颜色常量
const int GRID_LINE_COLOR_R = 100;
const int GRID_LINE_COLOR_G = 100;
const int GRID_LINE_COLOR_B = 100;
const int GRID_LINE_COLOR_A = 128;

// 游戏常量
const float FRAME_RATE = 60.0f;
// const float DELTA_TIME = 1.0f / FRAME_RATE; // DELTA_TIME 通常在游戏循环中计算得出，而不是作为编译期常量

// 实体常量
const float ENTITY_DEFAULT_SCALE = 1.0f;

// 植物常量 (新增部分)
// -----------------------------------------------------------------------------
// 向日葵 (Sunflower)
const int SUNFLOWER_HEALTH = 80;
const int SUNFLOWER_COST = 50;
// 可以在这里定义向日葵生产阳光的间隔、数量等，例如：
// const float SUNFLOWER_SUN_PRODUCTION_INTERVAL = 24.0f; // 秒
// const int SUNFLOWER_SUN_AMOUNT = 25;

// 豌豆射手 (Peashooter)
const int PEASHOOTER_HEALTH = 100;
const int PEASHOOTER_COST = 100;
// 可以在这里定义豌豆射手的攻击力、射速等，例如：
// const float PEASHOOTER_SHOOT_INTERVAL = 1.5f; // 秒
// const int PEASHOOTER_PROJECTILE_DAMAGE = 20;

// 坚果墙 (WallNut) - 示例
const int WALLNUT_HEALTH = 1000; // 坚果墙生命值很高
const int WALLNUT_COST = 50;

// 土豆雷 (Potato Mine) - 示例
// const int POTATOMINE_HEALTH = 70;
// const int POTATOMINE_COST = 25;
// const float POTATOMINE_ARM_TIME = 15.0f; // 秒，准备时间

// 植物通用常量 (如果需要)
// const float PLANT_ANIMATION_SPEED = 0.1f;
// -----------------------------------------------------------------------------

// 僵尸常量 (未来添加)
// ...

// 阳光常量 (未来添加)
// const int SUN_VALUE_SMALL = 15;
// const int SUN_VALUE_NORMAL = 25;
// const int SUN_FALL_SPEED = 50; // pixels per second

// 其他游戏常量
// ...
