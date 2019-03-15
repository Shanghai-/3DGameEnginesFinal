#include "CCamera.h"

CCamera::CCamera(std::shared_ptr<GameObject> parent) :
    CCamera(parent, glm::vec3(0.f))
{
}

CCamera::CCamera(std::shared_ptr<GameObject> parent, glm::vec3 localPos) :
    Component(parent),
    m_isActive(true),
    m_r0(glm::vec4(0.f)),
    m_r1(glm::vec4(0.f)),
    m_r2(glm::vec4(0.f)),
    m_r3(glm::vec4(0.f)),
    m_localPos(localPos)
{
    m_cam = std::make_shared<Camera>();
}

CCamera::~CCamera()
{
}

// True if this is the camera Graphics should use this frame, false otherwise
bool CCamera::isActive() { return m_isActive; }

// Getter for the underlying Graphics Camera object
std::shared_ptr<Camera> CCamera::getCameraObject() { return m_cam; }

// Getter/setter for local position
void CCamera::setLocalPos(glm::vec3 val) { m_localPos = val; }
glm::vec3 CCamera::getLocalPos()         { return m_localPos; }

// Getters for frustum culling planes
glm::vec4 CCamera::getNegXPlane() { return m_r3 - m_r0; }
glm::vec4 CCamera::getNegYPlane() { return m_r3 - m_r1; }
glm::vec4 CCamera::getNegZPlane() { return m_r3 - m_r2; }
glm::vec4 CCamera::getPosXPlane() { return m_r3 + m_r0; }
glm::vec4 CCamera::getPosYPlane() { return m_r3 + m_r1; }
glm::vec4 CCamera::getPosZPlane() { return m_r3 + m_r2; }

// Setters for frustum matrix rows
void CCamera::setR0(glm::vec4 val) { m_r0 = val; }
void CCamera::setR1(glm::vec4 val) { m_r1 = val; }
void CCamera::setR2(glm::vec4 val) { m_r2 = val; }
void CCamera::setR3(glm::vec4 val) { m_r3 = val; }
