#ifndef GALAGA_BUTTERFLY_HPP
#define GALAGA_BUTTERFLY_HPP

#include "Enemy.hpp"
#include "Util/Animation.hpp"

class Butterfly : public Enemy {
public:
    Butterfly(const glm::vec2& startPos,
              const glm::vec2& formationPos,
              const std::vector<BezierPath>& entryPath)
        : Enemy("", 10) {

        m_IdleFrames = {
            RESOURCE_DIR"/Image/Character/enemy_Butterfly1.png",
            RESOURCE_DIR"/Image/Character/enemy_Butterfly2.png",
        };

        setAnimation(m_IdleFrames);

        m_Transform.translation = startPos;
        m_Transform.scale = {0.9f, 0.9f};
        m_health = 1;
        m_Score = 80;
        m_FormationPos = formationPos;
        SetPath(entryPath);
    }

    void Update() override {
        m_PrevPosition = m_Transform.translation;

        switch (m_State) {
            case State::ENTERING:
                UpdatePath();
                updateDirectionAnimation();
                if (m_State == State::FORMATION) {
                    setAnimation(m_IdleFrames);
                    m_Transform.rotation = 0.0f;
                }
                break;
            case State::FORMATION:
                updateFormation();
                break;
            case State::DIVING:
                UpdatePath();
                updateDirectionAnimation();
                if (IsOutOfScreen()) {
                    StartReturn();
                }
                break;
            case State::RETURNING:
                UpdatePath();
                updateDirectionAnimation();
                if (m_State == State::FORMATION) {
                    setAnimation(m_IdleFrames);
                    m_Transform.rotation = 0.0f;
                }
                break;
        }
    }

    void SetFormationOffset(float offsetX) {
        // 關鍵差別：方向相反，用 -offsetX
        m_FormationOffsetX = -offsetX;
    }

    void StartDive(const glm::vec2& playerPos) override {
        if (m_State != State::FORMATION) return;

        m_State = State::DIVING;
        setAnimation(m_DiveFrames);

        glm::vec2 currentPos = m_Transform.translation;
        std::vector<BezierPath> divePath = {
            {{ currentPos,
               {(currentPos.x + playerPos.x) / 2.0f, currentPos.y - 100.0f},
               playerPos }},
            {{ playerPos,
               {playerPos.x, playerPos.y - 150.0f},
               {playerPos.x, -500.0f} }}
        };
        SetPath(divePath);
        m_State = State::DIVING;
    }

    [[nodiscard]] bool IsOutOfScreen() const {
        return m_Transform.translation.y < -450.0f;
    }

private:
    float m_FormationOffsetX = 0.0f;
    std::vector<std::string> m_IdleFrames;
    std::vector<std::string> m_DiveFrames;
    glm::vec2 m_PrevPosition = {0.0f, 0.0f};

    void setAnimation(const std::vector<std::string>& frames) {
        if (m_CurrentFrames == frames) return;
        m_CurrentFrames = frames;
        m_Drawable = std::make_shared<Util::Animation>(
            frames, true, 120, true, 0
        );
        std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->Play();
    }

    std::vector<std::string> m_CurrentFrames;

    void updateFormation() {
        m_Transform.translation = {
            m_FormationPos.x + m_FormationOffsetX,
            m_FormationPos.y
        };
    }

    void updateDirectionAnimation() {
        glm::vec2 dir = m_Transform.translation - m_PrevPosition;
        float len = glm::length(dir);
        if (len < 0.01f) return;
        dir = dir / len;

        float angle = glm::degrees(std::atan2(dir.x, dir.y));
        m_Transform.rotation = -angle;
    }

    void StartReturn() {
        m_Transform.translation = {m_FormationPos.x, 500.0f};
        std::vector<BezierPath> returnPath = {
            {{ {m_FormationPos.x, 500.0f},
               {m_FormationPos.x, 350.0f},
               {m_FormationPos.x, 200.0f} }},
            {{ {m_FormationPos.x, 200.0f},
               {m_FormationPos.x, (m_FormationPos.y + 200.0f) / 2.0f},
               m_FormationPos }}
        };
        SetPath(returnPath);
        m_State = State::RETURNING;
    }
};

#endif