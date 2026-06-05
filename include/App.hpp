#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"
#include "Util/Renderer.hpp"
#include "Util/GameObject.hpp"
#include "Util/SFX.hpp"
#include "PLAYER.hpp"
#include "Sprite.hpp"
#include "Label.hpp"
#include "Cursor.hpp"
#include "Player_bullet.hpp"
#include "Enemy_bullet.hpp"
#include "Explosion.hpp"
#include "Stage.hpp"

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
        PLAYER_CAPTURED,
        PLAYER_DEAD,
        PAUSED,
        GAME_OVER,
        CLEAR,
        STAGE2_RESULT,
        RESULT
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void End();

    // 更新玩家生命值圖片
    void UpdateLifeIcons() {
        if (m_Lives <= 5) {
            for (int i = 0; i < (int)m_LifeIcons.size(); i++) {
                m_LifeIcons[i]->SetVisible(i < m_Lives);
                m_LifeIcons[i]->SetPosition({430.0f + i * 40.0f, -100.0f});
            }
            m_LifeCountLabel->SetVisible(false);
        } else {
            for (int i = 0; i < (int)m_LifeIcons.size(); i++) {
                m_LifeIcons[i]->SetVisible(i == 0);
            }

            m_LifeIcons[0]->SetPosition({430.0f, -100.0f});
            m_LifeCountLabel->SetVisible(true);
            m_LifeCountLabel->SetText("x" + std::to_string(m_Lives));
            m_LifeCountLabel->SetPosition({500.0f, -100.0f});
        }
    }

    // 更新關卡圖案
    void UpdateStageFlagIcons() {
        int currentStageCount = m_Stages[m_Stagenumber]->getStageLevel();

        for (int i = 0; i < (int)m_StageFlagIcons.size(); i++) {
            m_StageFlagIcons[i]->SetVisible(i < currentStageCount);
        }
    }

private:
    State m_CurrentState = State::START;
    GameState m_GameState = GameState::START_SCREEN;

    Util::Renderer m_Root;

    // 玩家
    std::shared_ptr<PLAYER> m_Player;
    int m_MenuIndex = 0;

    // 封面
    std::shared_ptr<Sprite> m_Logo;
    std::shared_ptr<Label> m_Text1P;
    bool m_IntroPlaying = true;   // 開場動畫進行中
    float m_IntroY = -500.0f;     // 從畫面底部開始
    float m_IntroTargetY = 0.0f;  // 最終停止的y軸位置

    // 背景
    std::shared_ptr<Sprite> m_Bg1;
    std::shared_ptr<Sprite> m_Bg2;
    float m_BgScrollY = 0.0f;
    float m_BgSwitchTimer = 0.0f;
    int m_BgIndex = 0;
    std::vector<std::string> m_BgPaths;

    // 箭頭
    std::shared_ptr<Cursor> m_Cursor;

    // START
    std::shared_ptr<Label> m_StartText;
    bool m_ShowingStart = false;
    float m_StartTimer = 0.0f;

    // 暫停文字
    std::shared_ptr<Label> m_PauseText;

    // READY
    std::shared_ptr<Label> m_ReadyText;
    bool m_ShowingReady = false;
    float m_ReadyTimer = 0.0f;

    // 玩家子彈
    std::vector<std::shared_ptr<Player_bullet>> m_Bullets;
    float m_ShootCooldown = 0.0f; // 射擊冷卻
    float m_DeathTimer = 0.0f;

    //敵人子彈
    std::vector<std::shared_ptr<Enemy_bullet>> m_EnemyBullets;

    // 最高分數、分數、生命值
    std::shared_ptr<Label> m_ScoreLabel;
    std::shared_ptr<Label> m_HighScoreLabel;

    // 生命值圖片
    std::vector<std::shared_ptr<Sprite>> m_LifeIcons;
    int m_HighScore = 0;
    int m_Score = 0;
    int m_Lives = 3;
    std::shared_ptr<Label> m_LifeCountLabel;

    // 敵人
    std::vector<std::shared_ptr<Enemy>> m_Enemies;

    // 已消滅敵人數
    int totalEnemies = 0;

    // Explosion!!!!!!!!!!!!!!!!!!!!!!!!!!!
    std::vector<std::shared_ptr<Explosion>> m_Explosions;

    // 關卡 (第幾關)
    std::vector<std::unique_ptr<Stage>> m_Stages;
    int m_Stagenumber = 1;      //Stage1 = 0 ，Stage2 = 1
    std::shared_ptr<Label> m_Stage1Text;
    bool m_ShowingStage = false;
    float m_StageTimer = 0.0f;

    // 關卡圖案
    std::vector<std::shared_ptr<Sprite>> m_StageFlagIcons;

    // GAMEOVER
    std::shared_ptr<Label> m_GameOverText;
    float m_PlayerDeathTimer = 0.0f;
    float m_GameOverTimer = 0.0f;

    // CLEAR
    std::shared_ptr<Label> m_ClearText;
    float m_ClearTimer = 0.0f;

    // Result計數
    int m_ShotsFired = 0;   // 總射擊次數
    int m_Hits = 0;         // 命中次數
    float m_ResultTimer = 0.0f;

    // Result畫面的文字物件
    std::shared_ptr<Label> m_ResultText;
    std::shared_ptr<Label> m_ResultShotsText;
    std::shared_ptr<Label> m_ResultHitsText;
    std::shared_ptr<Label> m_ResultRatioText;

    // 音效
    std::shared_ptr<Util::SFX> m_StartMusic;
    std::shared_ptr<Util::SFX> m_StageSound;
    std::shared_ptr<Util::SFX> m_ShootSound;
    std::shared_ptr<Util::SFX> m_EnemyExplodeSound;
    std::shared_ptr<Util::SFX> m_PlayerExplodeSound;

    // Galaga光束圖片
    std::shared_ptr<Sprite> m_TractorBeam;

    // 光束抓到玩家
    bool m_PlayerCaptured = false;
    glm::vec2 m_CaptureTargetPos;     // 玩家要移動到的目標位置
    std::shared_ptr<Boss_Galaga> m_CapturingBoss; // 哪隻 Boss 在抓
    std::shared_ptr<Label> m_CapturedText;
    float m_CapturedTimer = 0.0f;
    bool m_MovingToFormation = false;  // 玩家是否在跟著 Boss 回編隊
    std::shared_ptr<Sprite> m_RedPlayer;

    //測試關卡敵人消滅數量
    std::shared_ptr<Label> m_EnermyKill;

    //獎勵關卡(關卡二)文字
    std::shared_ptr<Label> m_Stage2HitsText;
    std::shared_ptr<Label> m_PerfectText;
    float m_Stage2ResultTimer = 0.0f;
    int m_Stage2Hits = 0;
};

#endif
