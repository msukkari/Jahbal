#pragma once

#include <d3d11_1.h>
#include <d3d11shader.h>
#include <DirectXMath.h>

#include "dxerr.h"

class Scene;

class JRenderer
{
public:
    JRenderer() {};

    bool Init(int width, int height, HWND hMainWnd);

    void OnResize();
	void DrawScene(Scene* scene);

	void ShutDown();


	ID3D11Device* GetGFXDevice() { return m_d3dDevice; }
	ID3D11DeviceContext* GetGFXDeviceContext() { return m_d3dImmediateContext; }

private:
    bool InitDX11(HWND hMainWnd);

    ID3D11Device* m_d3dDevice;
    ID3D11DeviceContext* m_d3dImmediateContext;
    IDXGISwapChain* m_SwapChain;
    ID3D11Texture2D* m_DepthStencilBuffer;
    ID3D11RenderTargetView* m_RenderTargetView;
    ID3D11DepthStencilView* m_DepthStencilView;
    D3D11_VIEWPORT m_ScreenViewport;

	DirectX::XMFLOAT4X4 m_ProjectionMatrix;
	DirectX::XMFLOAT4X4 m_ViewMatrix;

    int m_ClientWidth;
    int m_ClientHeight;
    bool m_Enable4xMSAA;
	const DirectX::XMVECTORF32 m_ClearColor = { 0.2f, 0.2f, 0.2f, 1.0f };


    UINT m_4xMsaaQuality;
};

// DX Error Checking
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

