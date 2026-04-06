#ifndef STAGE_0_HPP
#define STAGE_0_HPP

#include "StageManager.hpp"

class Stage0 : public StageManager {
public:
    Stage0(Util::Renderer& root,
           std::vector<std::shared_ptr<Enemy>>& enemies)
        : StageManager(root, enemies) {}

protected:
    void buildSpawnList() override {
        m_SpawnList.clear();

        std::vector<glm::vec2> row1 = {
            {-200.0f, 220.0f}, {-120.0f, 220.0f}, {-40.0f, 220.0f},
            {  40.0f, 220.0f}, { 120.0f, 220.0f}
        };
        std::vector<glm::vec2> row2 = {
            {-200.0f, 150.0f}, {-120.0f, 150.0f}, {-40.0f, 150.0f},
            {  40.0f, 150.0f}, { 120.0f, 150.0f}
        };

        // 第一排：從右側飛入，路徑終點 = 編隊位置
        for (int i = 0; i < 5; i++) {
            glm::vec2 fpos = row1[i];
            std::vector<Enemy::BezierPath> path = {
                // 從右下角→右上角→畫面中上方
                {{ {500.0f, -250.0f}, {500.0f, 300.0f}, {0.0f, 300.0f} }},
                // 從中上方→彎向編隊位置（終點 = fpos）
                {{ {0.0f, 300.0f}, {fpos.x, 300.0f}, fpos }}
            };
            m_SpawnList.push_back({
                {500.0f, -250.0f},
                fpos,
                path,
                300.0f * i
            });
        }

        // 第二排：從左側飛入，鏡像，路徑終點 = 編隊位置
        for (int i = 0; i < 5; i++) {
            glm::vec2 fpos = row2[i];
            std::vector<Enemy::BezierPath> path = {
                {{ {-500.0f, -250.0f}, {-500.0f, 300.0f}, {0.0f, 300.0f} }},
                {{ {0.0f, 300.0f}, {fpos.x, 300.0f}, fpos }}
            };
            m_SpawnList.push_back({
                {-500.0f, -250.0f},
                fpos,
                path,
                1500.0f + 300.0f * i
            });
        }
    }
};

#endif