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
#include "MeshVisual.h"
#include "BillboardVisual.h"
#include "TerrainVisual.h"
#include "Material.h"
#include "Shader.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "DirectXTK/SimpleMath.h"
#include "Camera.h" 
#include "ShaderManager.h"
#include "Shader.h"
#include "JGeneric.h"
#include "JBillboard.h"
#include "JQuadTess.h"
#include "JTerrain.h"
#include "Light.h"

using namespace DirectX;

bool JRenderer::Init(int width, int height, HWND hMainWnd)
{
    m_ClientWidth = width;
    m_ClientHeight = height;
	m_Enable4xMSAA = true;

	// Calculate projection matrix
	m_ProjectionMatrix = Matrix::CreatePerspectiveFieldOfViewLH(0.25f*(3.14f), (float)m_ClientWidth / (float)m_ClientHeight, 1.0f, 1000.0f);
		
    if (!InitDX11(hMainWnd))
        return false;

	m_blendStates = std::vector<ID3D11BlendState*>(BSSIZE);
	m_rasterizerStates = std::vector<ID3D11RasterizerState*>(RSSIZE);
	m_depthStencilStates = std::vector<ID3D11DepthStencilState*>(DSSIZE);
	InitRasterizerStates();
	InitBlendStates();
	InitDepthStencilStates();

    return true;
}

void JRenderer::DrawScene(Scene* scene)
{
	ID3D11DeviceContext* dc = GetGFXDeviceContext();

	dc->ClearRenderTargetView(m_renderTargetView, reinterpret_cast<const float*>(&m_ClearColor));
	dc->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	Camera* cam = scene->GetActiveCamera();
	Vector3 eyePos = Vector3(cam->m_position);
	Light* sun = nullptr;
	Light* point = nullptr;
	sun = scene->GetLightList()->at(0);
	if (scene->GetLightList()->size() >= 2)
	{
		point = scene->GetLightList()->at(1);
	}

	for (unsigned int i = 0; i < scene->GetEntityList()->size(); i++)
	{
		Entity* entity = scene->GetEntityList()->at(i);
		
		if (entity->m_VisualComponent->m_visualType == VisualType::MESH) DrawMeshEntity(entity, cam, sun, point);
		else if (entity->m_VisualComponent->m_visualType == VisualType::BILLBOARD) DrawBillboardEntity(entity, cam, sun, point);
		else if (entity->m_VisualComponent->m_visualType == VisualType::TERRAIN) DrawTerrainEntity(entity, cam);
	}

	HR(m_swapChain->Present(0, 0));
}

void JRenderer::DrawMeshEntity(Entity* entity, Camera* cam, Light* sun, Light* point)
{
	ID3D11DeviceContext* dc = GetGFXDeviceContext();
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Vector3 eyePos = Vector3(cam->m_position);

	dc->IASetInputLayout(ShaderManager::GetInstance()->m_JGeneric->m_InputLayout);
	ShaderManager::GetInstance()->m_JGeneric->SetDLight((DLightData*)sun->m_LightData);
	ShaderManager::GetInstance()->m_JGeneric->SetPLight((PLightData*)point->m_LightData);

	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f }; // only used with D3D11_BLEND_BLEND_FACTOR
	dc->RSSetState(m_rasterizerStates[RSSOLID]);
	dc->OMSetBlendState(m_blendStates[BSNOBLEND], blendFactors, 0xffffffff);
	dc->OMSetDepthStencilState(m_depthStencilStates[DSDEFAULT], 0);

	ID3DX11EffectTechnique* activeTech = ShaderManager::GetInstance()->m_JGeneric->Tech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	for (unsigned int p = 0; p < techDesc.Passes; p++)
	{
		Matrix rotation = Matrix::CreateFromYawPitchRoll(entity->m_rotationEuler.x, entity->m_rotationEuler.y, entity->m_rotationEuler.z);
		Matrix model = rotation * Matrix::CreateTranslation(entity->m_position);
		Matrix modelInvTranspose = model; modelInvTranspose.Invert().Transpose();
		Matrix view = cam->GetLookAtMatrix();
		Matrix MVP = model * view * m_ProjectionMatrix;

		ShaderManager::GetInstance()->m_JGeneric->SetWorldViewProj(MVP);
		ShaderManager::GetInstance()->m_JGeneric->SetWorld(model);
		ShaderManager::GetInstance()->m_JGeneric->SetWorldInvTranspose(modelInvTranspose);
		ShaderManager::GetInstance()->m_JGeneric->SetMaterial(entity->m_VisualComponent->m_Material);
		ShaderManager::GetInstance()->m_JGeneric->SetEyePosW(eyePos);

		MeshVisual* meshVisual = (MeshVisual*)entity->m_VisualComponent;
		Mesh* mesh = meshVisual->m_Mesh;
		for (unsigned int s = 0; s < mesh->m_subMeshList.size(); s++)
		{
			SubMesh* subMesh = &mesh->m_subMeshList[s];

			UINT stride = sizeof(MeshVertex);
			UINT offset = 0;
			GetGFXDeviceContext()->IASetVertexBuffers(0, 1, &subMesh->m_VB, &stride, &offset);
			GetGFXDeviceContext()->IASetIndexBuffer(subMesh->m_IB, DXGI_FORMAT_R32_UINT, 0);

			ShaderManager::GetInstance()->m_JGeneric->SetDiffuseMap(subMesh->m_diffuseSRV);
			ShaderManager::GetInstance()->m_JGeneric->SetSpecMap(subMesh->m_specSRV);

			activeTech->GetPassByIndex(p)->Apply(0, GetGFXDeviceContext());
			GetGFXDeviceContext()->DrawIndexed(subMesh->m_indexList.size(), 0, 0);
		}
	}

	dc->RSSetState(0);
	dc->OMSetBlendState(0, blendFactors, 0xffffffff);
	dc->OMSetDepthStencilState(0, 0);
}

void JRenderer::DrawBillboardEntity(Entity* entity, Camera* cam, Light* sun, Light* point)
{
	ID3D11DeviceContext* dc = GetGFXDeviceContext();
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	Vector3 eyePos = Vector3(cam->m_position);

	dc->IASetInputLayout(ShaderManager::GetInstance()->m_JBillboard->m_InputLayout);
	ShaderManager::GetInstance()->m_JBillboard->SetDLight((DLightData*)sun->m_LightData);
	ShaderManager::GetInstance()->m_JBillboard->SetPLight((PLightData*)point->m_LightData);

	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f }; // only used with D3D11_BLEND_BLEND_FACTOR
	dc->RSSetState(m_rasterizerStates[RSSOLID]);
	dc->OMSetBlendState(m_blendStates[BSALPHACOVERAGE], blendFactors, 0xffffffff);
	dc->OMSetDepthStencilState(m_depthStencilStates[DSDEFAULT], 0);

	ID3DX11EffectTechnique* activeTech = ShaderManager::GetInstance()->m_JBillboard->Tech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	for (unsigned int p = 0; p < techDesc.Passes; p++)
	{
		Matrix view = cam->GetLookAtMatrix();
		Matrix VP = view * m_ProjectionMatrix;

		ShaderManager::GetInstance()->m_JBillboard->SetViewProj(VP);
		ShaderManager::GetInstance()->m_JBillboard->SetMaterial(entity->m_VisualComponent->m_Material);
		ShaderManager::GetInstance()->m_JBillboard->SetEyePosW(eyePos);

		BillboardVisual* boardVisual = (BillboardVisual*)entity->m_VisualComponent;

		UINT stride = sizeof(BillBoardVertex);
		UINT offset = 0;
		GetGFXDeviceContext()->IASetVertexBuffers(0, 1, &boardVisual->m_VB, &stride, &offset);

		ShaderManager::GetInstance()->m_JBillboard->SetDiffuseMap(boardVisual->m_diffuseSRV);

		activeTech->GetPassByIndex(p)->Apply(0, GetGFXDeviceContext());
		GetGFXDeviceContext()->Draw(1, 0);
	}

	dc->RSSetState(0);
	dc->OMSetBlendState(0, blendFactors, 0xffffffff);
	dc->OMSetDepthStencilState(0, 0);
}

void JRenderer::DrawTerrainEntity(Entity* entity, Camera* cam)
{
	ID3D11DeviceContext* dc = GetGFXDeviceContext();
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	dc->IASetInputLayout(ShaderManager::GetInstance()->m_JTerrain->m_InputLayout);

	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f }; // only used with D3D11_BLEND_BLEND_FACTOR
	dc->RSSetState(m_rasterizerStates[RSWIREFRAME]);
	dc->OMSetBlendState(m_blendStates[BSNOBLEND], blendFactors, 0xffffffff);
	dc->OMSetDepthStencilState(m_depthStencilStates[DSDEFAULT], 0);

	TerrainVisual* terrainVisual = (TerrainVisual*)entity->m_VisualComponent;
	
	UINT stride = sizeof(TerrainVertex);
	UINT offset = 0;
	GetGFXDeviceContext()->IASetVertexBuffers(0, 1, &terrainVisual->m_VB, &stride, &offset);
	GetGFXDeviceContext()->IASetIndexBuffer(terrainVisual->m_IB, DXGI_FORMAT_R16_UINT, 0);

	Vector3 eyePos = Vector3(cam->m_position);
	Matrix view = cam->GetLookAtMatrix();
	Matrix VP = view * m_ProjectionMatrix;

	ShaderManager::GetInstance()->m_JTerrain->SetEyePosW(eyePos);
	ShaderManager::GetInstance()->m_JTerrain->SetViewProj(VP);

	ShaderManager::GetInstance()->m_JTerrain->SetHeightMap(terrainVisual->m_heightMapSRV);
	ShaderManager::GetInstance()->m_JTerrain->SetTessParams(Vector4(0, 1000, 0, 6));

	ID3DX11EffectTechnique* activeTech = ShaderManager::GetInstance()->m_JTerrain->Tech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	for (unsigned int p = 0; p < techDesc.Passes; p++)
	{
		activeTech->GetPassByIndex(p)->Apply(0, GetGFXDeviceContext());
		GetGFXDeviceContext()->DrawIndexed(terrainVisual->m_numPatchQuadFaces * 4, 0, 0);
	}

	dc->RSSetState(0);
	dc->OMSetBlendState(0, blendFactors, 0xffffffff);
	dc->OMSetDepthStencilState(0, 0);
}

void JRenderer::InitBlendStates()
{
	m_blendStates[BSNOBLEND] = nullptr;

	D3D11_BLEND_DESC alphaCoverageDesc = { 0 };
	alphaCoverageDesc.AlphaToCoverageEnable = true;
	alphaCoverageDesc.IndependentBlendEnable = false;
	alphaCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HR(m_d3dDevice->CreateBlendState(&alphaCoverageDesc, &m_blendStates[BSALPHACOVERAGE]));

	D3D11_BLEND_DESC noRenderTargetWritesDesc = { 0 };
	noRenderTargetWritesDesc.AlphaToCoverageEnable = false;
	noRenderTargetWritesDesc.IndependentBlendEnable = false;
	noRenderTargetWritesDesc.RenderTarget[0].BlendEnable = false;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].RenderTargetWriteMask = 0;
	HR(m_d3dDevice->CreateBlendState(&noRenderTargetWritesDesc, &m_blendStates[BSNOTARGETWRITE]));

	D3D11_BLEND_DESC alphaBlendDesc = { 0 };
	alphaBlendDesc.AlphaToCoverageEnable			= false;
	alphaBlendDesc.IndependentBlendEnable			= false;
	alphaBlendDesc.RenderTarget[0].BlendEnable		= true;
	alphaBlendDesc.RenderTarget[0].SrcBlend			= D3D11_BLEND_SRC_ALPHA;
	alphaBlendDesc.RenderTarget[0].DestBlend		= D3D11_BLEND_INV_SRC_ALPHA;
	alphaBlendDesc.RenderTarget[0].BlendOp			= D3D11_BLEND_OP_ADD;
	alphaBlendDesc.RenderTarget[0].SrcBlendAlpha	= D3D11_BLEND_ONE;
	alphaBlendDesc.RenderTarget[0].DestBlendAlpha	= D3D11_BLEND_ZERO;
	alphaBlendDesc.RenderTarget[0].BlendOpAlpha		= D3D11_BLEND_OP_ADD;
	alphaBlendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D11_COLOR_WRITE_ENABLE_ALL;
	HR(m_d3dDevice->CreateBlendState(&alphaBlendDesc, &m_blendStates[BSALPHA]));

}

void JRenderer::InitRasterizerStates()
{
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(m_d3dDevice->CreateRasterizerState(&wireframeDesc, &m_rasterizerStates[RSWIREFRAME]));

	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	HR(m_d3dDevice->CreateRasterizerState(&solidDesc, &m_rasterizerStates[RSSOLID]));

	D3D11_RASTERIZER_DESC solidBackDesc;
	ZeroMemory(&solidBackDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidBackDesc.FillMode = D3D11_FILL_SOLID;
	solidBackDesc.CullMode = D3D11_CULL_FRONT;
	solidBackDesc.FrontCounterClockwise = false;
	solidBackDesc.DepthClipEnable = true;

	HR(m_d3dDevice->CreateRasterizerState(&solidBackDesc, &m_rasterizerStates[RSSOLIDBACK]));
}

void JRenderer::InitDepthStencilStates()
{
	m_depthStencilStates[DSDEFAULT] = nullptr;

	D3D11_DEPTH_STENCIL_DESC markStencilDesc;
	markStencilDesc.DepthEnable = true;
	markStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	markStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	markStencilDesc.StencilEnable = true;
	markStencilDesc.StencilReadMask = 0xff;
	markStencilDesc.StencilWriteMask = 0xff;

	markStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	markStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	markStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	markStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	markStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	markStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	markStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	markStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HR(m_d3dDevice->CreateDepthStencilState(&markStencilDesc, &m_depthStencilStates[DSMARKSTENCIL]));

	D3D11_DEPTH_STENCIL_DESC stencilEqualDesc;
	stencilEqualDesc.DepthEnable = true; // having the depth test enabled might not actually be necessary here, since the stencil buffer marking pass already does a depth check
	stencilEqualDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	stencilEqualDesc.DepthFunc = D3D11_COMPARISON_LESS;
	stencilEqualDesc.StencilEnable = true;
	stencilEqualDesc.StencilReadMask = 0xff;
	stencilEqualDesc.StencilWriteMask = 0xff;

	stencilEqualDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	stencilEqualDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	stencilEqualDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	stencilEqualDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	stencilEqualDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	stencilEqualDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	stencilEqualDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	stencilEqualDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	HR(m_d3dDevice->CreateDepthStencilState(&stencilEqualDesc, &m_depthStencilStates[DSSTENCILEQUAL]));
}

void JRenderer::ShutDown()
{
	ReleaseCOM(m_renderTargetView);
	ReleaseCOM(m_depthStencilView);
	ReleaseCOM(m_swapChain);
	ReleaseCOM(m_depthStencilBuffer);


	for (int i = 0; i < m_rasterizerStates.size(); i++) ReleaseCOM(m_rasterizerStates[i]);
	for (int i = 0; i < m_blendStates.size(); i++) ReleaseCOM(m_blendStates[i]);

	// Restore all default settings.
	if (m_d3dImmediateContext)
		m_d3dImmediateContext->ClearState();

	ReleaseCOM(m_d3dImmediateContext);
	ReleaseCOM(m_d3dDevice);
}

//boiler-plate
void JRenderer::OnResize()
{
    assert(m_d3dImmediateContext);
    assert(m_d3dDevice);
    assert(m_swapChain);

    // Release the old views, as they hold references to the buffers we
    // will be destroying.  Also release the old depth/stencil buffer.

    ReleaseCOM(m_renderTargetView);
    ReleaseCOM(m_depthStencilView);
    ReleaseCOM(m_depthStencilBuffer);


    // Resize the swap chain and recreate the render target view.

    HR(m_swapChain->ResizeBuffers(1, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
    ID3D11Texture2D* backBuffer;
    HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
    HR(m_d3dDevice->CreateRenderTargetView(backBuffer, 0, &m_renderTargetView));
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

    HR(m_d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &m_depthStencilBuffer));
    HR(m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer, 0, &m_depthStencilView));


    // Bind the render target view and depth/stencil view to the pipeline.

    m_d3dImmediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);


    // Set the viewport transform.

    m_screenViewport.TopLeftX = 0;
    m_screenViewport.TopLeftY = 0;
    m_screenViewport.Width = static_cast<float>(m_ClientWidth);
    m_screenViewport.Height = static_cast<float>(m_ClientHeight);
    m_screenViewport.MinDepth = 0.0f;
    m_screenViewport.MaxDepth = 1.0f;

    m_d3dImmediateContext->RSSetViewports(1, &m_screenViewport);
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

    HR(dxgiFactory->CreateSwapChain(m_d3dDevice, &sd, &m_swapChain));

    ReleaseCOM(dxgiDevice);
    ReleaseCOM(dxgiAdapter);
    ReleaseCOM(dxgiFactory);

    // The remaining steps that need to be carried out for d3d creation
    // also need to be executed every time the window is resized.  So
    // just call the OnResize method here to avoid code duplication.

    OnResize();

    return true;

}