#include "modelclass.h"
#include <time.h>

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_instanceBuffer = 0;
	m_colour = 0;
	indices = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* _device, WCHAR* textureFilename, int vertexCount, int instances, D3DXVECTOR3 position, D3DXVECTOR4 baseColour)
{
	bool result;

	device = _device;
	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	basePos = position;

	return true;
}

bool ModelClass::Initialize(ID3D11Device * _device, D3DXVECTOR3 _position, D3DXVECTOR4 _baseColour, int _instances)
{
	return false;
}

bool ModelClass::Initialize(ID3D11Device * _device, D3DXVECTOR3 _position, D3DXVECTOR4 _baseColour, int vertexCount, int _instances)
{
	return false;
}

void ModelClass::Shutdown()
{

	// Release the model texture.
	ReleaseTexture();

	// Release the vertex and index buffers.
	ShutdownBuffers();

	// Release the instance array now that the instance buffer has been created and loaded.
	if (instances) {
		delete[] instances;
		instances = 0;
	}

	if (instancesColor) {
		delete[] instancesColor;
		instancesColor = 0;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	if (vertices) {
		delete[] vertices;
		vertices = 0;
	}
	
	// Release indices
	if (indices) {
		delete[] indices;
		indices = 0;
	}

	if (verticesColor) {
		delete[] verticesColor;
		verticesColor = 0;
	}

	//hair1->Shutdown();
	//delete hair1;

	return;
}

void ModelClass::ShutdownData() {
	//Use for data created in inheritance
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//RenderTexture(deviceContext);
	//RenderColor(deviceContext);

	return;
}

int ModelClass::GetVertexCount()
{
	return m_vertexCount;
}


int ModelClass::GetInstanceCount()
{
	return m_instanceCount;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void ModelClass::Update(float windValue) {}

//return a random float between a -> b 
float ModelClass::randFloat(float a, float b) {
	return ((b - a) *  ((float)rand() / RAND_MAX)) + a;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::InitializeVertexBuffer() {
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	HRESULT result;

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	if (vertices) {
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
		vertexData.pSysMem = vertices;
	}
	else if (verticesColor) {
		vertexBufferDesc.ByteWidth = sizeof(VertexTypeColor) * m_vertexCount;
		vertexData.pSysMem = verticesColor;
	}
	else {
		return false;
	}
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;

}

bool ModelClass::InitializeIndexBuffer() {
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;

}

bool ModelClass::InitializeInstanceBuffer() {
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;
	HRESULT result;

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	if (instances) {
		instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
		instanceData.pSysMem = instances;
	}
	else if (instancesColor) {
		instanceBufferDesc.ByteWidth = sizeof(InstanceTypeColor) * m_instanceCount;
		instanceData.pSysMem = instancesColor;
	}
	else {
		return false;
	}
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// Release the instance buffer.
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	return;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}