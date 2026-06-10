#ifndef GALAGA_STAGE2_HPP
#define GALAGA_STAGE2_HPP

#include "Stage.hpp"

class Stage2 :public Stage{
public:
    Stage2() {
        stagelevel = 2;
        totalEnemies = 40;
        nowWaveEnemies=10;
        m_WaveEnemiesKilled=0;
        Wave1();
    }

    // 敵人消滅
    void OnEnemyKilled() override {
        m_WaveEnemiesKilled++;
        CheckWaveFinished();
    }

    // 看波結束了沒
    void CheckWaveFinished() {
        if (m_MissEnemies + m_WaveEnemiesKilled < nowWaveEnemies) return;
        if (!IsSpawnDone()) return;

        m_TotalEnemieskill += m_WaveEnemiesKilled;
        m_TotalMissEnemies += m_MissEnemies;

        m_WaveEnemiesKilled = 0;
        m_MissEnemies = 0;

        m_CurrentWave++;

        switch (m_CurrentWave+1) {
            case 2:
                Wave2();
                nowWaveEnemies=10;
                break;
            /*case 3:
                Wave3();
                nowWaveEnemies=10;
                break;
            case 4:
                Wave4();
                nowWaveEnemies=10;
                break;*/
            default:
                stageclear = true;
        }
    }

    void Wave1() {
        m_ZakoList.push_back({});
        m_ButterflyList.push_back({});
        m_GalagaList.push_back({});

        // ZAKO
        for (int i = 0; i < 10; i++) {
            std::vector<Enemy::BezierPath> path = {
                { { {500.0f, 350.0f}, {-390.0f, 430.0f}, {-300.0f, -80.0f}, {-150.0f, -100.0f} } },
                { { {-150.0f, -100.0f}, {40.0f, -100.0f}, {50.0f, 450.0f}, {-600.0f,350.0f} } },

                { { {-600.0f, 350.0f}, {90.0f, 430.0f}, {0.0f, -80.0f}, {-150.0f, -100.0f} } },
                { { {-150.0f, -100.0f}, {-270.0f, -110.0f}, {-450.0f, 390.0f}, {800.0f,350.0f} } }
            };
            glm::vec2 fPos={800.0f,350.0f};
            m_ZakoList[m_CurrentWave].push_back(
                std::make_shared<Zako>(glm::vec2{-100.0f, 500.0f}, fPos, path)
            );
        }
    }
    void Wave2() {
        m_ZakoList.push_back({});
        m_ButterflyList.push_back({});
        m_GalagaList.push_back({});

        // Butterfly
        for (int i = 0; i < 10; i++) {
            std::vector<Enemy::BezierPath> path = {
                { { {-800.0f, -300.0f}, {-150.0f, 0.0f}, {-150.0f, 0.0f}, {-150.0f, 0.0f} } },
                { { {-150.0f, 0.0f}, {300.0f, 250.0f}, {350.0f, -50.0f}, {-150.0f,0.0f} } },

                { { {-150.0f, 0.0f}, {-600.0f, 250.0f}, {-650.0f, -50.0f}, {-150.0f, 0.0f} } },
                { { {-150.0f, 0.0f}, {-150.0f, 0.0f}, {-150.0f, 0.0f}, {800.0f,-800.0f} } }
            };
            glm::vec2 fPos={800.0f,800.0f};
            m_ButterflyList[m_CurrentWave].push_back(
                std::make_shared<Butterfly>(glm::vec2{-100.0f, 500.0f}, fPos, path)
            );
        }
    }

    void Update(std::vector<std::shared_ptr<Enemy>>& enemies, Util::Renderer& root) override {
        if (stageclear) return;
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
                CheckWaveFinished();
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
        return m_TotalMissEnemies;
    }
private:
    std::vector<std::vector<std::shared_ptr<Enemy>>> m_ZakoList;
    std::vector<std::vector<std::shared_ptr<Enemy>>> m_ButterflyList;
    std::vector<std::vector<std::shared_ptr<Enemy>>> m_GalagaList;
    int m_ZakoIndex = 0;
    int m_ButterflyIndex = 0;
    int m_GalagaIndex = 0;
    float m_Timer = 0.0f;
    int m_MissEnemies = 0;          // 目前 wave miss
    int m_TotalEnemieskill = 0;     // 已完成 wave 的 kill 總數
    int m_TotalMissEnemies = 0;     // 已完成 wave 的 miss 總數
};

#endif //GALAGA_STAGE2_HPP