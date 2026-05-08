//
// Created by TUF on 2026/4/6.
//

#ifndef GALAGA_STAGE0_0_HPP
#define GALAGA_STAGE0_0_HPP

#include "Stage.hpp"

class Stage2 :public Stage{
public:
    Stage2(){
        //獎勵關卡 沒有編隊位置
        for (int i = 0; i < 8; i++) {
            glm::vec2 fPos = formationPositions[i];
            std::vector<Enemy::BezierPath> path = {
                { { {350.0f, 350.0f}, {-390.0f, 430.0f}, {-300.0f, -80.0f}, {-150.0f, -100.0f} } },
                { { {350.0f, 350.0f}, {-390.0f, 430.0f}, {-300.0f, -80.0f},{-150.0f, -100.0f} } }
            };
            m_ZakoList.push_back(
                std::make_shared<Zako>(glm::vec2{-100.0f, 500.0f}, fPos, path)
            );
        }
    }

    void Update(std::vector<std::shared_ptr<Enemy>>& enemies, Util::Renderer& root) {
        m_Timer -= Util::Time::GetDeltaTimeMs();
        if (m_Timer <= 0) {
            if (m_ZakoIndex < (int)m_ZakoList.size()) {
                enemies.push_back(m_ZakoList[m_ZakoIndex]);
                root.AddChild(m_ZakoList[m_ZakoIndex]);
                m_ZakoIndex++;
            }
            if (m_ButterflyIndex < (int)m_ButterflyList.size()) {
                enemies.push_back(m_ButterflyList[m_ButterflyIndex]);
                root.AddChild(m_ButterflyList[m_ButterflyIndex]);
                m_ButterflyIndex++;
            }
            if (m_GalagaIndex < (int)m_GalagaList.size()) {
                enemies.push_back(m_GalagaList[m_GalagaIndex]);
                root.AddChild(m_GalagaList[m_GalagaIndex]);
                m_GalagaIndex++;
            }
            m_Timer = 300.0f;
        }
    }

    [[nodiscard]] bool IsSpawnDone() {
        return m_ZakoIndex >= (int)m_ZakoList.size() &&
               m_ButterflyIndex >= (int)m_ButterflyList.size() &&
               m_GalagaIndex >= (int)m_GalagaList.size() ;
    }

    [[nodiscard]] int TotalEnemyCount(){
        return totalEnemies;
    }

private:
    std::vector<std::shared_ptr<Enemy>> m_ZakoList;
    std::vector<std::shared_ptr<Enemy>> m_ButterflyList;
    std::vector<std::shared_ptr<Enemy>> m_GalagaList;
    int m_ZakoIndex = 0;
    int m_ButterflyIndex = 0;
    int m_GalagaIndex = 0;
    int totalEnemies = 42;
    float m_Timer = 0.0f;
};

#endif //GALAGA_STAGE0_0_HPP