#pragma once

#include <string>
#include <memory>
#include <Keyboard.h>
#include <Mouse.h>

class JRenderer;
class Scene;

class Engine
{
public:
    bool Init();
    void Run();

	void ShutDown();

    static Engine* GetInstance()
    {
        if (m_spInstance == nullptr)
            m_spInstance = new Engine();

        return m_spInstance;
    }

    LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool isRunning() { return m_Running; }

private:
    Engine() {};

    bool InitWindow();

    void HandleEvents();
    void Update();
    void DrawScene(Scene* scene);

	void OnKeyDown(WPARAM keyState);

    HINSTANCE		m_hAppInst;
    HWND			m_hMainWnd;
    bool			m_Minimized;
    bool			m_Maximized;
    bool			m_Resizing;
    bool			m_Running;
    int				m_ClientWidth;
    int				m_ClientHeight;
    std::wstring	m_MainWndCaption;


    JRenderer* m_JRenderer;
	Scene* m_ActiveScene;

	std::unique_ptr<DirectX::Keyboard> m_Keyboard;
	std::unique_ptr<DirectX::Mouse> m_Mouse;

    static Engine* m_spInstance;
};




