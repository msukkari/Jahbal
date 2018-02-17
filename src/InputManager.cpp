#include <memory>

#include "InputManager.h"

InputManager* InputManager::m_pInstance = nullptr;

void InputManager::Init(HWND window)
{
	m_Keyboard = std::make_unique<Keyboard>();
	m_Mouse = std::make_unique<Mouse>();
	m_Mouse->SetWindow(window);
}