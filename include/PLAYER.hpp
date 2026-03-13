//
// Created by willyANO on 2026/3/13.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class PLAYER : public Util::GameObject {
public:
    explicit PLAYER(const std::string& imagePath);

    void Move(float dx, float dy);

    [[nodiscard]] glm::vec2 GetPosition() const { return m_Transform.translation; }
    void SetPosition(const glm::vec2& pos) { m_Transform.translation = pos; }

private:
    float m_Speed = 5.0f;
    // 之後加：float m_HP = 3;
};

#endif