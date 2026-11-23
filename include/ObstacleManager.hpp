#ifndef OBSTACLE_MANAGER_HPP
#define OBSTACLE_MANAGER_HPP

#include "GameConfig.hpp"
#include <vector>

namespace Engine {

class Player;

struct ObstacleInstance {
    float posX;
    float posZ;
    float scaleX;
    float scaleY;
    float scaleZ;
    bool isActive;
    int obstacleType; // 0 = standard ground cube, 1 = floating barrier, 2 = hazard
};

class ObstacleManager {
private:
    float m_enemyLanesPrimary[GameConfig::MAX_OBSTACLES];
    float m_enemyLanesSecondary[GameConfig::MAX_OBSTACLES];
    float m_enemyLanesTertiary[GameConfig::MAX_OBSTACLES];

public:
    ObstacleManager();

    void Initialize(unsigned int seed = 0);
    bool CheckCollisions(Player& player);
    void RenderVisible(float playerZ) const;

    float GetLaneOffset(int index, int tier) const;
};

} // namespace Engine

#endif // OBSTACLE_MANAGER_HPP
