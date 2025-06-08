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

        GameState *newStatePtr = state.get();
        m_states.push_back(std::move(state));
        if (newStatePtr)
        {
            newStatePtr->enter();
        }
        std::cout << "Pushed new state. Stack size: " << m_states.size() << std::endl;
    }
}

void StateManager::popState()
{
    if (!m_states.empty())
    {
        m_states.back()->exit();
        m_states.pop_back();
        std::cout << "Popped state. Stack size: " << m_states.size() << std::endl;
    }
}

void StateManager::changeState(std::unique_ptr<GameState> state)
{
    if (!m_states.empty())
    {
        popState();
    }
    pushState(std::move(state));
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
        m_states.back()->update(deltaTime);
    }
}

void StateManager::render(sf::RenderWindow &window)
{
    // 从下往上渲染所有状态
    for (const auto &state_ptr : m_states)
    {
        if (state_ptr)
        {
            state_ptr->render(window);
        }
    }
}

void StateManager::handleEvent(const sf::Event &event)
{
    if (!m_states.empty())
    {
        m_states.back()->handleEvent(event);
    }
}

GameState *StateManager::getCurrentState() const
{
    if (!m_states.empty())
    {
        return m_states.back().get();
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