
#ifndef GALAGA_ENEMY_BULLET_HPP
#define GALAGA_ENEMY_BULLET_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Enemy_bullet:public Util::GameObject {
public:
    explicit Player_bullet(const glm::vec2& startPos) {
        m_Drawable = std::make_unique<Util::Image>(
            RESOURCE_DIR"/Image/Character/bullet_enemy.png");
        m_Transform.translation = startPos;
        m_Transform.translation.y-=m_speed; //子彈出現位置
        m_Transform.scale = {1.0f, 1.0f};
        m_ZIndex = 20;
        m_Visible = true;
    }

    void flyUp() {
        m_Transform.translation.y-=m_speed;
    }
    [[nodiscard]] glm::vec2 GetPosition() const {
        return m_Transform.translation;
    }
    [[nodiscard]] bool IsOutOfScreen() const {
        return m_Transform.translation.y<-400.0f;
    }

    static void Remove(std::vector<std::shared_ptr<Enemy_bullet>>& bullets,
                   std::function<bool(const std::shared_ptr<Enemy_bullet>&)> condition) {
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [&condition](const auto& b) {
                    if (condition(b)) {
                        return true;
                    }
                    return false;
                }),
            bullets.end()
        );
    }

    [[nodiscard]] bool GetVisibility() const { return m_Visible; }
private:
    float m_speed=10.0f;
};

#endif //GALAGA_ENEMY_BULLET_HPP