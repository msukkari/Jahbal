#pragma once

#include <d3d11.h>


class JGeneric;
class JBillboard;
class JQuadTess;

class ShaderManager
{
public:
	void Init(ID3D11Device* device);
	void ShutDown();

	JGeneric* m_JGeneric;
	JBillboard* m_JBillboard;
	JQuadTess* m_JQuadTess;

	static ShaderManager* GetInstance()
	{
		if (m_pInstance == nullptr) m_pInstance = new ShaderManager();

		return m_pInstance;
	}

private:
	static ShaderManager* m_pInstance;
};