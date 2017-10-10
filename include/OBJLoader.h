#pragma once

#include <vector>
#include <string>

#include "Vertex.h"

class OBJLoader
{
public:
	static void loadOBJ(std::string filename, std::vector<Vertex> &vertexList, std::vector<int> &indexList);
};