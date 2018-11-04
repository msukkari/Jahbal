#pragma once

#include "Shader.h"
#include "DirectXTK\SimpleMath.h"

class JTerrain : public Shader
{
public:
	JTerrain(ID3D11Device* device);
	~JTerrain();

	VP_DEC
	EYEPOS_DEC
	TPARAMS_DEC

	HMAP_DEC
};