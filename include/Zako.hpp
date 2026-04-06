#ifndef ZAKO_HPP
#define ZAKO_HPP

#include "Enemy.hpp"
#include "Util/Animation.hpp"

class Zako : public Enemy {
public:
    Zako(const glm::vec2& startPos,
         const glm::vec2& formationPos,
         const std::vector<BezierPath>& entryPath)
        : Enemy("", 10) {

        std::vector<std::string> frames = {
            RESOURCE_DIR"/Image/Character/enemy_zako1.png",
            RESOURCE_DIR"/Image/Character/enemy_zako2.png",
        };
        m_Drawable = std::make_shared<Util::Animation>(
            frames, true, 200, true, 0
        );
        std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->Play();

        m_Transform.translation = startPos;
        m_Transform.scale = {1.0f, 1.0f};
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
                break;
            case State::DIVING:
                UpdatePath();
                break;
            case State::RETURNING:
                break;
        }
    }

    void SetFormationOffset(float offsetX) {
        m_FormationOffsetX = offsetX;
    }

    void StartDive(const glm::vec2& playerPos) override {
        (void)playerPos;
        m_State = State::DIVING;
    }

private:
    float m_FormationOffsetX = 0.0f;

    void updateFormation() {
        m_Transform.translation = {
            m_FormationPos.x + m_FormationOffsetX,
            m_FormationPos.y
        };
    }
};

#endif