//
// Created by TUF on 2026/4/5.
//

#ifndef GALAGA_BUTTERFLY_HPP
#define GALAGA_BUTTERFLY_HPP

#include "Enemy.hpp"

class Butterfly:public Enemy {
public:
    explicit Butterfly(const glm::vec2& startPos)
        :Enemy(RESOURCE_DIR"/Image/Character/enemy_Butterfly1.png",10) {
        m_Transform.translation=startPos;
        m_Transform.scale={1.0f,1.0f};
        m_health=1;
        m_Score=80;
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

#endif //GALAGA_BUTTERFLY_HPP