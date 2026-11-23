#ifndef CAMERA_HPP
#define CAMERA_HPP

#if defined(__APPLE__)
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

namespace Engine {

class Camera {
private:
    float m_eyeX;
    float m_eyeY;
    float m_eyeZ;
    float m_targetX;
    float m_targetY;
    float m_targetZ;
    float m_upX;
    float m_upY;
    float m_upZ;

    float m_offsetY;
    float m_offsetZ;
    float m_shakeIntensity;
    float m_shakeDecay;

public:
    Camera();

    void Update(float targetX, float targetY, float targetZ);
    void ApplyViewMatrix() const;
    void ApplyProjectionMatrix(int width, int height) const;
    void TriggerShake(float intensity = 0.15f);

    float GetEyeX() const { return m_eyeX; }
    float GetEyeY() const { return m_eyeY; }
    float GetEyeZ() const { return m_eyeZ; }
};

} // namespace Engine

#endif // CAMERA_HPP
