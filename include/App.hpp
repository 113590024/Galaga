#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"
#include "Util/Renderer.hpp"
#include "Util/GameObject.hpp"
#include "PLAYER.hpp"
#include "Sprite.hpp"
#include "Label.hpp"
#include "Cursor.hpp"
#include "Player_bullet.hpp"
#include "Zako.hpp"
#include "Explosion.hpp"
#include "Stage0_0.hpp"

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
        PLAYER_DEAD,
        PAUSED,
        GAME_OVER,
        RESULT
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
    bool m_IntroPlaying = true;   // 開場動畫進行中
    float m_IntroY = -500.0f;     // 從畫面底部開始
    float m_IntroTargetY = 0.0f;  // 最終停止的 Y 位置

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

    // 分數、生命值
    std::shared_ptr<Label> m_ScoreLabel;
    std::shared_ptr<Label> m_LivesLabel;
    int m_Score = 0;
    int m_Lives = 3;

    // 敵人
    std::vector<std::shared_ptr<Enemy>> m_Enemies;

    // Explosion!!!!!!!!!!!!!!!!!!!!!!!!!!!
    std::vector<std::shared_ptr<Explosion>> m_Explosions;

    // 關卡 (第幾關)
    std::shared_ptr<Stage0_0> m_Stage0_0;

    //GAMEOVER
    std::shared_ptr<Label> m_GameOverText;
    float m_PlayerDeathTimer = 0.0f;
    float m_GameOverTimer = 0.0f;
    float m_ResultTimer = 0.0f;

    //Result計數
    int m_ShotsFired = 0;   // 總射擊次數
    int m_Hits = 0;         // 命中次數

    // Result畫面的文字物件
    std::shared_ptr<Label> m_ResultText;
    std::shared_ptr<Label> m_ResultShotsText;
    std::shared_ptr<Label> m_ResultHitsText;
    std::shared_ptr<Label> m_ResultRatioText;
};

#endif