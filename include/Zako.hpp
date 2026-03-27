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
        m_Score = 100;
    }

    void Update() override {
        // 移動...之後再做
    }

    [[nodiscard]] int GetScore() const { return m_Score; }
};

#endif