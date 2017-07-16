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

private:
	ID3DX11Effect* m_FX;
	ID3DX11EffectTechnique* m_Tech;
	ID3D11InputLayout* m_InputLayout;

	VisualComponent* m_ComponentOwner;

};

