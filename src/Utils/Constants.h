#pragma once
#define WINDOW_TITLE "Plants vs Zombies"

// 窗口设置
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 700;

// 网格设置
const int GRID_ROWS = 5;
const int GRID_COLS = 9;
const int GRID_WIDTH = 80;
const int GRID_HEIGHT = 100;
const int GRID_START_X = 250;
const int GRID_START_Y = 150;

// 颜色常量
const int GRID_LINE_COLOR_R = 100;
const int GRID_LINE_COLOR_G = 100;
const int GRID_LINE_COLOR_B = 100;
const int GRID_LINE_COLOR_A = 128;

// 游戏常量
const float FRAME_RATE = 60.0f;
const float DELTA_TIME = 1.0f / FRAME_RATE;

// 实体常量
const float ENTITY_DEFAULT_SCALE = 1.0f;