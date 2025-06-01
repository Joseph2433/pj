#ifndef CONSTANTS_H
#define CONSTANTS_H

// 窗口尺寸设置，参考经典植物大战僵尸的分辨率
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

// 游戏网格设置，经典PVZ是9列5行的布局
const int GRID_COLS = 9;
const int GRID_ROWS = 5;
const int GRID_CELL_WIDTH = 80;
const int GRID_CELL_HEIGHT = 100;

// 游戏区域在屏幕上的位置
const int GRID_OFFSET_X = 250;
const int GRID_OFFSET_Y = 160;

// 植物和僵尸的基础属性
const int PEASHOOTER_COST = 100; // 豌豆射手的阳光消耗
const int SUNFLOWER_COST = 50;   // 向日葵的阳光消耗
const int WALLNUT_COST = 50;     // 坚果墙的阳光消耗

const int BASIC_ZOMBIE_HEALTH = 100; // 普通僵尸的生命值
const int ZOMBIE_SPEED = 20;         // 僵尸移动速度（像素/秒）

// 其他游戏参数
const int STARTING_SUN = 50;             // 游戏开始时的阳光数量
const float SUN_GENERATION_TIME = 10.0f; // 向日葵产生阳光的间隔时间

#endif