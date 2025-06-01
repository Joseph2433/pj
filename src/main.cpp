#include "Core/Game.h"
#include <iostream>
#include <exception>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#endif

int main()
{
#ifdef _WIN32
    // 在Windows上分配控制台窗口（如果没有的话）
    if (AllocConsole())
    {
        freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
        freopen_s((FILE **)stderr, "CONOUT$", "w", stderr);
        freopen_s((FILE **)stdin, "CONIN$", "r", stdin);

        // 使输出同步
        std::ios::sync_with_stdio(true);
        std::cin.tie(nullptr);
    }
#endif

    std::cout << "=== Program Start ===" << std::endl;
    std::cout.flush(); // 强制刷新输出缓冲区

    try
    {
        std::cout << "Creating Game object..." << std::endl;
        std::cout.flush();

        Game game;

        std::cout << "Game object created successfully" << std::endl;
        std::cout.flush();

        std::cout << "Starting game.run()..." << std::endl;
        std::cout.flush();

        game.run();

        std::cout << "Game.run() completed" << std::endl;
        std::cout.flush();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Standard Exception caught: " << e.what() << std::endl;
        std::cerr.flush();
        system("pause"); // Windows特有，暂停程序
        return -1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception caught!" << std::endl;
        std::cerr.flush();
        system("pause");
        return -1;
    }

    std::cout << "=== Program End ===" << std::endl;
    system("pause"); // 防止窗口关闭
    return 0;
}