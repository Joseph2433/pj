#include "StateManager.h"

void StateManager::pushState(std::unique_ptr<GameState> state)
{
    if (state)
    {
        m_states.push(std::move(state));
        m_states.top()->enter();
    }
}

void StateManager::popState()
{
    if (!m_states.empty())
    {
        m_states.top()->exit();
        m_states.pop();
    }
}

void StateManager::changeState(std::unique_ptr<GameState> state)
{
    if (!m_states.empty())
    {
        m_states.top()->exit();
        m_states.pop();
    }
    pushState(std::move(state));
}

void StateManager::clearStates()
{
    while (!m_states.empty())
    {
        m_states.top()->exit();
        m_states.pop();
    }
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