#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"
#include "Util/Renderer.hpp"
#include "Util/GameObject.hpp"
#include "PLAYER.hpp"
#include "Sprite.hpp"
#include "Label.hpp"
#include "Cursor.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END
    };
    enum class GameState {
        START_SCREEN,
        PLAYING,
        PAUSED,
        GAME_OVER
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void End();

private:
    State m_CurrentState = State::START;
    GameState m_GameState = GameState::START_SCREEN;

    Util::Renderer m_Root;

    // 玩家
    std::shared_ptr<PLAYER> m_Player;
    int m_MenuIndex = 0; // 0 = 1 PLAYER, 1 = 2 PLAYERS

    // 封面
    std::shared_ptr<Sprite> m_Logo;
    std::shared_ptr<Label> m_Text1P;
    std::shared_ptr<Label> m_Text2P;

    // 背景
    std::shared_ptr<Sprite> m_Bg1;
    std::shared_ptr<Sprite> m_Bg2;
    float m_BgScrollY = 0.0f;
    float m_BgSwitchTimer = 0.0f;
    int m_BgIndex = 0;
    std::vector<std::string> m_BgPaths;

    // 箭頭
    std::shared_ptr<Cursor> m_Cursor;

    // 暫停文字
    std::shared_ptr<Label> m_PauseText;
};

#endif