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
        // 移動...之後再做
    }

    [[nodiscard]] int GetScore() const { return m_Score; }
};

#endif //GALAGA_BUTTERFLY_HPP