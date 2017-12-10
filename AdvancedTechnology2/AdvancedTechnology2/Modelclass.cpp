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
	result = generateData(device, vertexCount, instances, position, baseColour);
	if (!result)
	{
		return false;
	}

	//result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	//result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

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

void ModelClass::Update(float windValue)
{
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	ShutdownBuffers();

	//hair1->Update(windValue);

	/*if (hair1->sections > 0) {
		for (int a = 0; a < m_vertexCount; a += 2) {
			if (a == 0) {
				verticesColor[0].position = hair1->positions[0];
				verticesColor[1].position = hair1->positions[1];
			}
			else {
				verticesColor[a].position = hair1->positions[a - 1];
				verticesColor[a + 1].position = hair1->positions[a + 1];
			}
		}

		hair1->UpdateInstances();
		for (int a = 0; a < m_instanceCount; a++) {
			instances[a].instancePosition = hair1->instancePositions[a];
		}
	}*/

	//InitializeBuffers(device);
}

bool ModelClass::generateData(ID3D11Device* device, int vertexCount, int instanceCount, D3DXVECTOR3 position, D3DXVECTOR4 baseColour) {

	// Set the number of vertices in the vertex array.
	m_vertexCount = vertexCount;

	// Set the number of instanced in the instance array;
	//m_instanceCount = RAND_MAX;
	m_instanceCount = instanceCount;

	// Create the vertex array.
	verticesColor = new VertexTypeColor[m_vertexCount];
	if (!verticesColor)
	{
		return false;
	}

	// Create the instance array
	instancesColor = new InstanceTypeColor[m_instanceCount];
	if (!instancesColor)
	{
		return false;
	}

	//hair1 = new Hair(position, 0.1f, vertexCount, instanceCount, 0.98f, 10);
	//hair1->InitiateInstances(D3DXVECTOR3(50, 50, 50));
	/*
	Update(0, device);

	D3DXVECTOR4 randColour;
	randColour = D3DXVECTOR4(randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.1f), randFloat(-0.2f, -0.1f));
	for (int a = 0; a < m_vertexCount; a += 2) {
		verticesColor[a].colour = baseColour + randColour;
		verticesColor[a + 1].colour = baseColour + randColour;
	}

	for (int a = 0; a < instanceCount; a++) {
		randColour = D3DXVECTOR4(randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.1f), randFloat(-0.2f, -0.1f));
		instances[a].instanceColour = baseColour + randColour;
		instances[a].instancePosition = hair1->instancePositions[a];
	}

	m_indexCount = (m_vertexCount - 1) * 2;
	indices = new unsigned int[m_indexCount];
	if (!indices)
	{
		return false;
	}

	for (int a = 0; a < m_indexCount; a += 2) {
		indices[a] = a;
		indices[a + 1] = a + 1;
	}
	*/


	return true;
}

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

/*
void ModelClass::RenderTexture(ID3D11DeviceContext* deviceContext)
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
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void ModelClass::RenderColor(ID3D11DeviceContext* deviceContext)
{

	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];


	// Set the buffer strides.
	strides[0] = sizeof(VertexTypeColor);
	strides[1] = sizeof(InstanceTypeColor);

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

	return;
}
*/

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