#pragma once // 防止头文件被多次包含

#include <SFML/Graphics/Texture.hpp>  // SFML 纹理
#include <SFML/Graphics/Font.hpp>     // SFML 字体
#include <SFML/Audio/SoundBuffer.hpp> // SFML 音效缓冲 (如果你使用音效)
#include <string>                     // 标准字符串
#include <map>                        // 标准映射容器 (用于存储资源)
#include <memory>                     // 标准智能指针 (std::unique_ptr)

// 资源管理器类，负责加载、存储和提供游戏资源（纹理、字体、音效等）
class ResourceManager
{
public:
    // 构造函数：可以初始化默认的备用资源
    ResourceManager();
    // 默认析构函数：unique_ptr 会自动管理其指向的资源内存
    ~ResourceManager() = default;

    // --- 纹理管理 ---
    // 加载纹理文件，并使用给定的ID进行注册
    // id: 纹理的唯一标识符 (字符串)
    // filename: 纹理图片文件的路径
    // 返回 true 如果加载成功，否则返回 false
    bool loadTexture(const std::string &id, const std::string &filename);
    // 根据ID获取已加载纹理的常量引用
    // 如果找不到指定ID的纹理，则返回一个默认纹理
    const sf::Texture &getTexture(const std::string &id) const;
    // 检查是否存在具有给定ID的纹理
    bool hasTexture(const std::string &id) const;

    // --- 字体管理 ---
    // 加载字体文件，并使用给定的ID进行注册
    // id: 字体的唯一标识符
    // filename: 字体文件的路径
    // 返回 true 如果加载成功，否则返回 false
    bool loadFont(const std::string &id, const std::string &filename);
    // 根据ID获取已加载字体的常量引用
    // 如果找不到指定ID的字体，则返回一个默认字体
    const sf::Font &getFont(const std::string &id) const;
    // 检查是否存在具有给定ID的字体
    bool hasFont(const std::string &id) const;

    // --- 音效缓冲管理 (如果项目中需要音效) ---
    // bool loadSoundBuffer(const std::string& id, const std::string& filename);
    // const sf::SoundBuffer& getSoundBuffer(const std::string& id) const;
    // bool hasSoundBuffer(const std::string& id) const;

private:
    // 私有辅助方法，用于在构造时创建默认的备用资源
    // 当请求的资源无法加载或找不到时，可以返回这些默认资源，避免程序崩溃
    void createDefaultResources();

    // --- 资源存储容器 ---
    // 使用 std::map 将字符串ID映射到指向SFML资源的智能指针
    // std::unique_ptr 确保资源在不再被引用时自动释放内存
    std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::unique_ptr<sf::Font>> m_fonts;
    // std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;

    // --- 备用/默认资源实例 ---
    // 这些是在 createDefaultResources() 中创建的，当 getXxx() 找不到ID时返回
    sf::Texture m_defaultTexture; // 默认纹理 (例如，一个品红色的方块)
    sf::Font m_defaultFont;       // 默认字体 (例如，尝试加载一个系统字体)
    // sf::SoundBuffer m_defaultSoundBuffer; // 默认音效缓冲
};
