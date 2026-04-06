#ifndef STAGE_ONE_HPP
#define STAGE_ONE_HPP

#include "StageManager.hpp"

class StageOne : public StageManager {
public:
    StageOne(Util::Renderer& root,
             std::vector<std::shared_ptr<Enemy>>& enemies)
        : StageManager(root, enemies) {}

protected:
    void buildSpawnList() override {
        m_SpawnList.clear();

        // 第一排：5隻從右側飛入
        std::vector<glm::vec2> row1Formation = {
            {-200.0f, 220.0f}, {-120.0f, 220.0f}, {-40.0f, 220.0f},
            { 40.0f,  220.0f}, { 120.0f, 220.0f}
        };
        for (int i = 0; i < 5; i++) {
            glm::vec2 fpos = row1Formation[i];
            m_SpawnList.push_back({
                {600.0f, 200.0f},
                fpos,
                {{{ {600.0f, 200.0f}, {300.0f, 50.0f}, fpos }}},
                300.0f * i
            });
        }

        // 第二排：5隻從左側飛入
        std::vector<glm::vec2> row2Formation = {
            {-200.0f, 150.0f}, {-120.0f, 150.0f}, {-40.0f, 150.0f},
            { 40.0f,  150.0f}, { 120.0f, 150.0f}
        };
        for (int i = 0; i < 5; i++) {
            glm::vec2 fpos = row2Formation[i];
            m_SpawnList.push_back({
                {-600.0f, 200.0f},
                fpos,
                {{{ {-600.0f, 200.0f}, {-300.0f, 50.0f}, fpos }}},
                1500.0f + 300.0f * i
            });
        }
    }
};

#endif