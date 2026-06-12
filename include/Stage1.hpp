#ifndef GALAGA_STAGE1_HPP
#define GALAGA_STAGE1_HPP

#include "Stage.hpp"

class Stage1 :public Stage {
public:
    Stage1() {
        stagelevel = 1;
        totalEnemies = 84;
        nowWaveEnemies=42;
        m_WaveEnemiesKilled=0;
        Wave1();
    }

    void OnEnemyKilled() override {
        m_WaveEnemiesKilled++;
        if (m_WaveEnemiesKilled >=nowWaveEnemies  && IsSpawnDone()) {
            m_CurrentWave++;          // 切到下一波
            m_WaveEnemiesKilled = 0;  // 重置擊殺數
            m_ZakoIndex = 0;          // 重置索引
            m_ButterflyIndex = 0;
            m_GalagaIndex = 0;
            switch (m_CurrentWave+1) {
                case 2:
                    Wave2();
                    nowWaveEnemies=42;
                    break;
            }
        }
    }

    void Wave1() {
        m_ZakoList.push_back({});
        m_ButterflyList.push_back({});
        m_GalagaList.push_back({});
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
                m_ZakoList[m_CurrentWave].push_back(
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
                m_ButterflyList[m_CurrentWave].push_back(
                    std::make_shared<Butterfly>(glm::vec2{-200.0f, 500.0f}, fPos, path)
                );
            }
            // Galaga右
            for (int i = 36; i < 39; i++) {
                glm::vec2 fPos = formationPositions[i];
                std::vector<Enemy::BezierPath> path = {
                    { { {400.0f, -150.0f}, {100.0f, -100.0f},
                        {100.0f, -100.0f}, fPos } }
                };
                m_GalagaList[m_CurrentWave].push_back(
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
                m_GalagaList[m_CurrentWave].push_back(
                    std::make_shared<Boss_Galaga>(glm::vec2{-700.0f, -150.0f}, fPos, path)
                );
            }
    }

    void Wave2() {
        m_ZakoList.push_back({});
        m_ButterflyList.push_back({});
        m_GalagaList.push_back({});
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
            m_ZakoList[m_CurrentWave].push_back(
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
            m_ButterflyList[m_CurrentWave].push_back(
                std::make_shared<Butterfly>(glm::vec2{-200.0f, 500.0f}, fPos, path)
            );
        }
        // Galaga右
        for (int i = 36; i < 39; i++) {
            glm::vec2 fPos = formationPositions[i];
            std::vector<Enemy::BezierPath> path = {
                { { {400.0f, -150.0f}, {100.0f, -100.0f},
                    {100.0f, -100.0f}, fPos } }
            };
            m_GalagaList[m_CurrentWave].push_back(
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
            m_GalagaList[m_CurrentWave].push_back(
                std::make_shared<Boss_Galaga>(glm::vec2{-700.0f, -150.0f}, fPos, path)
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
    }
    [[nodiscard]] bool IsSpawnDone() {
        return m_ZakoIndex >= (int)m_ZakoList[m_CurrentWave].size() &&
               m_ButterflyIndex >= (int)m_ButterflyList[m_CurrentWave].size() &&
               m_GalagaIndex >= (int)m_GalagaList[m_CurrentWave].size() ;
    }

private:
    std::vector<std::vector<std::shared_ptr<Enemy>>> m_ZakoList;
    std::vector<std::vector<std::shared_ptr<Enemy>>> m_ButterflyList;
    std::vector<std::vector<std::shared_ptr<Enemy>>> m_GalagaList;
    int m_ZakoIndex = 0;
    int m_ButterflyIndex = 0;
    int m_GalagaIndex = 0;
    float m_Timer = 0.0f;
};

#endif //GALAGA_STAGE1_HPP