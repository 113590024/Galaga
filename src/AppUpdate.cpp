#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"

//測試用
#include "Util/Logger.hpp"

void App::Update() {

    // 背景捲動只在非暫停時執行
    if (m_GameState != GameState::PAUSED) {
        m_BgScrollY -= 3.0f;
        m_Bg1->SetPosition({0.0f, m_BgScrollY});
        m_Bg2->SetPosition({0.0f, m_BgScrollY + 720.0f});
        if (m_BgScrollY < -720.0f) {
            m_BgScrollY = 0.0f;
        }
        m_BgSwitchTimer += Util::Time::GetDeltaTimeMs();
        if (m_BgSwitchTimer >= 300.0f) {
            m_BgSwitchTimer = 0.0f;
            m_BgIndex = (m_BgIndex + 1) % 3;
            m_Bg1->SetImage(m_BgPaths[m_BgIndex]);
            m_Bg2->SetImage(m_BgPaths[m_BgIndex]);
        }
    }

    // 開始畫面
    if (m_GameState == GameState::START_SCREEN) {
        if (Util::Input::IsKeyUp(Util::Keycode::UP)) {
            m_MenuIndex = 0;
            m_Cursor->SetPosition({-130.0f, -50.0f});
        }
        if (Util::Input::IsKeyUp(Util::Keycode::DOWN)) {
            m_MenuIndex = 1;
            m_Cursor->SetPosition({-130.0f, -90.0f});
        }
        if (Util::Input::IsKeyUp(Util::Keycode::RETURN)) {
            m_GameState = GameState::PLAYING;
            m_Logo->SetVisible(false);
            m_Text1P->SetVisible(false);
            m_Text2P->SetVisible(false);
            m_Cursor->SetVisible(false);
            m_Player->SetVisible(true);
        }
    }

    // 遊戲進行中
    if (m_GameState == GameState::PLAYING) {
        if (Util::Input::IsKeyPressed(Util::Keycode::LEFT))  m_Player->Move(-1, 0);
        if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) m_Player->Move(1, 0);

        // 之後加：敵人移動、子彈、碰撞偵測
        //按Z或X發射子彈
        m_ShootCooldown -= Util::Time::GetDeltaTimeMs();
        if ((Util::Input::IsKeyDown(Util::Keycode::Z) && m_ShootCooldown <= 0.0f)
            || (Util::Input::IsKeyDown(Util::Keycode::X) && m_ShootCooldown <= 0.0f)){
            auto bullet = std::make_shared<Player_bullet>(m_Player->GetPosition());
            m_Bullets.push_back(bullet);
            m_Root.AddChild(bullet);
            m_ShootCooldown = 100.0f; // 0.1秒冷卻
        }
        // 更新子彈位置，移除出畫面的子彈
        for (auto& bullet : m_Bullets) {
            bullet->flyUp();
        }
        m_Bullets.erase(
            std::remove_if(m_Bullets.begin(), m_Bullets.end(),
                [](const auto& b) { return b->IsOutOfScreen(); }),
            m_Bullets.end()
        );

        if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
            m_GameState = GameState::PAUSED;
            m_PauseText->SetVisible(true);
        }

    }

    // 暫停中（什麼都不動，只等待繼續）
    if (m_GameState == GameState::PAUSED) {
        if (Util::Input::IsKeyUp(Util::Keycode::RETURN)) {
            m_GameState = GameState::PLAYING;
            m_PauseText->SetVisible(false);
        }
    }

    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    m_Root.Update();
}