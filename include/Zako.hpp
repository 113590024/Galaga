#ifndef ZAKO_HPP
#define ZAKO_HPP

#include "Enemy.hpp"

class Zako : public Enemy {
public:
    explicit Zako(const glm::vec2& startPos)
        : Enemy(RESOURCE_DIR"/Image/Character/enemy_zako1.png", 10) {
        m_Transform.translation = startPos;
        m_Transform.scale = {1.0f, 1.0f};
        m_health=1;
        m_Score = 50;
    }

    void Update() override {
        switch (m_State) {
            case State::ENTERING:
                UpdatePath();
                break;
            case State::FORMATION:
                // 之後做編隊移動
                break;
            case State::DIVING:
                UpdatePath();
                break;
            case State::RETURNING:
                // 之後做
                break;
        }
    }

    void StartDive(const glm::vec2& playerPos) override {
        m_FormationPos = m_Transform.translation;
        m_State = State::DIVING;
        // 之後設定俯衝路徑
    }

private:
    //
};

#endif