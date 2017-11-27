#pragma once
#ifndef _HAIRCLASS_H_
#define _HAIRCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include "textureclass.h"

class Hair {

public:
	
	Hair(D3DXVECTOR3 position, float length, float sections, float _gravity = 0, float rigidity = 0);
	Hair(D3DXVECTOR3 _position, float _sections, Hair* hair1, Hair* hair2);
	bool Update(float windValue);

	D3DXVECTOR3 startPosition;
	D3DXVECTOR3* positions;
	float length;
	float sections;
	float gravity;
	float rigidity;
};

#endif