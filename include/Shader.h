#pragma once

#include "d3dx11effect.h"

class Shader
{
public:
	Shader();
	~Shader();


private:
	ID3DX11Effect* m_FX;
	ID3DX11EffectTechnique* m_Tech;
};

