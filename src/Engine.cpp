#include <windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>
#include <DirectXMath.h>
#include <chrono>

#include "Engine.h"
#include "BaseEntity.h"
#include "Entity.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "JRenderer.h"
#include "Scene.h"
#include "Camera.h"
#include "DirectXTK/SimpleMath.h"
#include "DirectXTK\WICTextureLoader.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "JGeneric.h"
#include "GeometryGenerator.h"
#include "InputManager.h"
#include "MeshVisual.h"
#include "BillboardVisual.h"
#include "TerrainVisual.h"

Engine* Engine::m_spInstance = nullptr;

#define getCurTime() std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;

bool Engine::Init()
{
    m_hAppInst = nullptr;
    m_hMainWnd = nullptr;
    m_Minimized = false;
    m_Maximized = false;
    m_Resizing = false;
    m_ClientWidth = 1280;
    m_ClientHeight = 720;

    m_MainWndCaption = L"Jahbal";

    m_Running = true;

	// Create the window
    if (!InitWindow())
        return false;

	// Instantiate the renderer and initialize it
    m_JRenderer = new JRenderer();
    if (!m_JRenderer->Init(m_ClientWidth, m_ClientHeight, m_hMainWnd))
        return false;

	ShaderManager::GetInstance()->Init(m_JRenderer->GetGFXDevice());

	InputManager::GetInstance()->Init(m_hMainWnd);
	
	// Scene is manually created/filled temporaraly
	// Eventually, the scene will be serialized for future loading or opened for editing in an edit mode
	{

		m_ActiveScene = new Scene();

		Material* material = new Material();
		material->Ambient = Vector4((181.0f / 255.0f), (50.0f / 255.0f), (43.0f / 255.0f), 1.0f);
		material->Diffuse = Vector4((181.0f / 255.0f), (50.0f / 255.0f), (43.0f / 255.0f), 1.0f);
		material->Specular = Vector4((181.0f / 255.0f), (50.0f / 255.0f), (43.0f / 255.0f), 1.0f);

		Mesh* nanosuitMesh = new Mesh(NULL, "resources/objects/nanosuit/nanosuit.obj");

		for (unsigned int i = 0; i < 1; i++)
		{
			for (int j = 0; j < 1; j++)
			{
				Entity* e = new Entity(m_JRenderer, Vector3(i * 3.0f, -6.0f, j * 3.0f));
				e->m_VisualComponent = new MeshVisual(e, m_JRenderer);
				MeshVisual* meshVisual = (MeshVisual*)e->m_VisualComponent;
				meshVisual->m_Mesh = nanosuitMesh;
				meshVisual->m_Mesh->m_componentOwner = meshVisual;
				e->m_VisualComponent->m_Material = material;
				m_ActiveScene->GetEntityList()->push_back(e);
			}
		}

		Mesh* planeMesh;
		{
			std::vector<MeshVertex> planeV;
			std::vector<int> planeI;
			GeometryGenerator::CreatePlane(40.0f, 40.0f, planeV, planeI);
			planeMesh = new Mesh(nullptr, planeV, planeI);
			ID3D11Resource* texResource = nullptr;
			ID3D11ShaderResourceView* srv;
			std::string path = "resources/textures/brickwall.jpg";
			std::wstring wc = std::wstring(path.begin(), path.end());
			DirectX::CreateWICTextureFromFile(
				Engine::GetInstance()->GetRenderer()->GetGFXDevice(),
				Engine::GetInstance()->GetRenderer()->GetGFXDeviceContext(),
				wc.c_str(),
				&texResource, &srv);
			ReleaseCOM(texResource);

			planeMesh->m_subMeshList[0].m_diffuseSRV = srv;
			//planeMesh->m_subMeshList[0].m_specSRV = nullptr;
		}

		// load tree billboard texture
		ID3D11ShaderResourceView* treeSRV;
		ID3D11Resource* texResource = nullptr;
		DirectX::CreateWICTextureFromFile(
			Engine::GetInstance()->GetRenderer()->GetGFXDevice(),
			Engine::GetInstance()->GetRenderer()->GetGFXDeviceContext(),
			L"resources/textures/tree0.dds",
			&texResource, &treeSRV);
		ReleaseCOM(texResource);

		Entity* plane = new Entity(m_JRenderer, Vector3(0.0f, -6.0f, 0.0f));
		MeshVisual* plane1MeshVisual = new MeshVisual(plane, m_JRenderer);
		plane1MeshVisual->m_Mesh = planeMesh;
		plane1MeshVisual->m_Material = material;
		plane->m_VisualComponent = plane1MeshVisual;

		Entity* plane2 = new Entity(m_JRenderer, Vector3(22.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, M_PI / 2));
		MeshVisual* plane2MeshVisual = new MeshVisual(plane2, m_JRenderer);
		plane2MeshVisual->m_Mesh = planeMesh;
		plane2MeshVisual->m_Material = material;
		plane2->m_VisualComponent = plane2MeshVisual;

		m_ActiveScene->GetEntityList()->push_back(plane);
		m_ActiveScene->GetEntityList()->push_back(plane2);

		srand((unsigned int)time(NULL));
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				Entity* board = new Entity(m_JRenderer, Vector3(float(rand() % 40), 0.0f, 20.0f + (rand() % 40)), Vector3(0.0f, 0.0f, 0.0f));
				BillboardVisual* boardVisual = new BillboardVisual(board, m_JRenderer, 10.0f, 10.0f);
				boardVisual->m_diffuseSRV = treeSRV;
				board->m_VisualComponent = boardVisual;
				m_ActiveScene->GetEntityList()->push_back(board);
			}
		}

		/*
		Entity* quadPatch = new Entity(m_JRenderer, Vector3(0.0f, -6.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
		TerrainVisual* terrainVisual = new TerrainVisual(quadPatch, m_JRenderer, 1000.0f, 1000.0f);
		quadPatch->m_VisualComponent = terrainVisual;
		m_ActiveScene->GetEntityList()->push_back(quadPatch);
		*/

		Entity* terrain = new Entity(m_JRenderer, Vector3(0.0f, -6.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));

		TerrainInfo terrainInfo(L"resources/textures/terrain.raw", 2049, 2049, 50, 1);
		TerrainVisual* terrainVisual = new TerrainVisual(terrain, m_JRenderer, terrainInfo);
		terrain->m_VisualComponent = terrainVisual;
		m_ActiveScene->GetEntityList()->push_back(terrain);

		// Camera
		Camera* camera = new Camera(50.0f);

		m_ActiveScene->SetActiveCamera(camera);

		// Directional light
		Light* sun = new Light(Directional);
		DLightData* sunData = new DLightData();
		sunData->Ambient = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
		sunData->Diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		sunData->Specular = Vector4(0.05f, 0.05f, 0.05f, 16.0f);
		sunData->Direction = Vector3(1.0f, 0.0f, 0.0f);
		sun->m_LightData = sunData;

		// Point light
		Light* point = new Light(Point);
		PLightData* pData = new PLightData();
		pData->Ambient = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
		pData->Diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		pData->Specular = Vector4(0.02f, 0.02f, 0.02f, 16.0f);
		pData->Position = Vector3(10.0f, 0.0f, 0.0f);
		pData->Range = 50.0f;
		pData->Att = Vector3(0.0f, 0.1f, 0.0f);
		point->m_LightData = pData;

		m_ActiveScene->GetLightList()->push_back(sun);
		m_ActiveScene->GetLightList()->push_back(point);
	}

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
	case WM_ACTIVATEAPP:
		DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
		DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
		break;
	}
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


void Engine::Run()
{
    MSG msg = { 0 };

	__int64 currentTime = getCurTime(), newTime = getCurTime(), frameTime;
		
	double fpsCounter = 0;
	double frameCount = 0;

    while (m_Running)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		else
		{
			newTime = getCurTime();

			frameTime = newTime - currentTime;
			currentTime = newTime;
			fpsCounter += frameTime;

			if (fpsCounter >= 1000)
			{
				double fps = frameCount / (fpsCounter / 1000.0);

				std::string title = "Jahbal - " + std::to_string(fps);
				SetWindowText(m_hMainWnd, std::wstring(title.begin(), title.end()).c_str());

				frameCount = 0;
				fpsCounter = 0;
			}


            HandleEvents();
            Update(frameTime);
            DrawScene(m_ActiveScene);

			frameCount++;
        }
    }

	ShutDown();
}

void Engine::HandleEvents()
{
	auto kb = InputManager::GetInstance()->m_Keyboard->GetState();
	if (kb.Escape) m_Running = false;
}

void Engine::Update(__int64 dt_msec)
{
	m_ActiveScene->GetActiveCamera()->Update(dt_msec);
	for (unsigned int i = 0; i < m_ActiveScene->GetEntityList()->size(); i++)
	{
		m_ActiveScene->GetEntityList()->at(i)->Update(dt_msec);
	}
}

void Engine::DrawScene(Scene* scene)
{
	if(m_JRenderer)
		m_JRenderer->DrawScene(scene);
	else
	{
		printf("ERROR: Attempting to draw scene with no renderer!");
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

