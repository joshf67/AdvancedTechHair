#pragma once
#include "hair.h"

void Hair::ShutdownData(){
	releaseInstances();
}

bool Hair::Initialize(ID3D11Device* _device, D3DXVECTOR3 _position, D3DXVECTOR4 _baseColour, int _vertexCount, int _instances) {
	//setup basic variables
	startPosition = _position;
	baseColour = _baseColour;
	m_instanceCount = _instances;
	m_vertexCount = _vertexCount;
	m_indexCount = (m_vertexCount - 1) * 2;
	verticesColor = new VertexTypeColor[m_vertexCount];
	indices = new unsigned int[m_indexCount];
	device = _device;

	//setup line renderer/hair's indicies
	for (int a = 0; a < m_vertexCount - 1; a++) {
		indices[a * 2] = a;
		indices[(a * 2) + 1] = a + 1;
	}
	
	InitiateInstances(D3DXVECTOR3(50, 50, 50));

	Update(0);

	return true;
}

Hair::Hair() {
	//setup basic variables
	startPosition = D3DXVECTOR3(0, 0, 0);
	baseColour = D3DXVECTOR4(0, 0, 0, 0);
	m_instanceCount = 0;
	m_vertexCount = 0;
	m_indexCount = 0;
	indices = 0;
	vertices = 0;
	verticesColor = 0;
	instancesColor = 0;
	instances = 0;
	device = 0;
}

Hair::Hair(ID3D11Device* _device, D3DXVECTOR3 _position, D3DXVECTOR4 _baseColour, float _length, int _sections, int _instances, float _gravity, float _rigidity) {
	//setup basic variables
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

	//setup line renderer/hair's indicies
	for (int a = 0; a < m_vertexCount - 1; a++) {
		indices[a * 2] = a;
		indices[(a * 2) + 1] = a + 1;
	}

	//create instance hairs
	InitiateInstances(D3DXVECTOR3(50, 50, 50));

	Update(0);
}

Hair::Hair(D3DXVECTOR3 _position, float _sections, Hair* hair1, Hair* hair2) {
	//setup basic variables
	startPosition = _position;
	length = (hair1->length + hair2->length) / 2;
	m_vertexCount = _sections;
	verticesColor = new VertexTypeColor[m_vertexCount];
	gravity = (hair1->gravity + hair2->gravity) / 2;
	rigidity = (hair1->rigidity + hair2->rigidity) / 2;
	indices = new unsigned int[(m_vertexCount - 1) * 2];

	//setup line renderer/hair's indicies
	for (int a = 0; a < m_vertexCount; a++) {
		indices[a * 2] = a;
		indices[(a * 2) + 1] = a + 1;
	}

	Update(0);
}

void Hair::Update(float windValue) {

	//delete buffer data
	ShutdownBuffers();

	//update hair data based on wind value
	for (int a = 0; a < m_vertexCount; a++) {
		D3DXVECTOR3 position(0, 0, 0);
		D3DXVECTOR3 effect(0, 0, 0);

		//drop hair y by length and add windValue to x
		effect.y = length;
		effect.x += windValue;

		//check if rigidity has been set
		if (rigidity != 0) {
			//delay fall of hair by rigidity amount
			effect.z = -gravity * (1 - (rigidity / 100)) * a;
		}
		else {
			//make hair fall by gravity
			effect.z = -gravity * a;
		}

		//find the direct angle to new position locked to 1
		D3DXVec3Normalize(&effect, &effect);
		//make the strand's length to length
		effect *= length;

		//check if currently effected section is the start
		if (a == 0) {
			//apply effect to start position
			position = startPosition;
		}
		else {
			//apply effect to previous position
			position = verticesColor[a - 1].position + effect;
		}

		//set the position to the new position
		verticesColor[a].position = position;
		//set the colour to the base colour
		verticesColor[a].colour = baseColour;
	}

	//update all instance hairs
	UpdateInstances();

	InitializeVertexBuffer();
	InitializeIndexBuffer();
	InitializeInstanceBuffer();
}

void Hair::InitiateInstances(D3DXVECTOR3 _positionMaxOffset) {
	//create instances based on instance count
	instancesColor = new InstanceTypeColor[m_instanceCount];
	instanceOffset = new D3DXVECTOR3[m_instanceCount];

	//loop through all instances
	for (int a = 0; a < m_instanceCount; a++) {
		//apply a random position to the instance if it is not the original hair strand
		if (m_instanceCount != 1) {
			instanceOffset[a] = D3DXVECTOR3(randFloat(-_positionMaxOffset.x, _positionMaxOffset.x), randFloat(-_positionMaxOffset.y, _positionMaxOffset.y), randFloat(-_positionMaxOffset.z, _positionMaxOffset.z));
		}
		else {
			instanceOffset[0] = D3DXVECTOR3(0, 0, 0);
		}
	}
	//update all instance hairs
	UpdateInstances();
}

void Hair::UpdateInstances() {
	//loop through and apply changes to instance hairs
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

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	return;
}