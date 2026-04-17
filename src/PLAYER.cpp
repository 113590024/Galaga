//
// Created by willyANO on 2026/3/13.
//

#include "PLAYER.hpp"

PLAYER::PLAYER(const std::string& imagePath) {
    m_Drawable = std::make_shared<Util::Image>(imagePath);
    m_Transform.translation = {-150.0f, -275.0f};
    m_Visible = true;
}

void PLAYER::Move(float dx, float dy) {
    m_Transform.translation.x += dx * m_Speed;
    m_Transform.translation.x = glm::clamp(m_Transform.translation.x, -600.0f, 300.0f);
    //中心點 x:-150.0f y:0.0f
}