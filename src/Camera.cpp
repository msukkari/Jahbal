#include "Camera.h"


Camera::Camera()
{
	m_Phi = 0; m_Theta = 0; m_Radius = 20.0;

	UpdatePosition();
	m_target = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void Camera::UpdatePosition()
{
	m_position = Vector4(m_Radius * cosf(m_Phi) * cosf(m_Theta),
		m_Radius * sinf(m_Phi),
		m_Radius * cosf(m_Phi) * sinf(m_Theta),
		1.0f);
}
XMMATRIX Camera::GetLookAtMatrix()
{
	XMVECTOR pos = XMLoadFloat4(&m_position);
	XMVECTOR target = XMLoadFloat4(&m_target);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	return XMMatrixLookAtLH(pos, target, up);
}

