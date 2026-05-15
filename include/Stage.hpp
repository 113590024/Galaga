#ifndef GALAGA_STAGE_HPP
#define GALAGA_STAGE_HPP

#include "Zako.hpp"
#include "Butterfly.hpp"
#include "Boss_Galaga.hpp"
#include "Util/Time.hpp"
#include <vector>
#include <memory>

class Stage {
public:
    int getStageLevel() {
        return stagelevel;
    }
    bool getstageclear() {
        return stageclear;
    }

    int TotalEnemyCount(){
        return totalEnemies;
    }

    virtual void OnEnemyKilled(){};

    virtual void Update(std::vector<std::shared_ptr<Enemy>>& enemies, Util::Renderer& root) = 0;

    int stagelevel=0;
    bool stageclear=false;
    int totalEnemies=100;
    int nowWaveEnemies=100;
    int m_CurrentWave = 0;
    int m_WaveEnemiesKilled=0;
private:
};

#endif //GALAGA_STAGE_HPP