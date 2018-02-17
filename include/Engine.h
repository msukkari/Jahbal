#pragma once

#include <string>
#include <memory>
#include <DirectXTK/Keyboard.h>
#include <DirectXTK/Mouse.h>

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


	JRenderer* GetRenderer() { return m_JRenderer; }

private:
    Engine() {};

    bool InitWindow();

    void HandleEvents();
    void Update(float dt);
    void DrawScene(Scene* scene);

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

    static Engine* m_spInstance;
};




