#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    m_Player = std::make_shared<PLAYER>(RESOURCE_DIR"/Image/Character/player_1.png");
    m_Root.AddChild(m_Player);

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    if (m_GameState == GameState::PLAYING) {

        // 玩家移動
        if (Util::Input::IsKeyPressed(Util::Keycode::LEFT))  m_Player->Move(-1, 0);
        if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) m_Player->Move(1, 0);

        // 之後在這裡加：敵人移動、子彈、碰撞偵測
    }

    if (m_GameState == GameState::START_SCREEN) {
        // 按空白鍵開始遊戲
        if (Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
            m_GameState = GameState::PLAYING;
        }
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    m_Root.Update();
}

void App::End() {
    LOG_TRACE("End");
}