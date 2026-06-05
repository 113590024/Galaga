//
// Created by TUF on 2026/4/6.
//

#ifndef GALAGA_STAGE2_HPP
#define GALAGA_STAGE2_HPP

#include "Stage.hpp"

class Stage2 :public Stage{
public:
    Stage2() {
        stagelevel = 2;
        totalEnemies = 8;
        nowWaveEnemies=8;
        m_WaveEnemiesKilled=0;
        Wave1();
    }

    void OnEnemyKilled() override {
        m_WaveEnemiesKilled++;
        if (m_MissEnemies+m_WaveEnemiesKilled >=nowWaveEnemies  && IsSpawnDone()) {
            m_CurrentWave++;          // 切到下一波
            m_TotalEnemieskill+=m_WaveEnemiesKilled;
            m_WaveEnemiesKilled = 0;  // 重置擊殺數
            m_MissEnemies=0;
            m_ZakoIndex = 0;          // 重置索引
            m_ButterflyIndex = 0;
            m_GalagaIndex = 0;
            switch (m_CurrentWave+1) {
                case 2:
                    //Wave2();
                    //nowWaveEnemies=;
                    break;
            }
        }
    }

    void Wave1() {
        m_ZakoList.push_back({});
        m_ButterflyList.push_back({});
        m_GalagaList.push_back({});

        // ZAKO
        for (int i = 0; i < 20; i++) {
            std::vector<Enemy::BezierPath> path = {
                { { {-100.0f, 500.0f}, {-100.0f, 350.0f}, {-100.0f, 350.0f}, {-400.0f, 180.0f} } },
                { { {-400.0f, 180.0f}, {-600.0f, 120.0f}, {-400.0f, -250.0f}, {-999.0f,-999.0f} } }
            };
            glm::vec2 fPos={-999.0f,-999.0f};
            m_ZakoList[m_CurrentWave].push_back(
                std::make_shared<Zako>(glm::vec2{-100.0f, 500.0f}, fPos, path)
            );
        }
    }
    void Update(std::vector<std::shared_ptr<Enemy>>& enemies, Util::Renderer& root) override {
        (void)enemies;
        (void)root;
        m_Timer -= Util::Time::GetDeltaTimeMs();
        if (m_Timer <= 0) {
            if (m_ZakoIndex < (int)m_ZakoList[m_CurrentWave].size()) {
                enemies.push_back(m_ZakoList[m_CurrentWave][m_ZakoIndex]);
                root.AddChild(m_ZakoList[m_CurrentWave][m_ZakoIndex]);
                m_ZakoIndex++;
            }
            if (m_ButterflyIndex < (int)m_ButterflyList[m_CurrentWave].size()) {
                enemies.push_back(m_ButterflyList[m_CurrentWave][m_ButterflyIndex]);
                root.AddChild(m_ButterflyList[m_CurrentWave][m_ButterflyIndex]);
                m_ButterflyIndex++;
            }
            if (m_GalagaIndex < (int)m_GalagaList[m_CurrentWave].size()) {
                enemies.push_back(m_GalagaList[m_CurrentWave][m_GalagaIndex]);
                root.AddChild(m_GalagaList[m_CurrentWave][m_GalagaIndex]);
                m_GalagaIndex++;
            }
            m_Timer = 300.0f;
        }
        //跑出螢幕外的敵人
        for (auto& enemy : enemies) {
            if (enemy->IsOutOfScreen() && enemy->IsAlive()) {
                enemy->Kill();
                m_MissEnemies++;
            }
        }
    }
    [[nodiscard]] bool IsSpawnDone() {
        return m_ZakoIndex >= (int)m_ZakoList[m_CurrentWave].size() &&
               m_ButterflyIndex >= (int)m_ButterflyList[m_CurrentWave].size() &&
               m_GalagaIndex >= (int)m_GalagaList[m_CurrentWave].size() ;
    }

    int TotalEnemieskill() {
        return m_TotalEnemieskill;
    }
    int TotalMissEnemies() {
        return m_MissEnemies;
    }
private:
    std::vector<std::vector<std::shared_ptr<Enemy>>> m_ZakoList;
    std::vector<std::vector<std::shared_ptr<Enemy>>> m_ButterflyList;
    std::vector<std::vector<std::shared_ptr<Enemy>>> m_GalagaList;
    int m_ZakoIndex = 0;
    int m_ButterflyIndex = 0;
    int m_GalagaIndex = 0;
    float m_Timer = 0.0f;
    int m_MissEnemies = 0;
    int m_TotalEnemieskill = 0; //獎勵關卡總擊殺
};

#endif //GALAGA_STAGE2_HPP