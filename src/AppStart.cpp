#include "App.hpp"
#include "Util/Logger.hpp"
#include "Util/Color.hpp"
#include "Sprite.hpp"
#include "Label.hpp"
#include "Player_bullet.hpp"

void App::Start() {
    LOG_TRACE("Start");

    // 背景
    m_BgPaths = {
        RESOURCE_DIR"/Image/Background/wall_1.png",
        RESOURCE_DIR"/Image/Background/wall_2.png",
        RESOURCE_DIR"/Image/Background/wall_3.png",
    };
    m_Bg1 = std::make_shared<Sprite>(m_BgPaths[0], -10);
    m_Bg1->SetScale({1.5f, 1.5f}); // 調整成只蓋左邊
    m_Bg1->SetPosition({-160.0f, 0.0f}); // 往左偏移
    m_Root.AddChild(m_Bg1);

    m_Bg2 = std::make_shared<Sprite>(m_BgPaths[0], -10);
    m_Bg2->SetScale({1.5f, 1.5f});
    m_Bg2->SetPosition({-160.0f, 720.0f});
    m_Root.AddChild(m_Bg2);

    // Galaga圖片
    m_Logo = std::make_shared<Sprite>(RESOURCE_DIR"/Image/Galaga.png", 10);
    m_Logo->SetScale({0.5f, 0.5f});
    m_Logo->SetPosition({0.0f, 80.0f});
    m_Root.AddChild(m_Logo);

    // 字幕：1 PLAYER
    m_Text1P = std::make_shared<Label>(
        RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 20, "1 PLAYER ",
        Util::Color::FromRGB(255, 255, 255), 10
    );
    m_Text1P->SetPosition({0.0f, -50.0f});
    m_Root.AddChild(m_Text1P);

    // 玩家(剛開始時消失)
    m_Player = std::make_shared<PLAYER>(RESOURCE_DIR"/Image/Character/player_1.png");
    m_Player->SetVisible(false);
    m_Root.AddChild(m_Player);

    // 箭頭
    m_Cursor = std::make_shared<Cursor>();
    m_Cursor->SetPosition({-130.0f, -50.0f}); // 對齊 1 PLAYER 左邊
    m_Root.AddChild(m_Cursor);

    // START (剛開始)
    m_StartText = std::make_shared<Label>(
    RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 30, "START",
    Util::Color::FromRGB(255, 120, 0), 20);
    m_StartText->SetPosition({-150.0f, 0.0f});
    m_StartText->SetVisible(false);
    m_Root.AddChild(m_StartText);

    // 暫停文字
    m_PauseText = std::make_shared<Label>(
    RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 30, "PAUSED",
    Util::Color::FromRGB(255, 255, 0), 20 // 黃色，最上層
    );
    m_PauseText->SetPosition({-150.0f, 0.0f});
    m_PauseText->SetVisible(false);
    m_Root.AddChild(m_PauseText);

    // 關卡文字
    m_Stage1Text = std::make_shared<Label>(
    RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 30, "Stage    1",
    Util::Color::FromRGB(0, 0, 255), 20 // 黃色，最上層
    );
    m_Stage1Text->SetPosition({-150.0f, 0.0f});
    m_Stage1Text->SetVisible(false);
    m_Root.AddChild(m_Stage1Text);

    // READY
    m_ReadyText = std::make_shared<Label>(
    RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 30, "READY",
    Util::Color::FromRGB(255, 120, 0), 20);
    m_ReadyText->SetPosition({-150.0f, 0.0f});
    m_ReadyText->SetVisible(false);
    m_Root.AddChild(m_ReadyText);

    // 最高分數
    m_HighScoreLabel = std::make_shared<Label>(
    RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 25, "HIGH\n SCORE",
    Util::Color::FromRGB(255, 255, 0), 20);
    m_HighScoreLabel->SetPosition({490.0f, 300.0f});
    m_Root.AddChild(m_HighScoreLabel);
    m_HighScoreLabel->SetVisible(false);

    //分數
    m_ScoreLabel = std::make_shared<Label>(
    RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 25, "SCORE",
    Util::Color::FromRGB(255, 255, 0), 20);
    m_ScoreLabel->SetPosition({480.0f, 100.0f});
    m_Root.AddChild(m_ScoreLabel);
    m_ScoreLabel->SetVisible(false);

    // 生命值圖片
    for (int i = 0; i < 3; i++) {
        auto icon = std::make_shared<Sprite>(
            RESOURCE_DIR"/Image/Character/player_1.png", 20
        );
        icon->SetScale({0.7f, 0.7f});
        // 右側，由左到右排列
        icon->SetPosition({430.0f + i * 40.0f, -100.0f});
        icon->SetVisible(false);
        m_LifeIcons.push_back(icon);
        m_Root.AddChild(icon);
    }

    //生人敵人
    m_Stage0_0 = std::make_shared<Stage0_0>();

    // GAMEOVER文字
    m_GameOverText = std::make_shared<Label>(
    RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 40, "GAME OVER",
    Util::Color::FromRGB(255, 0, 0), 50);
    m_GameOverText->SetPosition({-150.0f, 0.0f});
    m_GameOverText->SetVisible(false);
    m_Root.AddChild(m_GameOverText);

    // Result文字
    m_ResultText = std::make_shared<Label>(
    RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 35, "- RESULT -",
    Util::Color::FromRGB(255, 0, 0), 50);
    m_ResultText->SetPosition({-150.0f, 80.0f});
    m_ResultText->SetVisible(false);
    m_Root.AddChild(m_ResultText);
    m_ResultShotsText = std::make_shared<Label>(
    RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 35, "SHOTS FIRED:        ",
    Util::Color::FromRGB(0, 0, 255), 50);
    m_ResultShotsText->SetPosition({-150.0f, 20.0f});
    m_ResultShotsText->SetVisible(false);
    m_Root.AddChild(m_ResultShotsText);
    m_ResultHitsText = std::make_shared<Label>(
    RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 35, "NUMBER OF HITS:     ",
    Util::Color::FromRGB(255, 255, 255), 50);
    m_ResultHitsText->SetPosition({-150.0, -40.0f});
    m_ResultHitsText->SetVisible(false);
    m_Root.AddChild(m_ResultHitsText);
    m_ResultRatioText = std::make_shared<Label>(
    RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 35, "HIT-MISS RATIO:     ",
    Util::Color::FromRGB(255, 255, 0), 50);
    m_ResultRatioText->SetPosition({-150.0f, -100.0f});
    m_ResultRatioText->SetVisible(false);
    m_Root.AddChild(m_ResultRatioText);

    // 音效
    m_StartMusic = std::make_shared<Util::SFX>(RESOURCE_DIR"/sound/GameStart.mp3");
    m_StageSound = std::make_shared<Util::SFX>(RESOURCE_DIR"/sound/StageFlag.mp3");
    m_ShootSound = std::make_shared<Util::SFX>(RESOURCE_DIR"/sound/Shooting.mp3");
    m_EnemyExplodeSound = std::make_shared<Util::SFX>(RESOURCE_DIR"/sound/Enemy1Kill.mp3");
    m_PlayerExplodeSound = std::make_shared<Util::SFX>(RESOURCE_DIR"/sound/Death.mp3");
    m_StartMusic->SetVolume(32);
    m_StageSound->SetVolume(32);
    m_ShootSound->SetVolume(32);
    m_EnemyExplodeSound->SetVolume(32);
    m_PlayerExplodeSound->SetVolume(32);

    m_CurrentState = State::UPDATE;
}