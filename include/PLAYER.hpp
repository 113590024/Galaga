//
// Created by willyANO on 2026/3/13.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"

class PLAYER : public Util::GameObject {
public:
    explicit PLAYER(const std::string& imagePath);

    void Move(float dx, float dy);

    [[nodiscard]] glm::vec2 GetPosition() const { return m_Transform.translation; }
    void SetPosition(const glm::vec2& pos) {
		m_Transform.translation = pos;
	}

    void TakeDamage() {
        if (m_IsInvincible) return;  // 無敵時不扣血
        m_HP = m_HP - 1;
        m_IsInvincible = true;
        m_InvincibleTimer = 5000.0f;  // 死亡後5秒無敵
    }
    void Update() {
        if (m_IsInvincible) {
            m_InvincibleTimer -= Util::Time::GetDeltaTimeMs();
            if (m_InvincibleTimer <= 0.0f) {
                m_IsInvincible = false;
            }
        }
    }

    void ResetPosition() {
        m_Transform.translation = {-125.0f, -300.0f};
    }

    [[nodiscard]] bool IsDead() const { return m_HP <= 0; }
    [[nodiscard]] bool IsAlive() const { return m_HP > 0; }
    [[nodiscard]] int GetHP() const { return m_HP; }
    [[nodiscard]] int GetVisibility() const { return m_IsInvincible; }
    void ResetHP() { m_HP = 3; }

private:
    float m_Speed = 10.0f;
    int m_HP = 3;
    bool m_IsInvincible = false;
    float m_InvincibleTimer = 0.0f;
};

#endif