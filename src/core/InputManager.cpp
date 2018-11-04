#include <memory>

#include "core/InputManager.h"

InputManager* InputManager::m_pInstance = nullptr;

void InputManager::Init(HWND window)
{
	m_Keyboard = std::make_unique<DirectX::Keyboard>();
	m_Mouse = std::make_unique<DirectX::Mouse>();
	m_Mouse->SetWindow(window);
}