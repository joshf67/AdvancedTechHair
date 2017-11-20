#pragma once
#include "hair.h"

Hair::Hair(D3DXVECTOR3 _position, float _length, float _sections, bool _gravityAffected, float _rigidity) {
	position = _position;
	length = _length;
	sections = _sections;
	gravityAffected = _gravityAffected;
	rigidity = _rigidity;
}

bool Hair::Initialize(ID3D11Device* device) {
	
	
	return true;
}

void Hair::Render() {

}