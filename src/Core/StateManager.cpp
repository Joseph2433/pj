#include "Core/StateManager.h"
#include "Core/GameState.h"
#include <iostream>

StateManager::StateManager()
    : m_pendingAction(StateAction::None), m_pendingState(nullptr)
{
}

StateManager::~StateManager()
{
    clearStates();
}

void StateManager::pushState(std::unique_ptr<GameState> state)
{
    m_pendingAction = StateAction::Push;
    m_pendingState = std::move(state);
}

void StateManager::popState()
{
    m_pendingAction = StateAction::Pop;
}

void StateManager::changeState(std::unique_ptr<GameState> state)
{
    m_pendingAction = StateAction::Change;
    m_pendingState = std::move(state);
}

void StateManager::clearStates()
{
    while (!m_states.empty())
    {
        m_states.top()->cleanup();
        m_states.pop();
    }
    m_pendingAction = StateAction::None;
    m_pendingState.reset();
}

void StateManager::handleEvents(const sf::Event &event)
{
    if (!m_states.empty())
    {
        m_states.top()->handleEvents(event);
    }
    processPendingAction();
}

void StateManager::update(float deltaTime)
{
    if (!m_states.empty())
    {
        m_states.top()->update(deltaTime);
    }
    processPendingAction();
}

void StateManager::render(sf::RenderWindow &window)
{
    if (!m_states.empty())
    {
        m_states.top()->render(window);
    }
}

bool StateManager::isEmpty() const
{
    return m_states.empty();
}

GameState *StateManager::getCurrentState() const
{
    if (!m_states.empty())
    {
        return m_states.top().get();
    }
    return nullptr;
}

void StateManager::processPendingAction()
{
    switch (m_pendingAction)
    {
    case StateAction::Push:
        if (m_pendingState)
        {
            if (!m_states.empty())
            {
                m_states.top()->pause();
            }
            m_states.push(std::move(m_pendingState));
            m_states.top()->init();
            std::cout << "State pushed" << std::endl;
        }
        break;

    case StateAction::Pop:
        if (!m_states.empty())
        {
            m_states.top()->cleanup();
            m_states.pop();
            if (!m_states.empty())
            {
                m_states.top()->resume();
            }
            std::cout << "State popped" << std::endl;
        }
        break;

    case StateAction::Change:
        if (m_pendingState)
        {
            if (!m_states.empty())
            {
                m_states.top()->cleanup();
                m_states.pop();
            }
            m_states.push(std::move(m_pendingState));
            m_states.top()->init();
            std::cout << "State changed" << std::endl;
        }
        break;

    case StateAction::Clear:
        clearStates();
        std::cout << "All states cleared" << std::endl;
        break;

    case StateAction::None:
    default:
        break;
    }

    m_pendingAction = StateAction::None;
    m_pendingState.reset();
}