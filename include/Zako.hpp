#ifndef ZAKO_HPP
#define ZAKO_HPP

#include "Enemy.hpp"
#include "Util/Animation.hpp"
#include "Util/Time.hpp"

class Zako : public Enemy {
public:
    Zako(const glm::vec2& startPos,
         const glm::vec2& formationPos,
         const std::vector<BezierPath>& entryPath)
        : Enemy("", 10) {

        // 待機動畫
        m_IdleFrames = {
            RESOURCE_DIR"/Image/Character/enemy_zako1.png",
            RESOURCE_DIR"/Image/Character/enemy_zako2.png",
        };

        setAnimation(m_IdleFrames);

        m_Transform.translation = startPos;
        m_Transform.scale = {0.9f, 0.9f};
        m_health = 1;
        m_Score = 100;
        m_FormationPos = formationPos;
        SetPath(entryPath);
    }

    void Update() override {

        switch (m_State) {
            case State::ENTERING:
                UpdatePath();
                break;
            case State::FORMATION:
                updateFormation();
                if (stopdiving==false) {
                    m_DiveTimer -= Util::Time::GetDeltaTimeMs();
                    if (m_DiveTimer <= 0.0f) {
                        StartDive({0.0f, 0.0f});
                    }
                }
                else {
                    m_DiveTimer =3000.0f;
                }
                break;
            case State::DIVING:
                UpdatePath();
                break;
            case State::RETURNING:
                UpdatePath();
                break;
        }
    }

    void SetFormationOffset(float offsetX) {
        m_FormationOffsetX = offsetX;
    }

    void StartDive(const glm::vec2& playerPos) override {
        (void)playerPos;
        m_FormationPos = m_Transform.translation;
        m_State = State::DIVING;
        m_DiveTimer=3000.0f;

        glm::vec2 start = m_Transform.translation;

        // 俯衝路徑
        std::vector<Enemy::BezierPath> divePath = {
            { { start,
                {start.x, start.y - 100.0f},
                {start.x-50.0f,  + 100.0f},
                {start.x-50.0f, -400.0f} } },

            // 回到編隊位置
            { { {m_FormationPos.x, -400.0f},
                {m_FormationPos.x, -300.0f},
                {m_FormationPos.x, -300.0f},
                m_FormationPos } }
        };

        SetPath(divePath);
    }

private:
    float m_FormationOffsetX = 0.0f;
    std::vector<std::string> m_IdleFrames;
    std::vector<std::string> m_DiveFrames;
    float m_DiveTimer = 5000.0f;  // 5秒

    void updateFormation() {
        m_Transform.translation = {
            m_FormationPos.x + m_FormationOffsetX,
            m_FormationPos.y
        };
    }


    // 避免每幀重複建立 Animation，記錄目前用的 frames
    std::vector<std::string> m_CurrentFrames;

    std::vector<std::string> getCurrentAnimFrames() {
        return m_CurrentFrames;
    }

    void setAnimation(const std::vector<std::string>& frames) {
        if (m_CurrentFrames == frames) return; // 已經是這個動畫就不重建
        m_CurrentFrames = frames;
        m_Drawable = std::make_shared<Util::Animation>(
            frames, true, 120, true, 0
        );
        std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->Play();
    }
};

#endif