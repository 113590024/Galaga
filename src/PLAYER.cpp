//
// Created by willyANO on 2026/3/13.
//

#include "PLAYER.hpp"

PLAYER::PLAYER(const std::string& imagePath) {
    m_Drawable = std::make_shared<Util::Image>(imagePath);
    m_Transform.translation = {0.0f, -275.0f};
    m_Visible = true;
}

void PLAYER::Move(float dx, float dy) {
    m_Transform.translation.x += dx * m_Speed;
    // 右邊界改成 +160，不讓玩家跑進 UI 區
    m_Transform.translation.x = glm::clamp(m_Transform.translation.x, -600.0f, 160.0f);
}