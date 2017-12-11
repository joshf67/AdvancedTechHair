#pragma once
#include "bear.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

void Bear::ShutdownData(){
	releaseInstances();
}

bool Bear::Initialize(ID3D11Device* _device, D3DXVECTOR3 _position, D3DXVECTOR4 _baseColour, int _instances) {
	startPosition = _position;
	baseColour = _baseColour;
	m_instanceCount = _instances;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile("../AdvancedTechnology2/testBear.FBX",
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene) {
		exit(0);
	}

	m_vertexCount = scene->mMeshes[0]->mNumVertices;

	verticesColor = new VertexTypeColor[m_vertexCount];

	for (int a = 0; a < m_vertexCount; a++) {
		verticesColor[a].position = startPosition + D3DXVECTOR3(scene->mMeshes[0]->mVertices[a].x, scene->mMeshes[0]->mVertices[a].y, scene->mMeshes[0]->mVertices[a].z);
		verticesColor[a].colour = _baseColour;
	}

	device = _device;

	//index setup for wireframe
	/*
	m_indexCount = scene->mMeshes[0]->mNumFaces * 6;

	indices = new unsigned int[m_indexCount];

	for (int a = 0; a < m_indexCount / 6; a++) {
	for (int b = 0; b < 3; b++) {
	for (int c = 0; c < 2; c++) {
	if (b != 2) {
	indices[((a * 3) + (b * 2) + c)] = scene->mMeshes[0]->mFaces[a].mIndices[b + c];
	}
	else {
	if (c == 1) {
	indices[((a * 3) + (b * 2) + c)] = scene->mMeshes[0]->mFaces[a].mIndices[0];
	}
	else {
	indices[((a * 3) + (b * 2) + c)] = scene->mMeshes[0]->mFaces[a].mIndices[b];
	}
	}
	}
	//indices[((a * 3) + b)] = scene->mMeshes[0]->mFaces[a].mIndices[b];
	}
	}
	*/


	//index setup for model

	m_indexCount = scene->mMeshes[0]->mNumFaces * 3;

	indices = new unsigned int[m_indexCount];

	for (int a = 0; a < m_indexCount / 3; a++) {
		for (int b = 0; b < 3; b++) {
			indices[((a * 3) + b)] = scene->mMeshes[0]->mFaces[a].mIndices[b];
		}
	}


	InitiateInstances(D3DXVECTOR3(50, 50, 50));

	Update(0);

	ShutdownBuffers();

	InitializeVertexBuffer();
	InitializeInstanceBuffer();
	InitializeIndexBuffer();

	return true;
}

Bear::Bear(ID3D11Device* _device, D3DXVECTOR3 _position, D3DXVECTOR4 _baseColour, int _instances) {
	startPosition = _position;
	baseColour = _baseColour;
	m_instanceCount = _instances;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile("../AdvancedTechnology2/testBear.FBX",
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene) {
		exit(0);
	}

	m_vertexCount = scene->mMeshes[0]->mNumVertices;

	verticesColor = new VertexTypeColor[m_vertexCount];

	for (int a = 0; a < m_vertexCount; a++) {
		verticesColor[a].position = D3DXVECTOR3(scene->mMeshes[0]->mVertices[a].x, scene->mMeshes[0]->mVertices[a].y, scene->mMeshes[0]->mVertices[a].z);
		verticesColor[a].colour = _baseColour + D3DXVECTOR4(randFloat(-1, 0), randFloat(-1, 0), randFloat(-1, 0), randFloat(-1, 0));
	}

	//index setup for wireframe
	/*
	m_indexCount = scene->mMeshes[0]->mNumFaces * 6;

	indices = new unsigned int[m_indexCount];

	for (int a = 0; a < m_indexCount / 6; a++) {
		for (int b = 0; b < 3; b++) {
			for (int c = 0; c < 2; c++) {
				if (b != 2) {
					indices[((a * 3) + (b * 2) + c)] = scene->mMeshes[0]->mFaces[a].mIndices[b + c];
				}
				else {
					if (c == 1) {
						indices[((a * 3) + (b * 2) + c)] = scene->mMeshes[0]->mFaces[a].mIndices[0];
					}
					else {
						indices[((a * 3) + (b * 2) + c)] = scene->mMeshes[0]->mFaces[a].mIndices[b];
					}
				}
			}
			//indices[((a * 3) + b)] = scene->mMeshes[0]->mFaces[a].mIndices[b];
		}
	}
	*/


	//index setup for model
	
	m_indexCount = scene->mMeshes[0]->mNumFaces * 3;

	indices = new unsigned int[m_indexCount];

	for (int a = 0; a < m_indexCount / 3; a++) {
		for (int b = 0; b < 3; b++) {
			indices[((a * 3) + b)] = scene->mMeshes[0]->mFaces[a].mIndices[b];
		}
	}
	

	//manual square tesselation test
	m_hairCount = scene->mMeshes[0]->mNumFaces;
	hairs = new hairStruct[m_hairCount];

	for (int a = 0; a < scene->mMeshes[0]->mNumFaces; a++) {
		//D3DXVECTOR3 midPos = verticesColor[scene->mMeshes[0]->mFaces[a].mIndices[0]].position - (verticesColor[scene->mMeshes[0]->mFaces[a].mIndices[0]].position - verticesColor[scene->mMeshes[0]->mFaces[a].mIndices[1]].position) / 2;
		//D3DXVECTOR3 midPos2 = ((midPos - verticesColor[scene->mMeshes[0]->mFaces[a].mIndices[2]].position) / 2);
		D3DXVECTOR3 midPos = verticesColor[scene->mMeshes[0]->mFaces[a].mIndices[0]].position;
		//D3DXVECTOR3 midPos = startPos - ((startPos - verticesColor[scene->mMeshes[0]->mFaces[a].mIndices[1]].position) / 2);
		//D3DXVECTOR3 endPos = midPos - ((midPos - verticesColor[scene->mMeshes[0]->mFaces[a].mIndices[2]].position) / 2);

		hairs[a].hair = new Hair(_device, midPos, _baseColour, 0.1f, 10, 1, 0.98, 10);
	}

	device = _device;

	InitiateInstances(D3DXVECTOR3(50, 50, 50));

	ShutdownBuffers();

	InitializeVertexBuffer();
	InitializeInstanceBuffer();
	InitializeIndexBuffer();

	Update(0);
}

void Bear::Update(float windValue) {

	//ShutdownBuffers();

	//InitializeVertexBuffer();
	//InitializeIndexBuffer();
	//InitializeInstanceBuffer();
}

void Bear::InitiateInstances(D3DXVECTOR3 _positionMaxOffset) {
	instancesColor = new InstanceTypeColor[m_instanceCount];
	instanceOffset = new D3DXVECTOR3[m_instanceCount];
	for (int a = 0; a < m_instanceCount; a++) {
		if (m_instanceCount != 1) {
			instanceOffset[a] = D3DXVECTOR3(randFloat(-_positionMaxOffset.x, _positionMaxOffset.x), randFloat(-_positionMaxOffset.y, _positionMaxOffset.y), randFloat(-_positionMaxOffset.z, _positionMaxOffset.z));
		}
		else {
			instanceOffset[0] = D3DXVECTOR3(0, 0, 0);
		}
	}
	UpdateInstances();
}

void Bear::UpdateInstances() {
	for (int a = 0; a < m_instanceCount; a++) {
		instancesColor[a].instancePosition = verticesColor[0].position + instanceOffset[a];
		instancesColor[a].instanceColour = D3DXVECTOR4(0,0,0,0);
	}
}

void Bear::releaseInstances() {
	delete[] instanceOffset;
	instanceOffset = 0;
}

void Bear::Render(ID3D11DeviceContext* deviceContext)
{
	
	//unsigned int strides[3];
	//unsigned int offsets[3];
	//ID3D11Buffer* bufferPointers[3];
	
	
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];
	

	// Set the buffer strides.
	strides[0] = sizeof(VertexTypeColor);
	//strides[1] = sizeof(unsigned long);
	strides[1] = sizeof(InstanceTypeColor);

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;
	//offsets[2] = 0;

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = m_vertexBuffer;
	//bufferPointers[1] = m_indexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	return;
}

void Bear::regenHair(D3DXVECTOR4 _baseColour, int vertexCount)
{
	for (int a = 0; a < m_hairCount; a++) {
		hairs[a].hair->Shutdown();
		hairs[a].hair->Initialize(device, hairs[a].hair->startPosition, _baseColour, vertexCount, 1);
		hairs[a].hair->m_instanceCount = 50;
		hairs[a].hair->InitiateInstances(D3DXVECTOR3(3,3,3));
	}
}

