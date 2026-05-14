#ifndef GALAGA_STAGE1_HPP
#define GALAGA_STAGE1_HPP

#include "Stage.hpp"

class Stage1 :public Stage {
public:
    Stage1() {
        stagelevel = 1;
        stageclear = false;
        totalEnemies = 0; // 還沒設計，所以暫時沒有敵人
    }

    void Update(std::vector<std::shared_ptr<Enemy>>& enemies, Util::Renderer& root) override {
        (void)enemies;
        (void)root;
        stageclear = true;
    }
private:
    std::vector<std::shared_ptr<Enemy>> m_ZakoList;
    std::vector<std::shared_ptr<Enemy>> m_ButterflyList;
    std::vector<std::shared_ptr<Enemy>> m_GalagaList;
    int m_ZakoIndex = 0;
    int m_ButterflyIndex = 0;
    int m_GalagaIndex = 0;
    int totalEnemies = 10000000;
    float m_Timer = 0.0f;
};

#endif //GALAGA_STAGE1_HPP