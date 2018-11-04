#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>

#include "DirectXTK/SimpleMath.h"
#include "Mesh.h"

struct MeshVertex;

class GeometryGenerator
{
public:

	static void CreatePlane(float width, float length, std::vector<MeshVertex>& v, std::vector<int>& i);
	static void CreateBox(float width, float height, float depth, std::vector<MeshVertex>& vertices, std::vector<int>&  indices);
};