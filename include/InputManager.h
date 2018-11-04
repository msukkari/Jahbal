#pragma once
#include <d3d11.h>

#include "DirectXTK\keyboard.h"
#include "DirectXTK\Mouse.h"

class InputManager
{
public:
	void Init(HWND window);
	
	static InputManager* GetInstance()
	{
		if (!m_pInstance) m_pInstance = new InputManager();
		return m_pInstance;
	}

	std::unique_ptr<DirectX::Keyboard> m_Keyboard;
	std::unique_ptr<DirectX::Mouse> m_Mouse;

private:
	InputManager() {}
	static InputManager* m_pInstance;
};