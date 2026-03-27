#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Enemy : public Util::GameObject {
public:
    explicit Enemy(const std::string& imagePath, float zIndex = 10) {
        m_Drawable = std::make_shared<Util::Image>(imagePath);
        m_ZIndex = zIndex;
        m_Visible = true;
    }

    // 在子類別實作
    virtual void Update() = 0;

    [[nodiscard]] glm::vec2 GetPosition() const {
        return m_Transform.translation;
    }

    void SetPosition(const glm::vec2& pos) {
        m_Transform.translation = pos;
    }

    // 還活著
    [[nodiscard]] bool IsAlive() const {
        return m_IsAlive;
    }

    // 拿分數
    [[nodiscard]] int GetScore() const {
        return m_Score;
    }

    // 嘎嘎
    void Kill() {
        m_IsAlive = false;
        m_Visible = false;
    }

    /*void takeDamage() {

    }*/

    // 碰撞偵測
    [[nodiscard]] bool IfCollides(const glm::vec2& pos, float radius = 20.0f) const {
        auto myPos = GetPosition();
        float dx = myPos.x - pos.x;
        float dy = myPos.y - pos.y;
        return (dx * dx + dy * dy) < (radius * radius);
    }

protected:
    bool m_IsAlive = true;
    int health=1;
    // 擊殺得分
    int m_Score = 0;
};

#endif