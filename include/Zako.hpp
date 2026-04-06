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

        // 待機動畫
        m_IdleFrames = {
            RESOURCE_DIR"/Image/Character/enemy_zako1.png",
            RESOURCE_DIR"/Image/Character/enemy_zako2.png",
        };
        // 俯衝動畫
        m_DiveFrames = {
            RESOURCE_DIR"/Image/Character/enemy_zako3.png",
            RESOURCE_DIR"/Image/Character/enemy_zako4.png",
            RESOURCE_DIR"/Image/Character/enemy_zako5.png",
            RESOURCE_DIR"/Image/Character/enemy_zako6.png",
            RESOURCE_DIR"/Image/Character/enemy_zako7.png",
            RESOURCE_DIR"/Image/Character/enemy_zako8.png",
        };

        setAnimation(m_IdleFrames);

        m_Transform.translation = startPos;
        m_Transform.scale = {1.0f, 1.0f};
        m_health = 1;
        m_Score = 100;
        m_FormationPos = formationPos;
        SetPath(entryPath);
    }

    void Update() override {
        m_PrevPosition = m_Transform.translation; // 記錄上一幀位置

        switch (m_State) {
            case State::ENTERING:
                UpdatePath();
                updateDirectionAnimation(); // 根據移動方向更新動畫
                // 路徑走完後切換到待機動畫
                if (m_State == State::FORMATION) {
                    setAnimation(m_IdleFrames);
                }
                break;
            case State::FORMATION:
                updateFormation();
                break;
            case State::DIVING:
                UpdatePath();
                updateDirectionAnimation();
                if (m_State == State::FORMATION) {
                    setAnimation(m_IdleFrames); // 返回後換回待機
                }
                break;
            case State::RETURNING:
                UpdatePath();
                updateDirectionAnimation();
                break;
        }
    }

    void SetFormationOffset(float offsetX) {
        m_FormationOffsetX = offsetX;
    }

    void StartDive(const glm::vec2& playerPos) override {
        (void)playerPos;
        m_State = State::DIVING;
        setAnimation(m_DiveFrames);
    }

private:
    float m_FormationOffsetX = 0.0f;
    std::vector<std::string> m_IdleFrames;
    std::vector<std::string> m_DiveFrames;
    glm::vec2 m_PrevPosition = {0.0f, 0.0f};

    void updateFormation() {
        m_Transform.translation = {
            m_FormationPos.x + m_FormationOffsetX,
            m_FormationPos.y
        };
    }

    void updateDirectionAnimation() {
        glm::vec2 dir = m_Transform.translation - m_PrevPosition;
        float len = glm::length(dir);
        if (len < 0.01f) return; // 沒在動就不換

        dir = dir / len; // 正規化

        // 根據方向角度決定用哪組動畫
        // atan2 回傳弧度，轉成角度
        float angle = glm::degrees(std::atan2(dir.y, dir.x));

        // 往上飛（-135 ~ -45度）→ 待機動畫
        // 往下飛（45 ~ 135度）→ 俯衝動畫
        // 往左右飛 → 俯衝動畫
        if (m_State == State::ENTERING || m_State == State::RETURNING) {
            if (angle > -135.0f && angle < -45.0f) {
                // 往上飛，用待機動畫
                if (getCurrentAnimFrames() != m_IdleFrames) {
                    setAnimation(m_IdleFrames);
                }
            } else {
                // 其他方向，用俯衝動畫
                if (getCurrentAnimFrames() != m_DiveFrames) {
                    setAnimation(m_DiveFrames);
                }
            }
        }
    }

    // 避免每幀重複建立 Animation，記錄目前用的 frames
    std::vector<std::string> m_CurrentFrames;

    std::vector<std::string> getCurrentAnimFrames() {
        return m_CurrentFrames;
    }

    void setAnimation(const std::vector<std::string>& frames) {
        if (m_CurrentFrames == frames) return; // 已經是這個動畫就不重建
        m_CurrentFrames = frames;
        m_Drawable = std::make_shared<Util::Animation>(
            frames, true, 120, true, 0
        );
        std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->Play();
    }
};

#endif