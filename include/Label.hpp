#ifndef LABEL_HPP
#define LABEL_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"

class Label : public Util::GameObject {
public:
    Label(const std::string& fontPath, int size, const std::string& text,
          const Util::Color& color, float zIndex = 0) {
        m_Drawable = std::make_unique<Util::Text>(fontPath, size, text, color);
        m_ZIndex = zIndex;
        m_Visible = true;
    }
    void SetPosition(const glm::vec2& pos) { m_Transform.translation = pos; }

    void SetText(const std::string& text) {
        std::dynamic_pointer_cast<Util::Text>(m_Drawable)->SetText(text);
    }
};

#endif