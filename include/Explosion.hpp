#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"

class Explosion : public Util::GameObject {
public:
    explicit Explosion(const glm::vec2& pos) {
        std::vector<std::string> frames = {
            RESOURCE_DIR"/Image/Character/explosion_enemy_1.png",
            RESOURCE_DIR"/Image/Character/explosion_enemy_2.png",
            RESOURCE_DIR"/Image/Character/explosion_enemy_3.png",
            RESOURCE_DIR"/Image/Character/explosion_enemy_4.png",
        };
        m_Drawable = std::make_shared<Util::Animation>(
            frames, false, 80, false, 0
        );
        m_Transform.translation = pos;
        m_Transform.scale = {0.7f, 0.7f};
        m_ZIndex = 30;
        m_Visible = true;

        std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->Play();
    }

    void Update() {
        auto anim = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if (anim->GetCurrentFrameIndex() == anim->GetFrameCount() - 1) {
            m_Finished = true;
            m_Visible = false;
        }
    }

    [[nodiscard]] bool IsFinished() const { return m_Finished; }

private:
    bool m_Finished = false;
};

#endif