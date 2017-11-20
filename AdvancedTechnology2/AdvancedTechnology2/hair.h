#pragma once
#ifndef _HAIRCLASS_H_
#define _HAIRCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include "textureclass.h"

class Hair {

public:
	Hair(D3DXVECTOR3 position, float length, float sections, bool gravityAffected = false, float rigidity = 0);
	bool Initialize(ID3D11Device* device);
	void Render();

private:
	D3DXVECTOR3 position;
	float length;
	float sections;
	bool gravityAffected;
	float rigidity;
};

#endif