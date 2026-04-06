#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"

//測試用
#include "Util/Logger.hpp"

void App::Update() {

    // 背景捲動只在非暫停時執行
    if (m_GameState != GameState::PAUSED) {
        m_BgScrollY -= 1.0f;
        m_Bg1->SetPosition({-160.0f, m_BgScrollY});
        m_Bg2->SetPosition({-160.0f, m_BgScrollY + 720.0f});
        if (m_BgScrollY < -720.0f) {
            m_BgScrollY = 0.0f;
        }
        m_BgSwitchTimer += Util::Time::GetDeltaTimeMs();
        if (m_BgSwitchTimer >= 600.0f) {
            m_BgSwitchTimer = 0.0f;
            m_BgIndex = (m_BgIndex + 1) % 3;
            m_Bg1->SetImage(m_BgPaths[m_BgIndex]);
            m_Bg2->SetImage(m_BgPaths[m_BgIndex]);
        }
    }

    // 開始畫面
    if (m_GameState == GameState::START_SCREEN) {
        // 開場動畫
        if (m_IntroPlaying) {
            m_IntroY += 5.0f; // 動畫速度
            if (m_IntroY >= m_IntroTargetY) {
                m_IntroY = m_IntroTargetY;
                m_IntroPlaying = false;
            }
            m_Logo->SetPosition({0.0f, m_IntroY + 80.0f});
            m_Text1P->SetPosition({0.0f, m_IntroY - 50.0f});
            m_Text2P->SetPosition({0.0f, m_IntroY - 90.0f});
            m_Cursor->SetVisible(false);
        }

        // 動畫結束
        if (!m_IntroPlaying) {
            m_Cursor->SetVisible(true); // 顯示箭頭
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
                m_ScoreLabel->SetVisible(true);
                m_LivesLabel->SetVisible(true);

                // 顯示敵人
                for (auto& enemy : m_Enemies) {
                    enemy->SetVisible(true);
                }
            }
        }

    }

    // 遊戲進行中
    if (m_GameState == GameState::PLAYING) {

        m_StageManager->Update();


        if (Util::Input::IsKeyPressed(Util::Keycode::LEFT))  m_Player->Move(-1, 0);
        if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) m_Player->Move(1, 0);

        //按Z或X發射子彈
        m_ShootCooldown -= Util::Time::GetDeltaTimeMs();
        if ((Util::Input::IsKeyDown(Util::Keycode::Z) && m_ShootCooldown <= 0.0f)
            || (Util::Input::IsKeyDown(Util::Keycode::X) && m_ShootCooldown <= 0.0f)){
            //最多兩顆子彈
            if (Player_bullet::getBulletcount()<2){
                Player_bullet::setBulletcount(1);
                auto bullet = std::make_shared<Player_bullet>(m_Player->GetPosition());
                m_Bullets.push_back(bullet);
                m_Root.AddChild(bullet);
                m_ShootCooldown = 0.5f; // 0.0005秒冷卻
            }
        }
        // 更新子彈位置
        for (auto& bullet : m_Bullets) {
            bullet->flyUp();
        }

        // 更新爆炸動畫
        for (auto& exp : m_Explosions) {
            exp->Update();
        }
        m_Explosions.erase(
            std::remove_if(m_Explosions.begin(), m_Explosions.end(),
                [](const auto& e) { return e->IsFinished(); }),
            m_Explosions.end()
        );

        //子彈超出螢幕
        Player_bullet::Remove(m_Bullets, [&](const auto& b) {
            if (b->IsOutOfScreen()) {
                m_Root.RemoveChild(b);
                return true;
            }
            return false;
        });
        //子彈打到敵人
        Player_bullet::Remove(m_Bullets, [&](const std::shared_ptr<Player_bullet>& b) {
            for (auto& enemy : m_Enemies) {
                if (!enemy->IsAlive()) continue;
                if (enemy->IfCollides(b->GetPosition(), 25.0f)) {
                    enemy->TakeDamage(1);       //敵人扣血

                    if (!enemy->IsAlive()) {
                    auto explosion = std::make_shared<Explosion>(enemy->GetPosition());
                    m_Explosions.push_back(explosion);
                    m_Root.AddChild(explosion);
    }

                    m_Root.RemoveChild(b);      //移除子彈
                    m_Score += enemy->GetScore(); // 加分

                    // 更新分數顯示
                    m_ScoreLabel->SetText("SCORE\n" + std::to_string(m_Score));
                    return true;
                }
            }
            return false;
        }
        );

        // 碰撞偵測
        /*for (auto& bullet : m_Bullets) {
            for (auto& enemy : m_Enemies) {
                if (!enemy->IsAlive()) continue;
                if (enemy->IfCollides(bullet->GetPosition(), 25.0f)) {
                    //enemy->Kill();              // 敵人消失
                    bullet->SetVisible(false);  // 子彈消失
                    //Player_bullet::setBulletcount(-1); // 子彈數量-1
                    m_Score += enemy->GetScore(); // 加分

                    // 更新分數顯示
                    m_ScoreLabel->SetText("SCORE\n" + std::to_string(m_Score));
                }
            }
        }*/

        // 移除被嘎嘎敵人和嘎嘎子彈
        m_Enemies.erase(
            std::remove_if(m_Enemies.begin(), m_Enemies.end(),
                [](const auto& e) { return !e->IsAlive(); }),
            m_Enemies.end()
        );
        m_Bullets.erase(
            std::remove_if(m_Bullets.begin(), m_Bullets.end(),
                [](const auto& b) {
                    if (!b->GetVisibility() || b->IsOutOfScreen()) {
                        Player_bullet::setBulletcount(-1);
                        return true;
                    }
                    return false;
                }),
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