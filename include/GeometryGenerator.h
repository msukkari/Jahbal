#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>

#include "SimpleMath.h"
#include "Mesh.h"

using namespace DirectX;

class GeometryGenerator
{
public:

	static void CreateBox(float width, float height, float depth, std::vector<Vertex>& vertices, std::vector<int>&  indices);
};