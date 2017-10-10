#include <stdio.h>
#include <fstream>
#include <sstream>
#include <d3d11.h>

#include "OBJLoader.h"
#include "SimpleMath.h"

using namespace DirectX;

void OBJLoader::loadOBJ(std::string filename, std::vector<Vertex> &vertexList, std::vector<int> &indexList)
{
	std::ifstream file;
	file.open(filename);

	std::vector<Vector3> pos;
	std::vector<Vector3> norms;

	std::string line;
	while (getline(file, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			std::istringstream s(line.substr(2));

			Vector3 v; s >> v.x >> v.y >> v.z; 
			pos.push_back(v);
		}
		else if (line.substr(0, 3) == "vn ")
		{
			std::istringstream s(line.substr(3));

			Vector3 n; s >> n.x >> n.y >> n.z; 
			pos.push_back(n);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::istringstream s(line.substr(2));

			int a, b, c;
			s >> a >> b >> c;
			a--; b--; c--;
			indexList.push_back(a); indexList.push_back(b); indexList.push_back(c);
		}
	}


	for (int i = 0; i < pos.size(); i++)
	{
		Vertex v;
		v.Position = pos[i];
		v.Normal = norms[i];

		vertexList.push_back(v);
	}
}