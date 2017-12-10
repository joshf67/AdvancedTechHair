#pragma once
#include "hair.h"

void Hair::ShutdownData(){
	releaseInstances();
}

bool Hair::Initialize(ID3D11Device* _device, D3DXVECTOR3 _position, D3DXVECTOR4 _baseColour, int _vertexCount, int _instances) {
	startPosition = _position;
	baseColour = _baseColour;
	m_instanceCount = _instances;
	m_vertexCount = _vertexCount;
	m_indexCount = (m_vertexCount - 1) * 2;
	verticesColor = new VertexTypeColor[m_vertexCount];
	indices = new unsigned int[m_indexCount];
	device = _device;

	for (int a = 0; a < m_vertexCount - 1; a++) {
		indices[a * 2] = a;
		indices[(a * 2) + 1] = a + 1;
	}
	
	InitiateInstances(D3DXVECTOR3(50, 50, 50));

	Update(0);

	return true;
}

Hair::Hair(ID3D11Device* _device, D3DXVECTOR3 _position, D3DXVECTOR4 _baseColour, float _length, int _sections, int _instances, float _gravity, float _rigidity) {
	startPosition = _position;
	baseColour = _baseColour;
	m_instanceCount = _instances;
	m_vertexCount = _sections;
	m_indexCount = (m_vertexCount - 1) * 2;
	length = _length;
	verticesColor = new VertexTypeColor[m_vertexCount];
	gravity = _gravity;
	rigidity = _rigidity;
	indices = new unsigned int[m_indexCount];
	device = _device;


	for (int a = 0; a < m_vertexCount - 1; a++) {
		indices[a * 2] = a;
		indices[(a * 2) + 1] = a + 1;
	}

	InitiateInstances(D3DXVECTOR3(50, 50, 50));

	Update(0);
}

Hair::Hair(D3DXVECTOR3 _position, float _sections, Hair* hair1, Hair* hair2) {
	startPosition = _position;
	length = (hair1->length + hair2->length) / 2;
	m_vertexCount = _sections;
	verticesColor = new VertexTypeColor[m_vertexCount];
	gravity = (hair1->gravity + hair2->gravity) / 2;
	rigidity = (hair1->rigidity + hair2->rigidity) / 2;
	indices = new unsigned int[(m_vertexCount - 1) * 2];

	for (int a = 0; a < m_vertexCount; a++) {
		indices[a * 2] = a;
		indices[(a * 2) + 1] = a + 1;
	}

	Update(0);
}

void Hair::Update(float windValue) {

	ShutdownBuffers();

	for (int a = 0; a < m_vertexCount; a++) {
		D3DXVECTOR3 position(0, 0, 0);
		D3DXVECTOR3 effect(0, 0, 0);
		effect.x = length;
		effect.x += windValue;
		if (rigidity != 0) {
			effect.y = -gravity * (1 - (rigidity / 100)) * a;
		}
		else {
			effect.y = -gravity * a;
		}
		D3DXVec3Normalize(&effect, &effect);
		effect *= length;
		if (a == 0) {
			position = startPosition + effect;
		}
		else {
			position = verticesColor[a - 1].position + effect;
		}
		verticesColor[a].position = position;
		verticesColor[a].colour = baseColour;
	}
	UpdateInstances();

	InitializeVertexBuffer();
	InitializeIndexBuffer();
	InitializeInstanceBuffer();
}

void Hair::InitiateInstances(D3DXVECTOR3 _positionMaxOffset) {
	instancesColor = new InstanceTypeColor[m_instanceCount];
	instanceOffset = new D3DXVECTOR3[m_instanceCount];
	for (int a = 0; a < m_instanceCount; a++) {
		instanceOffset[a] = D3DXVECTOR3(randFloat(-_positionMaxOffset.x, _positionMaxOffset.x), randFloat(-_positionMaxOffset.y, _positionMaxOffset.y), randFloat(-_positionMaxOffset.z, _positionMaxOffset.z));
	}
}

void Hair::UpdateInstances() {
	for (int a = 0; a < m_instanceCount; a++) {
		instancesColor[a].instancePosition = verticesColor[0].position + instanceOffset[a];
		instancesColor[a].instanceColour = D3DXVECTOR4(0,0,0,0);
	}
}

void Hair::releaseInstances() {
	delete[] instanceOffset;
	instanceOffset = 0;
}

void Hair::Render(ID3D11DeviceContext* deviceContext)
{

	//unsigned int strides[3];
	//unsigned int offsets[3];
	//ID3D11Buffer* bufferPointers[3];
	
	
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];
	

	// Set the buffer strides.
	strides[0] = sizeof(VertexTypeColor);
	strides[1] = sizeof(InstanceTypeColor);
	//strides[2] = sizeof(unsigned long);

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;
	//offsets[2] = 0;

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;
	//bufferPointers[2] = m_indexBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	return;
}