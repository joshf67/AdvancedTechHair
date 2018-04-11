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

	//setup assimp importer
	Assimp::Importer importer;

	//load bear model
	const aiScene* scene = importer.ReadFile("../AdvancedTechnology2/testBear.FBX",
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	//end if the scene fails
	if (!scene) {
		exit(0);
	}

	//setup vertex count
	m_vertexCount = scene->mMeshes[0]->mNumVertices;
	verticesColor = new VertexTypeColor[m_vertexCount];

	//loop through all vertices and setup position and colour
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
	
	//setup index count
	m_indexCount = scene->mMeshes[0]->mNumFaces * 3;
	indices = new unsigned int[m_indexCount];

	//loop through all indices and set them up
	for (int a = 0; a < m_indexCount / 3; a++) {
		for (int b = 0; b < 3; b++) {
			indices[((a * 3) + b)] = scene->mMeshes[0]->mFaces[a].mIndices[b];
		}
	}
	

	//manual face tesselation test
	m_hairCount = scene->mMeshes[0]->mNumFaces;
	hairs = new hairStruct[m_hairCount];

	//loop through all faces find the middle position and create new hair at that position
	for (int a = 0; a < scene->mMeshes[0]->mNumFaces; a++) {
		D3DXVECTOR3 midPos = (verticesColor[scene->mMeshes[0]->mFaces[a].mIndices[0]].position + 
								verticesColor[scene->mMeshes[0]->mFaces[a].mIndices[1]].position + 
								verticesColor[scene->mMeshes[0]->mFaces[a].mIndices[2]].position) / 3;

		hairs[a].hair = new Hair(_device, midPos, _baseColour, 0.1f, 10, 1, 0.98, 0);
	}

	//loop through all verticies and apply magic numbers to line up hair with bear
	for (int a = 0; a < m_vertexCount; a++) {
		verticesColor[a].position = D3DXVECTOR3(scene->mMeshes[0]->mVertices[a].x * 2, scene->mMeshes[0]->mVertices[a].y * 2, scene->mMeshes[0]->mVertices[a].z * 2);
		verticesColor[a].position += D3DXVECTOR3(-5.75f, 7.125f, 0.5f); //magic number to put hair in correct position
	}

	device = _device;


	//generate the default hairs
	regenHair(D3DXVECTOR4(0.2f, 0.2f, 0.1f, 1), 10);

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
	//setup instance buffer for rendering
	instancesColor = new InstanceTypeColor[m_instanceCount];
	instanceOffset = new D3DXVECTOR3[m_instanceCount];

	instanceOffset[0] = D3DXVECTOR3(0, 0, 0);
	UpdateInstances();
}

void Bear::UpdateInstances() {
	instancesColor[0].instancePosition = verticesColor[0].position + instanceOffset[0];
	instancesColor[0].instanceColour = D3DXVECTOR4(0,0,0,0);
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
	//loop through all hairs and reset them
	for (int a = 0; a < m_hairCount; a++) {
		hairs[a].hair->Shutdown();
		hairs[a].hair->releaseInstances();
		hairs[a].hair->Initialize(device, hairs[a].hair->startPosition, _baseColour, vertexCount, 1);
		hairs[a].hair->m_instanceCount = 500;
		hairs[a].hair->InitiateInstances(D3DXVECTOR3(2,2,2));
	}
}

