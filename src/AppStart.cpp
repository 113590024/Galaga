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

    // 字幕：2 PLAYER
    m_Text2P = std::make_shared<Label>(
        RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 20, "2 PLAYERS",
        Util::Color::FromRGB(255, 255, 255), 10
    );
    m_Text2P->SetPosition({0.0f, -90.0f});
    m_Root.AddChild(m_Text2P);

    // 玩家(剛開始時消失)
    m_Player = std::make_shared<PLAYER>(RESOURCE_DIR"/Image/Character/player_1.png");
    m_Player->SetVisible(false);
    m_Root.AddChild(m_Player);

    // 箭頭
    m_Cursor = std::make_shared<Cursor>();
    m_Cursor->SetPosition({-130.0f, -50.0f}); // 對齊 1 PLAYER 左邊
    m_Root.AddChild(m_Cursor);

    // 暫停文字(預設隱藏)
    m_PauseText = std::make_shared<Label>(
    RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 30, "PAUSED",
    Util::Color::FromRGB(255, 255, 0), 20 // 黃色，最上層
    );
    m_PauseText->SetPosition({0.0f, 0.0f});
    m_PauseText->SetVisible(false);
    m_Root.AddChild(m_PauseText);

    //分數、生命值
    m_ScoreLabel = std::make_shared<Label>(
    RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 16, "SCORE",
    Util::Color::FromRGB(255, 255, 0), 20
);
    m_ScoreLabel->SetPosition({480.0f, 200.0f});
    m_Root.AddChild(m_ScoreLabel);

    m_LivesLabel = std::make_shared<Label>(
        RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 16, "LIVES: 3",
        Util::Color::FromRGB(255, 255, 255), 20
    );
    m_LivesLabel->SetPosition({480.0f, 100.0f});
    m_Root.AddChild(m_LivesLabel);
    m_ScoreLabel->SetVisible(false);
    m_LivesLabel->SetVisible(false);

    // 測試用 生成敵人 剛開始時消失
    for (int i = 0; i < 5; i++) {
        auto zako = std::make_shared<Zako>(
            glm::vec2(-400.0f + i * 80.0f, 200.0f)
        );
        zako->SetVisible(false);
        m_Enemies.push_back(zako);
        m_Root.AddChild(zako);
    }

    m_CurrentState = State::UPDATE;
}