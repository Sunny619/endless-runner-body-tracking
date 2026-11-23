#include "Player.hpp"
#include "GameConfig.hpp"
#include <cmath>

namespace Engine {

Player::Player() {
    Reset();
}

void Player::Reset() {
    m_groundY = GameConfig::PLAYER_DEFAULT_Y;
    m_posX = GameConfig::LANE_POS_CENTER;
    m_posY = m_groundY;
    m_posZ = 0.0f;
    m_targetLaneX = GameConfig::LANE_POS_CENTER;

    m_isJumping = false;
    m_jumpPhaseDeg = 0.0f;
    m_forwardSpeed = GameConfig::FORWARD_VELOCITY;

    m_immunityTicks = 0;
    m_remainingLives = GameConfig::MAX_LIVES;
}

void Player::Update() {
    // Advance forward continuously along negative Z axis
    m_posZ -= m_forwardSpeed;

    // Smooth lateral transition towards target lane
    float laneDiff = m_targetLaneX - m_posX;
    if (std::abs(laneDiff) > 0.05f) {
        m_posX += laneDiff * 0.35f;
    } else {
        m_posX = m_targetLaneX;
    }

    // Execute jump kinematics based on sinusoidal elevation trajectory
    if (m_isJumping) {
        if (m_jumpPhaseDeg >= 180.0f) {
            m_jumpPhaseDeg = 0.0f;
            m_posY = m_groundY;
            m_isJumping = false;
        } else {
            float rad = m_jumpPhaseDeg * 3.14159265f / 180.0f;
            m_posY = m_groundY + std::sin(rad);
            m_jumpPhaseDeg += GameConfig::JUMP_STEP_DEG;
        }
    }

    // Decrement post-collision immunity invulnerability frame counter
    if (m_immunityTicks > 0) {
        m_immunityTicks--;
    }
}

void Player::MoveLeft() {
    // Restrict lateral movement to 3 discrete lanes (-1.4f, 0.0f, 1.4f)
    if (m_targetLaneX > GameConfig::LANE_POS_LEFT + 0.1f) {
        m_targetLaneX -= GameConfig::LANE_WIDTH;
    }
}

void Player::MoveRight() {
    if (m_targetLaneX < GameConfig::LANE_POS_RIGHT - 0.1f) {
        m_targetLaneX += GameConfig::LANE_WIDTH;
    }
}

void Player::Jump() {
    if (!m_isJumping) {
        m_isJumping = true;
        m_jumpPhaseDeg = 0.0f;
    }
}

void Player::DecrementLife() {
    if (m_remainingLives > 0) {
        m_remainingLives--;
    }
}

void Player::SetImmunity(int ticks) {
    m_immunityTicks = ticks;
}

} // namespace Engine
