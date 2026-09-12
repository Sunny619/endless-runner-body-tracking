#include "ObstacleManager.hpp"
#include "Player.hpp"
#include "GameConfig.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>

namespace Engine {

ObstacleManager::ObstacleManager() {
    Initialize(static_cast<unsigned int>(time(nullptr)));
}

void ObstacleManager::Initialize(unsigned int seed) {
    srand(seed);
    for (int i = 0; i < GameConfig::MAX_OBSTACLES; i++) {
        // Randomly assign each obstacle to lane -1, 0, or 1 multiplied by lane width
        m_enemyLanesPrimary[i] = (rand() % 3 - 1) * GameConfig::LANE_WIDTH;
        m_enemyLanesSecondary[i] = (rand() % 3 - 1) * GameConfig::LANE_WIDTH;
        m_enemyLanesTertiary[i] = (rand() % 3 - 1) * GameConfig::LANE_WIDTH;
    }
}

bool ObstacleManager::CheckCollisions(Player& player) {
    if (player.IsImmune()) {
        return false;
    }

    float playerX = player.GetX();
    float playerY = player.GetY();
    float playerZ = player.GetZ();
    float groundY = player.GetGroundY();

    bool collisionOccurred = false;

    for (int i = -6, j = 0; i > -GameConfig::MAP_LENGTH / 2 && j < GameConfig::MAX_OBSTACLES; i -= 12, j++) {
        float obstacleZ = static_cast<float>(i);

        // Process only obstacles close to or slightly behind the player
        if (obstacleZ < playerZ + 0.5f) {
            // Check primary obstacle lane
            if (std::abs(playerX - m_enemyLanesPrimary[j]) < 0.7f && obstacleZ > playerZ && (playerY - groundY < GameConfig::COLLISION_RADIUS)) {
                collisionOccurred = true;
                break;
            }

            // Check secondary periodic obstacle (every 3rd interval)
            if (j % 3 == 0) {
                if (std::abs(playerX - m_enemyLanesSecondary[j]) < 0.7f && obstacleZ > playerZ && (playerY - groundY < GameConfig::COLLISION_RADIUS)) {
                    collisionOccurred = true;
                    break;
                }
            }

            // Check tertiary periodic obstacle (every 6th interval)
            if (j % 6 == 0) {
                if (std::abs(playerX - m_enemyLanesTertiary[j]) < 0.7f && obstacleZ > playerZ && (playerY - groundY < GameConfig::COLLISION_RADIUS)) {
                    collisionOccurred = true;
                    break;
                }
            }
        }
    }

    if (collisionOccurred) {
        player.DecrementLife();
        player.SetImmunity(GameConfig::DEFAULT_IMMUNITY_TICKS);
        return true;
    }

    return false;
}

void ObstacleManager::RenderVisible(float playerZ) const {
    for (int i = -6, j = 0; i > -GameConfig::MAP_LENGTH / 2 && j < GameConfig::MAX_OBSTACLES; i -= 12, j++) {
        float obstacleZ = static_cast<float>(i);

        if (obstacleZ < playerZ + 2.0f) {
            // Draw primary enemy cube
            glPushMatrix();
            glTranslatef(m_enemyLanesPrimary[j], GameConfig::PLAYER_DEFAULT_Y, obstacleZ);
            glScalef(1.0f, 0.7f, 1.0f);
            glutSolidCube(1.0);
            glPopMatrix();

            // Draw secondary obstacle
            if (j % 3 == 0) {
                glPushMatrix();
                glTranslatef(m_enemyLanesSecondary[j], GameConfig::PLAYER_DEFAULT_Y, obstacleZ);
                glScalef(1.0f, 0.7f, 1.0f);
                glutSolidCube(1.0);
                glPopMatrix();
            }

            // Draw tertiary obstacle
            if (j % 6 == 0) {
                glPushMatrix();
                glTranslatef(m_enemyLanesTertiary[j], GameConfig::PLAYER_DEFAULT_Y, obstacleZ);
                glScalef(1.0f, 0.7f, 1.0f);
                glutSolidCube(1.0);
                glPopMatrix();
            }
        }
    }
}

float ObstacleManager::GetLaneOffset(int index, int tier) const {
    if (index < 0 || index >= GameConfig::MAX_OBSTACLES) return 0.0f;
    if (tier == 1) return m_enemyLanesPrimary[index];
    if (tier == 2) return m_enemyLanesSecondary[index];
    return m_enemyLanesTertiary[index];
}

} // namespace Engine
