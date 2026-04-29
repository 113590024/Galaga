#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"
#include "Enemy_bullet.hpp"
#include "Stage0_0.hpp"

//測試用
#include "Util/Logger.hpp"

void App::Update() {
    // 背景動畫 (在不是暫停時)
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
            m_Cursor->SetVisible(false);
        }

        // 動畫結束
        if (!m_IntroPlaying && !m_ShowingStart) {
            m_Cursor->SetVisible(true);

            if (Util::Input::IsKeyUp(Util::Keycode::UP)) {
                m_MenuIndex = 0;
                m_Cursor->SetPosition({-80.0f, -50.0f});
            }
            if (m_MenuIndex == 0){
                if (Util::Input::IsKeyUp(Util::Keycode::RETURN)){
                    m_Logo->SetVisible(false);
                    m_Text1P->SetVisible(false);
                    m_Cursor->SetVisible(false);
                    m_StageText->SetVisible(true);
                    m_HighScoreLabel->SetVisible(true);
                    m_HighScoreLabel->SetText("HIGH\n SCORE\n " + std::to_string(m_HighScore));
                    m_ScoreLabel->SetVisible(true);
                    m_ScoreLabel->SetText("SCORE\n 0");
                    m_Lives = m_Player->GetHP();
                    UpdateLifeIcons();
                    m_StartMusic->Play();
                    m_StartText->SetVisible(true);
                    m_ShowingStart = true;
                }
            }
        }

        // START
        if (m_ShowingStart && !m_ShowingReady && !m_ShowingStage) {
            m_StartTimer += Util::Time::GetDeltaTimeMs();
            if (m_StartTimer >= 7000.0f) {
                m_StageSound->Play();
                m_StartText->SetVisible(false);
                m_Stage1Text->SetVisible(true);
                m_ShowingStage = true;
            }
        }

        // 關卡
        if (m_ShowingStage && !m_ShowingReady){
            m_StageTimer += Util::Time::GetDeltaTimeMs();
            if (m_StageTimer >= 2000.0f) {
                m_Stage1Text->SetVisible(false);
                m_ReadyText->SetVisible(true);
                m_ShowingReady = true;
            }
            totalEnemies=0;
        }

        // READY
        if (m_ShowingReady) {
            m_ReadyTimer += Util::Time::GetDeltaTimeMs();
            if (m_ReadyTimer >= 2000.0f) {
                m_ReadyText->SetVisible(false);
                m_Player->SetVisible(true);
                m_GameState = GameState::PLAYING;
                // 顯示敵人
                for (auto& enemy : m_Enemies) {
                    enemy->SetVisible(true);
                }
            }
        }
    }

    // 遊戲進行中
    if (m_GameState == GameState::PLAYING) {
        m_Player->Update();

        // 關卡 (目前一關)
        //m_StageManager->Update();
        m_Stage0_0->Update(m_Enemies, m_Root);

        // 更新所有敵人
        for (auto& enemy : m_Enemies) {
            enemy->Update();
        }

        if (Util::Input::IsKeyPressed(Util::Keycode::LEFT))  m_Player->Move(-1, 0);
        if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) m_Player->Move(1, 0);

        // 按Z或X發射子彈
        m_ShootCooldown -= Util::Time::GetDeltaTimeMs();
        if ((Util::Input::IsKeyDown(Util::Keycode::Z) && m_ShootCooldown <= 0.0f)
            || (Util::Input::IsKeyDown(Util::Keycode::X) && m_ShootCooldown <= 0.0f)){
            // 最多兩顆子彈
            if (Player_bullet::getBulletcount()<2){
                Player_bullet::setBulletcount(1);
                auto bullet = std::make_shared<Player_bullet>(m_Player->GetPosition());
                m_Bullets.push_back(bullet);
                m_Root.AddChild(bullet);
                m_ShootCooldown = 0.5f; // 0.0005秒冷卻
                m_ShootSound->Play();   // 射擊音效
                m_ShotsFired++;     // Shot計數
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

        // 子彈超出螢幕
        Player_bullet::Remove(m_Bullets, [&](const auto& b) {
            if (b->IsOutOfScreen()) {
                m_Root.RemoveChild(b);
                return true;
            }
            return false;
        });
        // 子彈打到敵人
        Player_bullet::Remove(m_Bullets, [&](const std::shared_ptr<Player_bullet>& b) {
            for (auto& enemy : m_Enemies) {
                if (!enemy->IsAlive()) continue;
                if (enemy->IfCollides(b->GetPosition(), 25.0f)) {
                    enemy->TakeDamage(1);       // 敵人扣血
                    m_EnemyExplodeSound->Play();
                    m_Hits++;
                    if (!enemy->IsAlive()) {
                        auto explosion = std::make_shared<Explosion>(enemy->GetPosition());
                        m_Explosions.push_back(explosion);
                        m_Root.AddChild(explosion);
                        m_Score += enemy->GetScore(); // 加分
                        totalEnemies+=1;
                    }
                    m_Root.RemoveChild(b);      // 移除子彈

                    // 更新分數顯示
                    if (m_HighScore < m_Score){
                        m_HighScoreLabel->SetText("HIGH\n SCORE\n " + std::to_string(m_Score));
                    }
                    m_ScoreLabel->SetText("SCORE\n" + std::to_string(m_Score));
                    return true;
                }
            }
            return false;
        }
        );

        // 敵人發射
        for (auto& enemy : m_Enemies) {
            enemy->shoot = [&](const glm::vec2& pos) {
                auto bullet = std::make_shared<Enemy_bullet>(pos);
                m_EnemyBullets.push_back(bullet);
                m_Root.AddChild(bullet);
            };
        }
        for (auto& bullet : m_EnemyBullets) {
            bullet->flyDown();
        }

        // 子彈打到玩家
        Enemy_bullet::Remove(m_EnemyBullets, [&](const std::shared_ptr<Enemy_bullet>& b) {
            if (!m_Player->IsAlive()) return false;
            if (m_Player->IfCollides(b->GetPosition(), 25.0f)) {
                // 玩家爆炸動畫
                auto expPlayer = std::make_shared<Explosion>(
                    m_Player->GetPosition(), Explosion::Type::PLAYER);
                m_Explosions.push_back(expPlayer);
                m_Root.AddChild(expPlayer);

                m_Player->TakeDamage();
                m_Lives = m_Player->GetHP();
                UpdateLifeIcons();
                m_Player->SetVisible(false);
                m_Root.RemoveChild(b);

                if (m_Player->IsDead()) {
                    m_GameState = GameState::GAME_OVER;
                    m_GameOverText->SetVisible(true);
                    m_GameOverTimer = 3000.0f;
                } else {
                    m_GameState = GameState::PLAYER_DEAD;
                    m_PlayerDeathTimer = 5000.0f;
                }
                return true;
            }
            return false;
        }
        );

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

        // 敵人碰到玩家
        for (auto& enemy : m_Enemies) {
            if (!enemy->IsAlive()) continue;
            if (enemy->IfCollides(m_Player->GetPosition(), 30.0f)) {

                // 玩家爆炸動畫
                auto expPlayer = std::make_shared<Explosion>(m_Player->GetPosition(), Explosion::Type::PLAYER);
                m_Explosions.push_back(expPlayer);
                m_Root.AddChild(expPlayer);

                m_Player->TakeDamage();
                m_PlayerExplodeSound->Play();
                m_Lives = m_Player->GetHP();
                UpdateLifeIcons();
                m_Player->SetVisible(false);

                enemy->TakeDamage(2);
                totalEnemies+=1;

                m_Score += enemy->GetScore(); // 加分

                // 更新分數顯示
                if (m_HighScore < m_Score){
                    m_HighScoreLabel->SetText("HIGH\n SCORE\n " + std::to_string(m_Score));
                }
                m_ScoreLabel->SetText("SCORE\n" + std::to_string(m_Score));

                if (m_Player->IsDead()) {
                    m_GameState = GameState::GAME_OVER;
                    m_GameOverText->SetVisible(true);
                    m_GameOverTimer = 3000.0f;
                } else {
                    m_GameState = GameState::PLAYER_DEAD;
                    m_PlayerDeathTimer = 5000.0f;
                }
                break;
            }
        }

        if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
            m_GameState = GameState::PAUSED;
            m_PauseText->SetVisible(true);
        }

        // 消滅所有敵人
        if (totalEnemies>=m_Stage0_0->TotalEnemyCount() && m_Player->IsAlive()){
            m_GameState = GameState::CLEAR;
            m_ClearText->SetVisible(true);
            m_ClearTimer = 3000.0f;
        }
    }

    // 暫停中
    if (m_GameState == GameState::PAUSED) {
        if (Util::Input::IsKeyUp(Util::Keycode::RETURN)) {
            m_GameState = GameState::PLAYING;
            m_PauseText->SetVisible(false);
        }
    }

    // 玩家受傷
    if (m_GameState == GameState::PLAYER_DEAD) {
        m_Player->Update();
        m_PlayerDeathTimer -= Util::Time::GetDeltaTimeMs();

        for (auto& enemy : m_Enemies) {
            enemy->Playerdead();    // 敵人不俯衝
        }

        // 爆炸動畫繼續爆
        for (auto& exp : m_Explosions) {
            exp->Update();
        }
        // 玩家子彈繼續飛
        for (auto& bullet : m_Bullets) {
            bullet->flyUp();
        }

        // 敵人子彈繼續飛
        for (auto& bullet : m_EnemyBullets) {
            bullet->flyDown();
        }

        m_Explosions.erase(
            std::remove_if(m_Explosions.begin(), m_Explosions.end(),
                [](const auto& e) { return e->IsFinished(); }),
            m_Explosions.end()
        );

        // 敵人繼續動但玩家不能動
        for (auto& enemy : m_Enemies) {
            enemy->Update();
        }

        if (m_PlayerDeathTimer >= 3000.0f){
            m_ReadyText->SetVisible(false);
        }

        if (m_PlayerDeathTimer >= 0.0f && m_PlayerDeathTimer <= 3000.0f){
            m_ReadyText->SetVisible(true);
        }

        if (m_PlayerDeathTimer <= 0.0f) {
            // RE:0
            m_ReadyText->SetVisible(false);
            m_Player->SetVisible(true);
            m_Player->ResetPosition();
            m_GameState = GameState::PLAYING;

            for (auto& enemy : m_Enemies) {
                enemy->Playernodead();  // 敵人可以俯衝
            }
        }
    }

    // 結算
    if (m_GameState == GameState::GAME_OVER){
        m_GameOverTimer -= Util::Time::GetDeltaTimeMs();

        // 玩家子彈繼續飛
        for (auto& bullet : m_Bullets) {
            bullet->flyUp();
        }

        // 敵人子彈繼續飛
        for (auto& bullet : m_EnemyBullets) {
            bullet->flyDown();
        }

        // 爆炸動畫繼續爆
        for (auto& exp : m_Explosions) {
            exp->Update();
        }
        m_Explosions.erase(
            std::remove_if(m_Explosions.begin(), m_Explosions.end(),
                [](const auto& e) { return e->IsFinished(); }),
            m_Explosions.end()
        );

        if (m_GameOverTimer <= 0.0f) {
            m_GameOverText->SetVisible(false);
            m_Player->SetVisible(false);
            // 清除所有敵人
            for (auto& enemy : m_Enemies) {
                m_Root.RemoveChild(enemy);
            }
            m_Enemies.clear();

            // 計算比例
            float ratio = (m_ShotsFired > 0) ? (static_cast<float>(m_Hits) / m_ShotsFired) * 100.0f : 0.0f;

            // Result文字
            m_ResultShotsText->SetText("SHOTS FIRED:        " + std::to_string(m_ShotsFired));
            m_ResultHitsText->SetText("NUMBER OF HITS:      " + std::to_string(m_Hits));
            m_ResultRatioText->SetText("HIT-MISS RATIO:     " + std::to_string(static_cast<int>(ratio)) + "%");

            // 顯示文字
            m_ResultText->SetVisible(true);
            m_ResultShotsText->SetVisible(true);
            m_ResultHitsText->SetVisible(true);
            m_ResultRatioText->SetVisible(true);

            m_ResultTimer = 5000.0f; // 給玩家 5 秒看分數
            m_GameState = GameState::RESULT; // 跳轉狀態
        }
    }

    if (m_GameState == GameState::CLEAR){
        m_ClearTimer -= Util::Time::GetDeltaTimeMs();

        // 玩家子彈繼續飛
        for (auto& bullet : m_Bullets) {
            bullet->flyUp();
        }

        // 敵人子彈繼續飛
        for (auto& bullet : m_EnemyBullets) {
            bullet->flyDown();
        }

        // 爆炸動畫繼續爆
        for (auto& exp : m_Explosions) {
            exp->Update();
        }
        m_Explosions.erase(
            std::remove_if(m_Explosions.begin(), m_Explosions.end(),
                [](const auto& e) { return e->IsFinished(); }),
            m_Explosions.end()
        );

        if (m_ClearTimer <= 0.0f) {
            m_ClearText->SetVisible(false);
            m_Player->SetVisible(false);

            // 計算比例
            float ratio = (m_ShotsFired > 0) ? (static_cast<float>(m_Hits) / m_ShotsFired) * 100.0f : 0.0f;

            // Result文字
            m_ResultShotsText->SetText("SHOTS FIRED:        " + std::to_string(m_ShotsFired));
            m_ResultHitsText->SetText("NUMBER OF HITS:      " + std::to_string(m_Hits));
            m_ResultRatioText->SetText("HIT-MISS RATIO:     " + std::to_string(static_cast<int>(ratio)) + "%");

            // 顯示文字
            m_ResultText->SetVisible(true);
            m_ResultShotsText->SetVisible(true);
            m_ResultHitsText->SetVisible(true);
            m_ResultRatioText->SetVisible(true);

            m_ResultTimer = 5000.0f; // 給玩家 5 秒看分數
            m_GameState = GameState::RESULT; // 跳轉狀態
        }
    }

    if (m_GameState == GameState::RESULT) {
        m_ResultTimer -= Util::Time::GetDeltaTimeMs();
        // 按 Enter 直接跳過等待時間
        if (m_ResultTimer <= 0.0f || Util::Input::IsKeyUp(Util::Keycode::RETURN)) {
            // 隱藏 Result 文字
            m_ResultText->SetVisible(false);
            m_ResultShotsText->SetVisible(false);
            m_ResultHitsText->SetVisible(false);
            m_ResultRatioText->SetVisible(false);

            // 重置數據
            m_ShotsFired = 0;
            m_Hits = 0;

            // 回到大廳：重置所有狀態
            m_Player->SetVisible(false);

            // 紀錄最高分數
            if (m_HighScore < m_Score){
                m_HighScore = m_Score;
            }
            m_HighScoreLabel->SetVisible(false);

            // 重置分數、生命
            m_Score = 0;
            m_ScoreLabel->SetText("SCORE\n 0");
            m_ScoreLabel->SetVisible(false);
            m_Player->ResetHP();
            m_Lives = m_Player->GetHP();
            // 隱藏玩家生命值圖片
            for (auto& icon : m_LifeIcons) {
                icon->SetVisible(false);
            }

            // 右下角關卡隱藏
            m_StageText->SetVisible(false);

            // 重置開場動畫狀態
            m_ShowingStart = false;
            m_ShowingStage = false;
            m_ShowingReady = false;
            m_StartTimer = 0.0f;
            m_StageTimer = 0.0f;
            m_ReadyTimer = 0.0f;
            m_IntroPlaying = true;
            m_IntroY = -500.0f;

            // 顯示選單
            m_Logo->SetVisible(true);
            m_Text1P->SetVisible(true);

            // 重建關卡
            m_Stage0_0 = std::make_unique<Stage0_0>();

            m_GameState = GameState::START_SCREEN;
        }
    }
    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    m_Root.Update();
}