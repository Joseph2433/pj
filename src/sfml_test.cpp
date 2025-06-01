// 简单的SFML测试程序
// 创建一个新的.cpp文件来测试基本SFML功能

#include <SFML/Graphics.hpp>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#endif

int main()
{
#ifdef _WIN32
    // 分配控制台窗口
    if (AllocConsole())
    {
        freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
        freopen_s((FILE **)stderr, "CONOUT$", "w", stderr);
        freopen_s((FILE **)stdin, "CONIN$", "r", stdin);
        std::ios::sync_with_stdio(true);
    }
#endif

    std::cout << "=== SFML Test Program Start ===" << std::endl;
    std::cout.flush();

    try
    {
        std::cout << "Testing SFML VideoMode..." << std::endl;
        sf::VideoMode mode(800, 600);
        std::cout << "VideoMode created: " << mode.width << "x" << mode.height << std::endl;

        std::cout << "Testing SFML Window creation..." << std::endl;
        sf::RenderWindow window;

        std::cout << "Attempting to create window..." << std::endl;
        window.create(mode, "SFML Test Window");

        if (!window.isOpen())
        {
            std::cerr << "ERROR: Window failed to open!" << std::endl;
            system("pause");
            return -1;
        }

        std::cout << "Window created successfully!" << std::endl;
        std::cout << "Window size: " << window.getSize().x << "x" << window.getSize().y << std::endl;

        // 创建一个简单的矩形
        sf::RectangleShape rectangle(sf::Vector2f(100, 50));
        rectangle.setFillColor(sf::Color::Red);
        rectangle.setPosition(350, 275);

        // 创建文本
        sf::Text text;
        text.setString("SFML Test - Press ESC to exit");
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(250, 200);

        std::cout << "Starting render loop..." << std::endl;

        int frameCount = 0;
        sf::Clock clock;

        while (window.isOpen())
        {
            frameCount++;
            if (frameCount % 60 == 0)
            {
                std::cout << "Frame: " << frameCount << ", FPS: " << (1.0f / clock.restart().asSeconds()) << std::endl;
            }

            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    std::cout << "Window close event" << std::endl;
                    window.close();
                }
                else if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        std::cout << "ESC key pressed" << std::endl;
                        window.close();
                    }
                }
            }

            window.clear(sf::Color::Blue);
            window.draw(rectangle);
            window.draw(text);
            window.display();
        }

        std::cout << "Window closed, exiting..." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        system("pause");
        return -1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception!" << std::endl;
        system("pause");
        return -1;
    }

    std::cout << "=== SFML Test Program End ===" << std::endl;
    system("pause");
    return 0;
}