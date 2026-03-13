#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Util/Renderer.hpp"
#include "PLAYER.hpp"

class App{
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    enum class GameState {
        START_SCREEN,
        PLAYING,
        PAUSED,
        GAME_OVER,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();
    State m_CurrentState = State::START;
    GameState m_GameState = GameState::START_SCREEN;

    Util::Renderer m_Root;

    std::shared_ptr<PLAYER> m_Player;
};
#endif
