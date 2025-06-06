#include "StateManager.h" 
#include "GameState.h"    
#include "Game.h"         
#include <iostream>
#include <stdexcept>

StateManager::StateManager(Game *game) : m_game(game)
{
    if (!m_game)
    {
        std::cerr << "FATAL ERROR: StateManager initialized with a null Game pointer!" << std::endl;
        throw std::logic_error("with a null Game pointer!");
    }
}

StateManager::~StateManager()
{
    clearStates(); 
}

void StateManager::pushState(std::unique_ptr<GameState> state)
{
    if (state)
    {
        if (!m_states.empty())
        {
            // 可选：如果希望新状态知道前一个状态，或者暂停前一个状态的更新
            // m_states.top()->pause();
        }
        m_states.push(std::move(state));
        m_states.top()->enter();
        std::cout << "Pushed new state. Stack size: " << m_states.size() << std::endl;
    }
}

void StateManager::popState()
{
    if (!m_states.empty())
    {
        m_states.top()->exit();
        m_states.pop();
        std::cout << "Popped state. Stack size: " << m_states.size() << std::endl;
        if (!m_states.empty())
        {
            // 可选：恢复栈顶状态
            // m_states.top()->resume();
        }
    }
}

void StateManager::changeState(std::unique_ptr<GameState> state)
{
    if (!m_states.empty())
    {
        popState(); // 先pop当前状态
    }
    pushState(std::move(state)); // 然后push新状态
}

void StateManager::clearStates()
{
    while (!m_states.empty())
    {
        popState();
    }
    std::cout << "All states cleared." << std::endl;
}

void StateManager::update(float deltaTime)
{
    if (!m_states.empty())
    {
        m_states.top()->update(deltaTime);
    }
}

void StateManager::render(sf::RenderWindow &window)
{
    if (!m_states.empty())
    {
        // 如果需要渲染栈中多个状态（例如暂停菜单覆盖游戏画面）
        // 可以考虑迭代渲染，但通常只渲染栈顶状态
        m_states.top()->render(window);
    }
}

void StateManager::handleEvent(const sf::Event &event)
{
    if (!m_states.empty())
    {
        m_states.top()->handleEvent(event);
    }
}

GameState *StateManager::getCurrentState() const
{
    if (!m_states.empty())
    {
        return m_states.top().get();
    }
    return nullptr;
}

bool StateManager::isEmpty() const
{
    return m_states.empty();
}

Game *StateManager::getGame() const
{
    return m_game;
}
