#pragma once
#include "hair.h"

void Hair::Shutdown() {
	releaseHair();
	releaseInstances();
}

Hair::Hair(D3DXVECTOR3 _position, float _length, float _sections, float _instances, float _gravity, float _rigidity) {
	startPosition = _position;
	instances = _instances;
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
			effect.y = -gravity * (1 - (rigidity / 100)) * a;
		}
		else {
			effect.y = -gravity * a;
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

void Hair::InitiateInstances(D3DXVECTOR3 _positionMaxOffset) {
	instancePositions = new D3DXVECTOR3[instances];
	instanceOffset = new D3DXVECTOR3[instances];
	int xVal = (_positionMaxOffset.x * 10);
	int yVal = (_positionMaxOffset.y * 10);
	int zVal = (_positionMaxOffset.z * 10);
	int xValMult = (_positionMaxOffset.x * 10) * 2;
	int yValMult = (_positionMaxOffset.y * 10) * 2;
	int zValMult = (_positionMaxOffset.z * 10) * 2;
	for (int a = 0; a < instances; a++) {
		//instanceOffset[a] = D3DXVECTOR3((((rand() % xValMult) - xVal) / 10), (((rand() % yValMult) - yVal) / 10), (((rand() % zValMult) - zVal) / 10));
		instanceOffset[a] = D3DXVECTOR3((((rand() % xValMult) - xVal) / 10), (((rand() % yValMult) - yVal) / 10), 0);
	}
	UpdateInstances();
}

void Hair::UpdateInstances() {
	for (int a = 0; a < instances; a++) {
		instancePositions[a] = positions[0] + instanceOffset[a];
	}
}

void Hair::releaseHair() {
	delete[] positions;
	positions = 0;
}

void Hair::releaseInstances() {
	delete[] instanceOffset;
	instanceOffset = 0;
	delete[] instancePositions;
	instancePositions = 0;
}