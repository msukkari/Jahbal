#include <d3d11_1.h>
#include <d3d11shader.h>
#include <stdlib.h>
#include <assert.h>
#include <DirectXMath.h>
#include <vector>

#include "JRenderer.h"
#include "Scene.h"
#include "Entity.h"
#include "VisualComponent.h"
#include "Material.h"
#include "Shader.h"
#include "Mesh.h"

using namespace DirectX;

bool JRenderer::Init(int width, int height, HWND hMainWnd)
{
    m_ClientWidth = width;
    m_ClientHeight = height;

	// Calculate projection matrix
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*(3.14f), (float)(m_ClientWidth) / m_ClientHeight, 1.0f, 1000.0f);
	XMStoreFloat4x4(&m_ProjectionMatrix, P);

	// Calculate view matrix
	// TODO: create camera class
	float x = -1.39f;
	float z = -1.92f;
	float y = 0.77f;

	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&m_ViewMatrix, V);
		
    if (!InitDX11(hMainWnd))
        return false;

    return true;
}

void JRenderer::DrawScene(Scene* scene)
{
	GetGFXDeviceContext()->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&m_ClearColor));
	GetGFXDeviceContext()->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	// Draw Scene
	for (int i = 0; i < scene->GetEntityList()->size(); i++)
	{
		Entity* entity = scene->GetEntityList()->at(i);

		GetGFXDeviceContext()->IASetInputLayout(entity->GetShader()->m_InputLayout);
		GetGFXDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		ID3DX11EffectTechnique* activeTech = entity->GetShader()->m_Tech;
		D3DX11_TECHNIQUE_DESC techDesc;
		activeTech->GetDesc(&techDesc);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		for (int p = 0; p < techDesc.Passes; p++)
		{
			GetGFXDeviceContext()->IASetVertexBuffers(0, 1, &entity->GetMesh()->m_VB, &stride, &offset);
			GetGFXDeviceContext()->IASetIndexBuffer(entity->GetMesh()->m_IB, DXGI_FORMAT_R32_UINT, 0);

			XMMATRIX model = XMMatrixIdentity();
			XMMATRIX view = XMLoadFloat4x4(&m_ViewMatrix);
			XMMATRIX projection = XMLoadFloat4x4(&m_ProjectionMatrix);
			XMMATRIX MVP = model * view * projection;

			entity->GetShader()->m_MVP->SetMatrix(reinterpret_cast<const float*>(&MVP));


			activeTech->GetPassByIndex(p)->Apply(0, GetGFXDeviceContext());
			GetGFXDeviceContext()->DrawIndexed(entity->GetMesh()->m_IndexCount, 0, 0);
		}
	}

	HR(m_SwapChain->Present(0, 0));
}



// Boiler-plate DX initialization
bool JRenderer::InitDX11(HWND hMainWnd)
{
    // Create the device and device context.

    UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDevice(
        0,                 // default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        0,                 // no software device
        createDeviceFlags,
        0, 0,              // default feature level array
        D3D11_SDK_VERSION,
        &m_d3dDevice,
        &featureLevel,
        &m_d3dImmediateContext);

    if (FAILED(hr))
    {
        MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
        return false;
    }

    if (featureLevel != D3D_FEATURE_LEVEL_11_0)
    {
        MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
        return false;
    }

    // Check 4X MSAA quality support for our back buffer format.
    // All Direct3D 11 capable devices support 4X MSAA for all render 
    // target formats, so we only need to check quality support.

    HR(m_d3dDevice->CheckMultisampleQualityLevels(
        DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality));
    assert(m_4xMsaaQuality > 0);

    // Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = m_ClientWidth;
    sd.BufferDesc.Height = m_ClientHeight;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // Use 4X MSAA? 
    if (m_Enable4xMSAA)
    {
        sd.SampleDesc.Count = 4;
        sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
    }
    // No MSAA
    else
    {
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
    }

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hMainWnd;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    IDXGIDevice* dxgiDevice = 0;
    HR(m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

    IDXGIAdapter* dxgiAdapter = 0;
    HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

    IDXGIFactory* dxgiFactory = 0;
    HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

    HR(dxgiFactory->CreateSwapChain(m_d3dDevice, &sd, &m_SwapChain));

    ReleaseCOM(dxgiDevice);
    ReleaseCOM(dxgiAdapter);
    ReleaseCOM(dxgiFactory);

    // The remaining steps that need to be carried out for d3d creation
    // also need to be executed every time the window is resized.  So
    // just call the OnResize method here to avoid code duplication.

    OnResize();

    return true;

}


//boiler-plate
void JRenderer::OnResize()
{
    assert(m_d3dImmediateContext);
    assert(m_d3dDevice);
    assert(m_SwapChain);

    // Release the old views, as they hold references to the buffers we
    // will be destroying.  Also release the old depth/stencil buffer.

    ReleaseCOM(m_RenderTargetView);
    ReleaseCOM(m_DepthStencilView);
    ReleaseCOM(m_DepthStencilBuffer);


    // Resize the swap chain and recreate the render target view.

    HR(m_SwapChain->ResizeBuffers(1, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
    ID3D11Texture2D* backBuffer;
    HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
    HR(m_d3dDevice->CreateRenderTargetView(backBuffer, 0, &m_RenderTargetView));
    ReleaseCOM(backBuffer);

    // Create the depth/stencil buffer and view.

    D3D11_TEXTURE2D_DESC depthStencilDesc;

    depthStencilDesc.Width = m_ClientWidth;
    depthStencilDesc.Height = m_ClientHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    // Use 4X MSAA? --must match swap chain MSAA values.
    if (m_Enable4xMSAA)
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
    }
    // No MSAA
    else
    {
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }

    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    HR(m_d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &m_DepthStencilBuffer));
    HR(m_d3dDevice->CreateDepthStencilView(m_DepthStencilBuffer, 0, &m_DepthStencilView));


    // Bind the render target view and depth/stencil view to the pipeline.

    m_d3dImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);


    // Set the viewport transform.

    m_ScreenViewport.TopLeftX = 0;
    m_ScreenViewport.TopLeftY = 0;
    m_ScreenViewport.Width = static_cast<float>(m_ClientWidth);
    m_ScreenViewport.Height = static_cast<float>(m_ClientHeight);
    m_ScreenViewport.MinDepth = 0.0f;
    m_ScreenViewport.MaxDepth = 1.0f;

    m_d3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);
}

void JRenderer::ShutDown()
{
	ReleaseCOM(m_RenderTargetView);
	ReleaseCOM(m_DepthStencilView);
	ReleaseCOM(m_SwapChain);
	ReleaseCOM(m_DepthStencilBuffer);

	// Restore all default settings.
	if (m_d3dImmediateContext)
		m_d3dImmediateContext->ClearState();

	ReleaseCOM(m_d3dImmediateContext);
	ReleaseCOM(m_d3dDevice);
}