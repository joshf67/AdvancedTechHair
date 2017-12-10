#pragma once
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include "textureclass.h"
#include "Colorshaderclass.h"

class ModelClass
{
protected:
	struct InstanceTypeColor {
		D3DXVECTOR3 instancePosition;
		D3DXVECTOR4 instanceColour;
	};
	struct InstanceType {
		D3DXVECTOR3 instancePosition;
	};
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
	struct VertexTypeColor
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 colour;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	virtual bool Initialize(ID3D11Device*, WCHAR*, int vertexCount, int instances, D3DXVECTOR3 position, D3DXVECTOR4 baseColour);
	virtual bool Initialize(ID3D11Device* _device, D3DXVECTOR3 _position, D3DXVECTOR4 _baseColour, int _instances);
	virtual bool Initialize(ID3D11Device* _device, D3DXVECTOR3 _position, D3DXVECTOR4 _baseColour, int vertexCount, int _instances);
	void Shutdown();
	virtual void ShutdownData();
	//void Render(ID3D11DeviceContext*);
	virtual void Render(ID3D11DeviceContext*);

	virtual void Update(float windValue);
	bool generateData(ID3D11Device*, int vertexCount, int instances, D3DXVECTOR3 position, D3DXVECTOR4 baseColour);
	float randFloat(float a, float b);

	int GetVertexCount();
	int GetInstanceCount();
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

protected:
	bool InitializeVertexBuffer();
	bool InitializeIndexBuffer();
	bool InitializeInstanceBuffer();
	void ShutdownBuffers();

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

protected:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer, *m_instanceBuffer;
	int m_vertexCount, m_indexCount, m_instanceCount;
	D3DXVECTOR3 basePos;
	TextureClass* m_Texture;
	ColorShaderClass* m_colour;

	VertexType* vertices;
	VertexTypeColor* verticesColor;
	InstanceType* instances;
	InstanceTypeColor* instancesColor;
	unsigned int* indices;
	ID3D11Device* device;
	//Hair *hair1;
};

#endif