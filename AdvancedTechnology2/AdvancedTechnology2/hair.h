#pragma once
#ifndef _HAIRCLASS_H_
#define _HAIRCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include "textureclass.h"

class Hair {

public:
	
	Hair(D3DXVECTOR3 position, float length, float sections, float instances = 0, float _gravity = 0, float rigidity = 0);
	Hair(D3DXVECTOR3 _position, float _sections, Hair* hair1, Hair* hair2);
	void InitiateInstances(D3DXVECTOR3 _positionMaxOffset);
	void UpdateInstances();
	bool Update(float windValue);
	void Shutdown();
	void releaseInstances();
	void releaseHair();

	D3DXVECTOR3 startPosition;
	D3DXVECTOR3* positions;
	D3DXVECTOR3* instanceOffset;
	D3DXVECTOR3* instancePositions;
	float instances;
	float length;
	float sections;
	float gravity;
	float rigidity;
};

#endif