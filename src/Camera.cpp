#include "Camera.h"

Camera::Camera(float radius)
{
	m_Phi = 0;
	m_Theta = 0;
	m_Radius = radius;

	UpdatePosition();
	m_target = Vector3::Zero;
}

Camera::Camera() : Camera(20.0){}

void Camera::UpdatePosition()
{
	m_position = Vector3(m_Radius * cosf(m_Phi) * cosf(m_Theta),
		m_Radius * sinf(m_Phi),
		m_Radius * cosf(m_Phi) * sinf(m_Theta));
}
Matrix Camera::GetLookAtMatrix()
{
	return Matrix::CreateLookAtLH(Vector3(m_position), Vector3(m_target), Vector3(0.0f, 1.0f, 0.0f));
}

