#pragma once

#include "d3dx11effect.h"

class VisualComponent;

class Shader
{
public:
	Shader(VisualComponent* owner);
	~Shader();

	void InitFX();
	void InitInputLayout();

	ID3DX11Effect* m_FX;
	ID3DX11EffectTechnique* m_Tech;
	ID3D11InputLayout* m_InputLayout;

	ID3DX11EffectMatrixVariable* m_MVP;

	VisualComponent* m_ComponentOwner;

};

