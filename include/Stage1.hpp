#ifndef GALAGA_STAGE1_HPP
#define GALAGA_STAGE1_HPP

#include "Zako.hpp"
#include "Butterfly.hpp"
#include "Boss_Galaga.hpp"
#include "Util/Time.hpp"
#include <vector>
#include <memory>

class Stage1 {
public:
    Stage1() {
        
    };
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