#pragma once
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include "textureclass.h"
#include "hair.h"

class ModelClass
{
private:
	struct InstanceType {
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, WCHAR*, int vertexCount, int instances);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	void Update(float windValue, ID3D11Device* device);
	bool generateData(ID3D11Device*, int vertexCount, int instances);

	int GetVertexCount();
	int GetInstanceCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer, *m_instanceBuffer;
	int m_vertexCount, m_indexCount, m_instanceCount;
	TextureClass* m_Texture;

	VertexType* vertices;
	InstanceType* instances;
	Hair *hair1;
};

#endif