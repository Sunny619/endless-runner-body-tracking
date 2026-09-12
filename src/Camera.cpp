#include "Camera.hpp"
#include <cmath>
#include <cstdlib>

namespace Engine {

Camera::Camera()
    : m_eyeX(0.0f)
    , m_eyeY(1.35f)
    , m_eyeZ(3.5f)
    , m_targetX(0.0f)
    , m_targetY(0.35f)
    , m_targetZ(0.0f)
    , m_upX(0.0f)
    , m_upY(1.0f)
    , m_upZ(0.0f)
    , m_offsetY(1.0f)
    , m_offsetZ(1.5f)
    , m_shakeIntensity(0.0f)
    , m_shakeDecay(0.90f)
{
}

void Camera::Update(float targetX, float targetY, float targetZ) {
    m_targetX = targetX;
    m_targetY = targetY;
    m_targetZ = targetZ;

    float shakeOffsetX = 0.0f;
    float shakeOffsetY = 0.0f;

    if (m_shakeIntensity > 0.005f) {
        shakeOffsetX = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * m_shakeIntensity;
        shakeOffsetY = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * m_shakeIntensity;
        m_shakeIntensity *= m_shakeDecay;
    } else {
        m_shakeIntensity = 0.0f;
    }

    m_eyeX = targetX + shakeOffsetX;
    m_eyeY = targetY + m_offsetY + shakeOffsetY;
    m_eyeZ = targetZ + m_offsetZ;
}

void Camera::ApplyViewMatrix() const {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        m_eyeX, m_eyeY, m_eyeZ,
        m_targetX, m_targetY, m_targetZ,
        m_upX, m_upY, m_upZ
    );
}

void Camera::ApplyProjectionMatrix(int width, int height) const {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Maintain symmetric frustum matching original aspect ratio
    glFrustum(-2.0, 2.0, -2.0, 2.0, 1.0, 10.0);
}

void Camera::TriggerShake(float intensity) {
    m_shakeIntensity = intensity;
}

} // namespace Engine
