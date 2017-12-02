#include "modelclass.h"
#include <time.h>

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_instanceBuffer = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device, WCHAR* textureFilename, int vertexCount, int instances)
{
	bool result;


	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = generateData(device, vertexCount, instances);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{

	// Release the model texture.
	ReleaseTexture();

	// Release the vertex and index buffers.
	ShutdownBuffers();

	// Release the instance array now that the instance buffer has been created and loaded.
	delete[] instances;
	instances = 0;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	hair1->Shutdown();
	delete hair1;

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

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

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void ModelClass::Update(float windValue)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));



	hair1->Update(windValue);

	if (hair1->sections > 0) {
		for (int a = 0; a < m_vertexCount; a += 2) {
			if (a == 0) {
				vertices[0].position = hair1->positions[0];
				vertices[1].position = hair1->positions[1];
			}
			else {
				vertices[a].position = hair1->positions[a - 1];
				vertices[a + 1].position = hair1->positions[a + 1];
			}
		}

		hair1->UpdateInstances();
		for (int a = 0; a < m_instanceCount; a++) {
			instances[a].position = hair1->instancePositions[a];
		}
	}
}

bool ModelClass::generateData(ID3D11Device* device, int vertexCount, int instanceCount) {

	// Set the number of vertices in the vertex array.
	m_vertexCount = vertexCount;

	// Set the number of instanced in the instance array;
	//m_instanceCount = RAND_MAX;
	m_instanceCount = instanceCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the instance array
	instances = new InstanceType[m_instanceCount];
	if (!instances)
	{
		return false;
	}

	hair1 = new Hair(D3DXVECTOR3(0, 0, 100), 0.1f, vertexCount, instanceCount, 0.98f, 10);
	hair1->InitiateInstances(D3DXVECTOR3(50, 50, 50));
	Update(0);

	for (int a = 0; a < instanceCount; a++) {
		instances[a].position = hair1->instancePositions[a];
		instances[a].texture = D3DXVECTOR2(0, rand() % 100);
	}

	return true;
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

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{

	D3D11_BUFFER_DESC vertexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, instanceData;
	HRESULT result;


	srand((unsigned)time(NULL));


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances;
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

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];


	// Set the buffer strides.
	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST

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