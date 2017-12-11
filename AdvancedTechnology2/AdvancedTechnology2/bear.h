#pragma once
#ifndef _BEARCLASS_H_
#define _BEARCLASS_H_

#include "modelclass.h"
#include "hair.h"

class Bear : public ModelClass {

public:
	
	bool Initialize(ID3D11Device* _device, D3DXVECTOR3 _position, D3DXVECTOR4 _baseColour, int _instances) override;
	Bear(ID3D11Device* _device, D3DXVECTOR3 _position, D3DXVECTOR4 _baseColour, int _instances);
	void InitiateInstances(D3DXVECTOR3 _positionMaxOffset);
	void UpdateInstances();
	void Update(float windValue) override;
	void ShutdownData() override;
	void releaseInstances();
	void Render(ID3D11DeviceContext*) override;
	void regenHair(D3DXVECTOR4 _baseColour, int vertexCount);

	struct hairStruct
	{
		Hair* hair;
	};

	D3DXVECTOR3 startPosition;
	D3DXVECTOR3* instanceOffset;
	D3DXVECTOR4 baseColour;
	int m_hairCount;
	hairStruct* hairs;

};

#endif