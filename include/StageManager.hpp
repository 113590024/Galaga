#ifndef STAGE_MANAGER_HPP
#define STAGE_MANAGER_HPP

#include "Enemy.hpp"
#include "Zako.hpp"
#include "Util/Renderer.hpp"
#include "Util/Time.hpp"
#include <vector>

class StageManager {
public:
    StageManager(Util::Renderer& root,
                 std::vector<std::shared_ptr<Enemy>>& enemies)
        : m_Root(root), m_Enemies(enemies) {}

    virtual ~StageManager() = default;

    virtual void StartStage() {
        m_SpawnIndex = 0;
        m_SpawnTimer = 0.0f;
        m_FormationOffsetX = 0.0f;
        m_FormationDir = 1.0f;
        m_StageComplete = false;
        buildSpawnList(); // 子類別實作
    }

    virtual void Update() {
        if (m_StageComplete) return;

        // 敵人入場計時
        if (m_SpawnIndex < (int)m_SpawnList.size()) {
            m_SpawnTimer += Util::Time::GetDeltaTimeMs();
            if (m_SpawnTimer >= m_SpawnList[m_SpawnIndex].delayMs) {
                spawnEnemy(m_SpawnList[m_SpawnIndex]);
                m_SpawnIndex++;
                m_SpawnTimer = 0.0f;
            }
        }

        // 更新敵人
        for (auto& enemy : m_Enemies) {
            if (!enemy->IsAlive()) continue;
            enemy->Update();
        }

        // 編隊左右移動
        updateFormationMove();

        // 所有敵人都死了且入場完畢 → 關卡結束
        if (m_SpawnIndex >= (int)m_SpawnList.size() && m_Enemies.empty()) {
            m_StageComplete = true;
        }
    }

    [[nodiscard]] bool IsStageComplete() const { return m_StageComplete; }

protected:
    struct SpawnData {
        glm::vec2 startPos;
        glm::vec2 formationPos;
        std::vector<Enemy::BezierPath> path;
        float delayMs;
    };

    Util::Renderer& m_Root;
    std::vector<std::shared_ptr<Enemy>>& m_Enemies;

    std::vector<SpawnData> m_SpawnList;
    int m_SpawnIndex = 0;
    float m_SpawnTimer = 0.0f;

    float m_FormationOffsetX = 0.0f;
    float m_FormationDir = 1.0f;
    float m_FormationSpeed = 0.5f;
    float m_FormationRange = 80.0f;
    bool m_StageComplete = false;

    // 子類別實作各自的關卡配置
    virtual void buildSpawnList() = 0;

    void spawnEnemy(const SpawnData& data) {
        auto zako = std::make_shared<Zako>(
            data.startPos, data.formationPos, data.path
        );
        m_Enemies.push_back(zako);
        m_Root.AddChild(zako);
    }

    void updateFormationMove() {
        m_FormationOffsetX += m_FormationDir * m_FormationSpeed;
        if (std::abs(m_FormationOffsetX) >= m_FormationRange) {
            m_FormationDir *= -1.0f;
        }
        for (auto& enemy : m_Enemies) {
            if (!enemy->IsAlive()) continue;
            auto zako = std::dynamic_pointer_cast<Zako>(enemy);
            if (zako && zako->IsInFormation()) {
                zako->SetFormationOffset(m_FormationOffsetX);
            }
        }
    }
};

#endif