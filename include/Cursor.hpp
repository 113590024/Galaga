#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "Label.hpp"

class Cursor : public Util::GameObject {
public:
    Cursor() {
        m_Drawable = std::make_unique<Util::Text>(
            RESOURCE_DIR"/Font/Emulogic-zrEw.ttf", 20, ">",
            Util::Color::FromRGB(255, 255, 255)
        );
        m_ZIndex = 15;
        m_Visible = true;
    }
    void SetPosition(const glm::vec2& pos) { m_Transform.translation = pos; }
};

#endif