//
// Created by TUF on 2026/4/5.
//

#ifndef GALAGA_BOSS_GALAGA_HPP
#define GALAGA_BOSS_GALAGA_HPP

#include "Enemy.hpp"

class Boss_Galaga:public Enemy {
public:
    explicit Boss_Galaga(const glm::vec2& startPos)
        :Enemy(RESOURCE_DIR"/Image/Character/enemy_Galaga_g1.png",10) {
        m_Transform.translation=startPos;
        m_Transform.scale={0.1f,0.1f};
        m_health=2;
        m_Score=150;
    }

    void Update() override {
        // 移動...之後再做
    }

    [[nodiscard]] int GetScore() const { return m_Score; }
};

#endif //GALAGA_BOSS_GALAGA_HPP