//
// Created by TUF on 2026/4/5.
//

#ifndef GALAGA_BOSS_GALAGA_HPP
#define GALAGA_BOSS_GALAGA_HPP

#include "Enemy.hpp"

class Boss_Galaga:public Enemy {
public:
    Boss_Galaga(const glm::vec2& startPos,
         const glm::vec2& formationPos,
         const std::vector<BezierPath>& entryPath)
        : Enemy("", 10) {

        // 待機動畫
        m_IdleFrames = {
            RESOURCE_DIR"/Image/Character/enemy_zako1.png",
            RESOURCE_DIR"/Image/Character/enemy_zako2.png",
        };
        // 俯衝動畫
        m_DiveFrames = {
            RESOURCE_DIR"/Image/Character/enemy_zako3.png",
            RESOURCE_DIR"/Image/Character/enemy_zako4.png",
            RESOURCE_DIR"/Image/Character/enemy_zako5.png",
            RESOURCE_DIR"/Image/Character/enemy_zako6.png",
            RESOURCE_DIR"/Image/Character/enemy_zako7.png",
            RESOURCE_DIR"/Image/Character/enemy_zako8.png",
        };

        //setAnimation(m_IdleFrames);

        m_Transform.translation = startPos;
        m_Transform.scale = {0.9f, 0.9f};
        m_health = 1;
        m_Score = 100;
        m_FormationPos = formationPos;
        SetPath(entryPath);
    }

    void Update() override {
        m_PrevPosition = m_Transform.translation; // 記錄上一幀位置

        switch (m_State) {
            case State::ENTERING:
                UpdatePath();
                //updateDirectionAnimation(); // 根據移動方向更新動畫
                // 路徑走完後切換到待機動畫
                if (m_State == State::FORMATION) {
                    //setAnimation(m_IdleFrames);
                }
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
                //updateDirectionAnimation();
                if (m_State == State::FORMATION) {
                    //setAnimation(m_IdleFrames); // 返回後換回待機
                }
                break;
            case State::RETURNING:
                UpdatePath();
                //updateDirectionAnimation();
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
        //setAnimation(m_DiveFrames);

        glm::vec2 start = m_Transform.translation;

        // 俯衝路徑：從編隊位置飛向玩家，再飛出畫面
        std::vector<Enemy::BezierPath> divePath = {
            { { start,
                {start.x, start.y - 100.0f},   // 先往下
                {start.x-50.0f,  + 100.0f},
                {start.x-50.0f, -400.0f} } },            // 飛出畫面下方

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
    glm::vec2 m_PrevPosition = {0.0f, 0.0f};
    float m_DiveTimer = 5000.0f;  // 5秒

    void updateFormation() {
        m_Transform.translation = {
            m_FormationPos.x + m_FormationOffsetX,
            m_FormationPos.y
        };
    }
};

#endif //GALAGA_BOSS_GALAGA_HPP