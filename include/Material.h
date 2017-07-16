#pragma once

class Shader;


class Material
{
public:
	Material();
	~Material();

	Shader* m_Shader;
};

