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
            // 可选: 调用当前栈顶状态的 pause() 方法
            // m_states.back()->pause(); // .back() 获取最后一个元素 (即栈顶)
        }
        // GameState 的 enter() 应该在它被加入并成为活动状态后调用
        // 所以先保存指针，再 push，然后调用 enter
        GameState *newStatePtr = state.get();
        m_states.push_back(std::move(state)); // vector 的 push_back 模拟栈的 push
        if (newStatePtr)
        {
            newStatePtr->enter(); // 对新加入的栈顶状态调用 enter
        }
        std::cout << "Pushed new state. Stack size: " << m_states.size() << std::endl;
    }
}

void StateManager::popState()
{
    if (!m_states.empty())
    {
        m_states.back()->exit(); // 调用栈顶状态的 exit()
        m_states.pop_back();     // vector 的 pop_back 模拟栈的 pop (unique_ptr 会自动释放资源)
        std::cout << "Popped state. Stack size: " << m_states.size() << std::endl;
        if (!m_states.empty())
        {
            // 可选: 调用新的栈顶状态的 resume() 方法
            // m_states.back()->resume();
        }
    }
}

void StateManager::changeState(std::unique_ptr<GameState> state)
{
    if (!m_states.empty())
    {
        popState(); // 先移除当前状态
    }
    pushState(std::move(state)); // 然后推入新状态
}

void StateManager::clearStates()
{
    while (!m_states.empty())
    {
        popState(); // 依次弹出所有状态，会调用它们的 exit()
    }
    std::cout << "All states cleared." << std::endl;
}

void StateManager::update(float deltaTime)
{
    if (!m_states.empty())
    {
        m_states.back()->update(deltaTime); // 只更新栈顶状态
    }
}

// 使用 std::vector 后，render 方法变得非常简单
void StateManager::render(sf::RenderWindow &window)
{
    // 从下往上渲染所有状态 (vector的迭代顺序就是从第一个元素到最后一个)
    // 这自然地实现了背景状态先渲染，覆盖型状态后渲染的效果
    for (const auto &state_ptr : m_states)
    {
        if (state_ptr)
        { // 确保指针有效
            state_ptr->render(window);
        }
    }
}

void StateManager::handleEvent(const sf::Event &event)
{
    if (!m_states.empty())
    {
        m_states.back()->handleEvent(event); // 只让栈顶状态处理事件
    }
}

GameState *StateManager::getCurrentState() const
{
    if (!m_states.empty())
    {
        return m_states.back().get(); // .back() 获取最后一个元素 (栈顶)
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