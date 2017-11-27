#pragma once
#include "hair.h"

Hair::Hair(D3DXVECTOR3 _position, float _length, float _sections, float _gravity, float _rigidity) {
	startPosition = _position;
	length = _length;
	sections = _sections;
	positions = new D3DXVECTOR3[sections];
	gravity = _gravity;
	rigidity = _rigidity;

	Update(0);

}

Hair::Hair(D3DXVECTOR3 _position, float _sections, Hair* hair1, Hair* hair2) {
	startPosition = _position;
	length = (hair1->length + hair2->length) / 2;
	sections = _sections;
	positions = new D3DXVECTOR3[sections];
	gravity = (hair1->gravity + hair2->gravity) / 2;
	rigidity = (hair1->rigidity + hair2->rigidity) / 2;

	Update(0);
}

bool Hair::Update(float windValue) {
	
	for (int a = 0; a < sections; a++) {
		D3DXVECTOR3 position(0, 0, 0);
		D3DXVECTOR3 effect(0, 0, 0);
		effect.x = length;
		effect.x += windValue;
		if (rigidity != 0) {
			effect.y = gravity * (1 - (100 / rigidity)) * a;
		}
		else {
			effect.y = gravity * a;
		}
		D3DXVec3Normalize(&effect, &effect);
		effect *= length;
		if (a == 0) {
			position = startPosition + effect;
		}
		else {
			position = positions[a - 1] + effect;
		}
		positions[a] = position;
	}

	return true;
}