//
// Created by TUF on 2026/4/6.
//

#ifndef GALAGA_STAGE0_0_HPP
#define GALAGA_STAGE0_0_HPP

#include "Zako.hpp"
#include "Butterfly.hpp"
#include "Boss_Galaga.hpp"
#include "Util/Time.hpp"
#include <vector>
#include <memory>

class Stage0_0 {
public:
    Stage0_0() {
        //編隊位置
        std::vector<glm::vec2> formationPositions = {
            // ZAKO
            {-350.0f, 100.0f},{-300.0f,  100.0f},{-250.0f, 100.0f},
            {-200.0f, 100.0f},{-150.0f, 100.0f},{-100.0f, 100.0f},
            {-50.0f,  100.0f},{0.0f, 100.0f},{50.0f, 100.0f},
            {100.0f, 100.0f},
            {-350.0f, 150.0f},{-300.0f,  150.0f},{-250.0f, 150.0f},
            {-200.0f, 150.0f},{-150.0f, 150.0f},{-100.0f, 150.0f},
            {-50.0f,  150.0f},{-0.0f, 150.0f},{50.0f, 150.0f},
            {100.0f, 150.0f},
            //Butterfly
            {50.0f,  200.0f},{0.0f, 200.0f},{-50.0f, 200.0f},
            {-100.0f, 200.0f},{-150.0f, 200.0f},{-200.0f,  200.0f},
            {-250.0f, 200.0f},{-300.0f, 200.0f},
            {50.0f,  250.0f},{0.0f, 250.0f},{-50.0f, 250.0f},
            {-100.0f, 250.0f},{-150.0f, 250.0f},{-200.0f,  250.0f},
            {-250.0f, 250.0f},{-300.0f, 250.0f},
            //Galaga右
            {-100.0f, 300.0f},{-50.0f, 300.0f},{0.0f, 300.0f},
            //Galaga左
            {-150.0f, 300.0f},{-200.0f, 300.0f},{-250.0f, 300.0f},
        };
        // ZAKO
        for (int i = 0; i < 20; i++) {
            glm::vec2 fPos = formationPositions[i];
            std::vector<Enemy::BezierPath> path = {
                { { {-100.0f, 500.0f}, {-100.0f, 350.0f}, {-100.0f, 350.0f}, {-400.0f, 180.0f} } },
                { { {-400.0f, 180.0f}, {-600.0f, 120.0f}, {-400.0f, -250.0f}, fPos } }
            };
            m_ZakoList.push_back(
                std::make_shared<Zako>(glm::vec2{-100.0f, 500.0f}, fPos, path)
            );
        }
        // Butterfly
        for (int i = 20; i < 36; i++) {
            glm::vec2 fPos = formationPositions[i];
            std::vector<Enemy::BezierPath> path = {
                { { {-200.0f, 500.0f}, {-200.0f, 350.0f},
                    {-200.0f, 350.0f}, {100.0f, 180.0f} } },
                { { {100.0f, 180.0f}, {300.0f, 120.0f},
                    {100.0f, -250.0f}, fPos } }
            };
            m_ButterflyList.push_back(
                std::make_shared<Butterfly>(glm::vec2{100.0f, 500.0f}, fPos, path)
            );
        }
        // Galaga右
        for (int i = 36; i < 39; i++) {
            glm::vec2 fPos = formationPositions[i];
            std::vector<Enemy::BezierPath> path = {
                { { {400.0f, -150.0f}, {100.0f, -100.0f},
                    {100.0f, -100.0f}, fPos } }
            };
            m_GalagaList.push_back(
                std::make_shared<Boss_Galaga>(glm::vec2{400.0f, -150.0f}, fPos, path)
            );
        }
        // Galaga左
        for (int i = 39; i < 42; i++) {
            glm::vec2 fPos = formationPositions[i];
            std::vector<Enemy::BezierPath> path = {
                { { {-700.0f, -150.0f}, {-450.0f, -100.0f},
                    {-450.0f, -100.0f}, fPos } }
            };
            m_GalagaList.push_back(
                std::make_shared<Boss_Galaga>(glm::vec2{-700.0f, -150.0f}, fPos, path)
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