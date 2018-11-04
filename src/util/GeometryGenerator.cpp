#include "util/GeometryGenerator.h"
#include "core/JRenderer.h"
#include "components/MeshVisual.h"

void GeometryGenerator::CreatePlane(float width, float length, std::vector<MeshVertex>& v, std::vector<int>& i)
{
	v.clear();
	v.resize(4);

	float w2 = 0.5f * width;
	float l2 = 0.5f * length;

	v[0] = MeshVertex(w2, 0.0f, l2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[1] = MeshVertex(w2, 0.0f, -l2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	v[2] = MeshVertex(-w2, 0.0f, -l2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[3] = MeshVertex(-w2, 0.0f, l2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);

	i.clear();
	i.resize(6);

	i[0] = 0;
	i[1] = 1;
	i[2] = 2;
	i[3] = 2;
	i[4] = 3;
	i[5] = 0;
}

void GeometryGenerator::CreateBox(float width, float height, float depth, std::vector<MeshVertex>& v, std::vector<int>&  i)
{
	v.clear();
	v.resize(24);

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;

	// Fill in the front face vertex data.
	v[0] = MeshVertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = MeshVertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = MeshVertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[3] = MeshVertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = MeshVertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[5] = MeshVertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[6] = MeshVertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[7] = MeshVertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8] = MeshVertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[9] = MeshVertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[10] = MeshVertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[11] = MeshVertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = MeshVertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f,  1.0f, 1.0f);
	v[13] = MeshVertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f,  0.0f, 1.0f);
	v[14] = MeshVertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f);
	v[15] = MeshVertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f,  1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = MeshVertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[17] = MeshVertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[18] = MeshVertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[19] = MeshVertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = MeshVertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[21] = MeshVertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[22] = MeshVertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[23] = MeshVertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);


	i.clear();
	i.resize(36);

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;
}