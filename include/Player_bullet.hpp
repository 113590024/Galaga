//
// Created by TUF on 2026/3/20.
//

#ifndef Player_bullet_hpp
#define Player_bullet_hpp

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Player_bullet : public Util::GameObject {
public:
    explicit Player_bullet(const glm::vec2& startPos) {
        m_Drawable = std::make_unique<Util::Image>(
            RESOURCE_DIR"/Image/Character/bullet_player.png");
        m_Transform.translation = startPos;
        m_Transform.scale = {1.0f, 1.0f};
        m_ZIndex = 20;
        m_Visible = true;
    }
    void flyUp() {
        m_Transform.translation.y+=m_speed;
    }
    [[nodiscard]] glm::vec2 GetPosition() const {
        return m_Transform.translation;
    }
    [[nodiscard]] bool IsOutOfScreen() const {
        return m_Transform.translation.y>400.0f;
    }
private:
    float m_speed=20.0f;
};
#endif //GALAGA_PLAYER_BULLET_HPP