#pragma once

#include <string>
#include <SFML/System/Time.hpp> // For sf::Time

// --- Window & Game ---
#define WINDOW_TITLE "JOSEPH'S OOP PROJECT"
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
const float PEASHOOTER_SHOOT_INTERVAL = 1.4f; // 豌豆射手两次射击之间的间隔时间 (秒)

// Wall-nut
const int WALLNUT_HEALTH = 4000;
const int WALLNUT_COST = 50;
const float WALLNUT_COOLDOWN_TIME = 20.0f;                 // 坚果墙冷却时间通常较长
const std::string WALLNUT_TEXTURE_KEY = "wallnut_texture"; // 确保与加载时一致
const std::string WALLNUT_ICON_TEXTURE_KEY = "wallnut_icon";

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

// --- 子弹 (Projectiles) ---

// **豌豆 (Pea)**
const float PEA_SPEED = 350.f; // 豌豆子弹的飞行速度 (单位：像素/秒)
                               // 你可以调整这个值来改变子弹快慢

const int PEA_DAMAGE = 20; // 豌豆子弹对僵尸造成的伤害值
                           // 普通僵尸可能需要承受多次攻击才会死亡

const std::string PEA_TEXTURE_KEY = "pea_projectile"; // 豌豆子弹纹理在 ResourceManager 中的键名
                                                      // 确保这个键名与你加载纹理时使用的ID一致
                                                      // 例如: resourceManager.loadTexture("pea_projectile", "assets/images/projectiles/pea.png");

// (可选) 子弹的近似尺寸，主要用于粗略的边界检测或非常简单的碰撞（更精确的碰撞用 getGlobalBounds()）
// 这些值应该大致匹配你的 pea.png 图片中豌豆视觉部分的大小
const float PEA_VISUAL_WIDTH = 13.f;  // 豌豆视觉上的宽度 (像素)
const float PEA_VISUAL_HEIGHT = 13.f; // 豌豆视觉上的高度 (像素)

// (可选) 子弹的生命周期（如果子弹飞行一定时间后会自动消失，即使没打中或飞出屏幕）
// 设置为负数或很大的数表示它主要依赖飞出屏幕或击中目标来销毁
const float PEA_LIFESPAN_SECONDS = 3.0f; // 例如，豌豆飞行3秒后消失 (如果设为 > 0)
                                         // 如果希望它一直飞直到出界，可以将此值设为 -1.f 或一个非常大的数，
                                         // 并在 Projectile 基类中相应处理。
                                         // 在我们之前的 Projectile 实现中，lifespan < 0 表示无限。

// --- 其他类型的子弹 (未来扩展) ---
// 例如：冰冻豌豆 (Ice Pea)
// const float ICE_PEA_SPEED = 320.f;
// const int ICE_PEA_DAMAGE = 20; // 伤害可能与普通豌豆相同，但有减速效果
// const float ICE_PEA_SLOW_DURATION = 3.0f; // 减速效果持续时间
// const std::string ICE_PEA_TEXTURE_KEY = "ice_pea_projectile";
// const float ICE_PEA_VISUAL_WIDTH = 15.f;
// const float ICE_PEA_VISUAL_HEIGHT = 15.f;

// --- 僵尸 (Zombies) ---
// 普通僵尸 (Basic Zombie)
const int BASIC_ZOMBIE_HEALTH = 100;                         // 普通僵尸的生命值
const float BASIC_ZOMBIE_SPEED = 25.f;                       // 普通僵尸的移动速度 (像素/秒，向左为负)
                                                             // 注意：速度通常设为正数，方向在逻辑中处理
const int BASIC_ZOMBIE_DAMAGE_PER_ATTACK = 10;               // 普通僵尸每次攻击对植物造成的伤害 (未来使用)
const float BASIC_ZOMBIE_ATTACK_INTERVAL = 1.0f;             // 普通僵尸的攻击间隔 (秒) (未来使用)
const std::string BASIC_ZOMBIE_TEXTURE_KEY = "basic_zombie"; // 普通僵尸纹理的键名
const float ZOMBIE_ATTACK_RANGE = 5.f;                       // 僵尸开始攻击植物的近距离阈值 (像素)
// (可选) 僵尸动画相关常量，如帧数、帧延迟等

// 僵尸生成相关
const float ZOMBIE_SPAWN_START_X_OFFSET = 50.f; // 僵尸在屏幕右侧外多少像素处生成
const float ZOMBIE_REACHED_HOUSE_X = 180.f;     // 僵尸到达这个X坐标时，玩家失败 (需要根据你的游戏布局调整)
                                                // 例如，在网格系统左侧或房子的位置