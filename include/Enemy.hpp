#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <vector>

class Enemy : public Util::GameObject {
public:
    enum class State {
        ENTERING,   // 入場中
        FORMATION,  // 編隊待機
        DIVING,     // 俯衝中
        RETURNING,  // 返回編隊
    };

    //移動路徑
    struct BezierPath {
        std::vector<glm::vec2> points;
    };

    explicit Enemy(const std::string& imagePath, float zIndex = 10) {
        if (!imagePath.empty()) {
            m_Drawable = std::make_shared<Util::Image>(imagePath);
        }
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

    // 俯衝
    virtual void StartDive(const glm::vec2& playerPos) {
        if (stopdiving==true) {
            m_State = State::FORMATION;
            return;
        }
        m_State = State::DIVING;
    }

    [[nodiscard]] bool IsEntering() const {
        return m_State == State::ENTERING;
    }
    [[nodiscard]] bool IsInFormation() const {
        return m_State == State::FORMATION;
    }
    [[nodiscard]] bool IsDiving() const {
        return m_State == State::DIVING;
    }
    [[nodiscard]] bool IsReturning() const {
        return m_State == State::RETURNING;
    }

    // 還活著
    [[nodiscard]] bool IsAlive() const {
        return m_IsAlive;
    }

    // 拿分數
    [[nodiscard]] int GetScore() const {
        return m_Score;
    }

    //
     void Playerdead() {
        stopdiving=true;
    }

    // 嘎嘎
    void Kill() {
        m_IsAlive = false;
        m_Visible = false;
    }

    void TakeDamage(int damage) {
        m_health -= damage;
        if (m_health <= 0) {
            Kill();
        }
    }

    // 碰撞偵測
    [[nodiscard]] bool IfCollides(const glm::vec2& pos, float radius = 20.0f) const {
        auto myPos = GetPosition();
        float dx = myPos.x - pos.x;
        float dy = myPos.y - pos.y;
        return (dx * dx + dy * dy) < (radius * radius);
    }

    // 由 AppUpdate 或 Stage 給入場路徑
    void SetPath(const std::vector<BezierPath>& path) {
        m_Path = path;
        m_PathIndex = 0;
        m_T = 0.0f;
        m_Transform.rotation = 0.0f;
        m_State = State::ENTERING;
    }

protected:
    bool m_IsAlive = true;
    int m_health=1;
    int m_Score = 0;
    State m_State = State::ENTERING;
    glm::vec2 m_FormationPos;
    bool stopdiving=false;

    std::vector<BezierPath> m_Path;//這隻敵人的所有路徑段
    int m_PathIndex = 0;//目前走到第幾段曲線
    float m_T = 0.0f;//目前這段曲線的進度

    void UpdatePath() {
        if (m_PathIndex >= m_Path.size()) {
            // 路徑走完
            if (m_State == State::ENTERING || m_State == State::DIVING) {
                m_State = State::FORMATION;
                m_Transform.translation = m_FormationPos;
            }
            return;
        }

        // 取得目前這段曲線
        auto& seg = m_Path[m_PathIndex];
        m_T += 0.01f;  // 移動速度
        if (m_T >= 1.0f) {
            m_T = 0.0f;
            m_PathIndex++;  // 走下一段
            return;
        }

        // Bezier 公式
        //(u + t)^3 = u³ + 3u²t + 3ut² + t³，u=1-t
        float u = 1.0f - m_T;
        int n = seg.points.size();
        glm::vec2 prevPos = m_Transform.translation;

        if (n == 2) {
            m_Transform.translation =
                u     * seg.points[0] +
                m_T   * seg.points[1];

        } else if (n == 3) {
            m_Transform.translation =
                u*u       * seg.points[0] +
                2*u*m_T   * seg.points[1] +
                m_T*m_T   * seg.points[2];

        } else if (n == 4) {
            m_Transform.translation =
                u*u*u         * seg.points[0] +
                3*u*u*m_T     * seg.points[1] +
                3*u*m_T*m_T   * seg.points[2] +
                m_T*m_T*m_T   * seg.points[3];
        }

        // 計算移動方向並旋轉
        /*glm::vec2 dir = m_Transform.translation - prevPos;
        if (glm::length(dir) > 0.001f) {
            // atan2 算出角度，+90度是因為圖片預設朝上
            float angle = glm::degrees(std::atan2(dir.x, dir.y));
            m_Transform.rotation = -angle;
        }*/
    }
};

#endif