//
// Created by willyANO on 2026/3/13.
//

#include "PLAYER.hpp"

PLAYER::PLAYER(const std::string& imagePath) {
    m_Drawable = std::make_shared<Util::Image>(imagePath);
    m_Transform.translation = {0.0f, -250.0f};
    m_Visible = true;
}

void PLAYER::Move(float dx, float dy) {
    m_Transform.translation.x += dx * m_Speed;
    m_Transform.translation.y += dy * m_Speed;

    // 限制不能移出畫面（根據你的視窗大小調整）
    m_Transform.translation.x = glm::clamp(m_Transform.translation.x, -310.0f, 310.0f);
}