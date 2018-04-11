#pragma once
#ifndef _HAIRCLASS_H_
#define _HAIRCLASS_H_

#include "modelclass.h"

class Hair : public ModelClass {

public:
	
	bool Initialize(ID3D11Device* _device, D3DXVECTOR3 _position, D3DXVECTOR4 _baseColour, int vertexCount, int _instances) override;
	Hair();
	Hair(ID3D11Device* _device, D3DXVECTOR3 position, D3DXVECTOR4 _baseColour, float length, int sections, int instances = 0, float _gravity = 0, float rigidity = 0);
	Hair(D3DXVECTOR3 _position, float _sections, Hair* hair1, Hair* hair2);
	void InitiateInstances(D3DXVECTOR3 _positionMaxOffset);
	void UpdateInstances();
	void Update(float windValue) override;
	void ShutdownData() override;
	void releaseInstances();
	void Render(ID3D11DeviceContext*) override;

	//store the original hair strand's position
	D3DXVECTOR3 startPosition;
	
	//store all of the positions of all instances
	D3DXVECTOR3* instanceOffset;

	//store the colour of the original hair strand
	D3DXVECTOR4 baseColour;

	//store the length, gravity and rigidity of the original hair strand
	float length;
	float gravity;
	float rigidity;

};

#endif