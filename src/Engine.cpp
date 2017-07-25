#include <windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include <sstream>
#include <iostream>
#include <fstream>

#include "Engine.h"
#include "Entity.h"
#include "Vertex.h"
#include "JRenderer.h"
#include "Scene.h"

Engine* Engine::m_spInstance = nullptr;

bool Engine::Init()
{
    m_hAppInst = nullptr;
    m_hMainWnd = nullptr;
    m_Minimized = false;
    m_Maximized = false;
    m_Resizing = false;
    m_ClientWidth = 800;
    m_ClientHeight = 600;

    m_MainWndCaption = L"Jahbal";

    m_Running = true;

	// Create the window
    if (!InitWindow())
        return false;

	// Instantiate the renderer and initialize it
    m_JRenderer = new JRenderer();
    if (!m_JRenderer->Init(m_ClientWidth, m_ClientHeight, m_hMainWnd))
        return false;

	// Scene is manually created/filled temporaraly
	// Eventually, the scene will be serialized for future loading or opened for editing in an edit mode
	m_ActiveScene = new Scene();

	/*
	Entity* entity = new Entity(m_JRenderer);

	std::vector<Vertex> vertexList(4);
	std::vector<int> indexList(6);

	vertexList[0].Position = { 1.0f, 1.0f, 0.0 };
	vertexList[1].Position = { 1.0f, -1.0f, 0.0 };
	vertexList[2].Position = { -1.0f, -1.0f, 0.0 };
	vertexList[3].Position = { -1.0f, 1.0f, 0.0 };

	indexList = { 0, 1, 2, 2, 3, 0 };

	entity->m_VisualComponent->CreateMesh(vertexList, indexList);
	entity->m_VisualComponent->CreateMaterial();
	m_ActiveScene->GetEntityList()->push_back(entity);
	*/

	Entity* entity = new Entity(m_JRenderer);

	std::ifstream fin("Models/skull.txt");

	if (!fin)
	{
		MessageBox(0, L"Models/skull.txt not found.", 0, 0);
		return false;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	std::vector<Vertex> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Position.x >> vertices[i].Position.y >> vertices[i].Position.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;
	}

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	std::vector<int> indices(3 * tcount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();

	entity->m_VisualComponent->CreateMesh(vertices, indices);
	entity->m_VisualComponent->CreateMaterial();
	m_ActiveScene->GetEntityList()->push_back(entity);
	

    return true;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Forward hwnd on because we can get messages (e.g., WM_CREATE)
    // before CreateWindow returns, and thus before m_hMainWnd is valid.
    return Engine::GetInstance()->MsgProc(hwnd, msg, wParam, lParam);
}

bool Engine::InitWindow()
{
    // boiler-plate win32 api window creation code

    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hAppInst;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = L"D3DWndClassName";

    if (!RegisterClass(&wc))
    {
        MessageBox(0, L"RegisterClass Failed.", 0, 0);
        return false;
    }

    // Compute window rectangle dimensions based on requested client area dimensions.
    RECT R = { 0, 0, m_ClientWidth, m_ClientHeight };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
    int width = R.right - R.left;
    int height = R.bottom - R.top;

    m_hMainWnd = CreateWindow(L"D3DWndClassName", m_MainWndCaption.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hAppInst, 0);
    if (!m_hMainWnd)
    {
        MessageBox(0, L"CreateWindow Failed.", 0, 0);
        return false;
    }

    ShowWindow(m_hMainWnd, SW_SHOW);
    UpdateWindow(m_hMainWnd);

    return true;
}


LRESULT Engine::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	case WM_QUIT:
		m_Running = false;
		break;

	// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


void Engine::Run()
{
    MSG msg = { 0 };

    while (m_Running)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            HandleEvents();
            Update();
            DrawScene(m_ActiveScene);
        }
    }

	ShutDown();
}

void Engine::HandleEvents()
{

}

void Engine::Update()
{

}

void Engine::DrawScene(Scene* scene)
{
	if(m_JRenderer)
		m_JRenderer->DrawScene(scene);
	else
	{
		printf("ERROR: Attempting to drawe scene with no renderer!");
		m_Running = false;
	}

}

void Engine::ShutDown()
{
	if(m_JRenderer)
		m_JRenderer->ShutDown();

	if (m_ActiveScene)
		m_ActiveScene->Shutdown();
}