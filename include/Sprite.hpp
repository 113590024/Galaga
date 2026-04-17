#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Sprite : public Util::GameObject {
public:
    Sprite(const std::string& imagePath, float zIndex = 0) {
        m_Drawable = std::make_unique<Util::Image>(imagePath);
        m_ZIndex = zIndex;
        m_Visible = true;
    }

    void SetPosition(const glm::vec2& pos) {
        m_Transform.translation = pos;
    }

    void SetScale(const glm::vec2& scale) {
        m_Transform.scale = scale;
    }

    void SetImage(const std::string& imagePath) {
        m_Drawable = std::make_shared<Util::Image>(imagePath);
    }

};

#endif