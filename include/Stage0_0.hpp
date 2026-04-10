//
// Created by TUF on 2026/4/6.
//

#ifndef GALAGA_STAGE0_0_HPP
#define GALAGA_STAGE0_0_HPP

#include "Zako.hpp"
#include "Util/Time.hpp"
#include <vector>
#include <memory>

class Stage0_0 {
public:
    Stage0_0() {
        //編隊位置
        std::vector<glm::vec2> formationPositions = {
            {-200.0f, 100.0f},
            {-150.0f, 100.0f},
            {-100.0f,  100.0f},
            {-50.0f, 100.0f},
            {0.0f, 100.0f},
        };
        // 5隻 Zako 排隊從同一個起點進來
        for (int i = 0; i < 5; i++) {
            glm::vec2 fPos = formationPositions[i];
            std::vector<Enemy::BezierPath> path = {
                { { {-100.0f, 500.0f}, {-100.0f, 350.0f}, {-100.0f, 350.0f}, {-400.0f, 180.0f} } },
                { { {-400.0f, 180.0f}, {-600.0f, 120.0f}, {-400.0f, -250.0f}, fPos } }
            };
            m_SpawnList.push_back(
                std::make_shared<Zako>(
                glm::vec2{-100.0f, 500.0f},
                //glm::vec2{300.0f, 500.0f},
                fPos,
                        path
                  )
            );
        }
    }

    // 每幀更新，負責計時派出敵人
    void Update(std::vector<std::shared_ptr<Enemy>>& enemies, Util::Renderer& root) {
        m_Timer -= Util::Time::GetDeltaTimeMs();
        if (m_Timer <= 0 && m_SpawnIndex < m_SpawnList.size()) {
            auto& e = m_SpawnList[m_SpawnIndex];
            enemies.push_back(e);     // 加入敵人列表
            root.AddChild(e);
            m_SpawnIndex++;
            m_Timer = 300.0f;         // 每隻間隔300ms
        }
    }

    // 這關敵人全部入場完畢
    [[nodiscard]] bool IsSpawnDone() const {
        return m_SpawnIndex >= m_SpawnList.size();
    }

private:
    std::vector<std::shared_ptr<Zako>> m_SpawnList;
    std::vector<Enemy::BezierPath> m_Path;
    int m_SpawnIndex = 0;   //入場敵人數量
    float m_Timer = 0.0f;
};

#endif //GALAGA_STAGE0_0_HPP
/*
{ { {-100.0f, 500.0f}, {-100.0f, 350.0f}, {-400.0f, 180.0f} } },
{ { {-400.0f, 180.0f}, {-600.0f, 120.0f}, {-400.0f, -250.0f}, fPos } }*/