#ifndef PLAYER_HPP
#define PLAYER_HPP

namespace Engine {

class Player {
private:
    float m_posX;
    float m_posY;
    float m_posZ;
    float m_targetLaneX;

    bool m_isJumping;
    float m_jumpPhaseDeg;
    float m_groundY;
    float m_forwardSpeed;

    int m_immunityTicks;
    int m_remainingLives;

public:
    Player();

    void Reset();
    void Update();

    void MoveLeft();
    void MoveRight();
    void Jump();

    bool IsJumping() const { return m_isJumping; }
    float GetX() const { return m_posX; }
    float GetY() const { return m_posY; }
    float GetZ() const { return m_posZ; }
    float GetGroundY() const { return m_groundY; }

    int GetLives() const { return m_remainingLives; }
    void DecrementLife();
    bool IsImmune() const { return m_immunityTicks > 0; }
    void SetImmunity(int ticks);

    void SetForwardSpeed(float speed) { m_forwardSpeed = speed; }
    float GetForwardSpeed() const { return m_forwardSpeed; }
};

} // namespace Engine

#endif // PLAYER_HPP
