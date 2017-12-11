#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "hair.h"
#include "bear.h"
#include "colorshaderclass.h"

#include "textureshaderclass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	void CreateNewHair();

	float x = 15;
	float y = 90;
	float z = -200;
	float Rotx = 90;
	float Roty = 0;
	float Rotz = 0;
	int instances = 10000;
	int sections = 10;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ModelClass* m_Model2;
	ModelClass* m_Model3;
	ModelClass* m_Model4;
	float wind = 0;

private:
	bool Render();

private:

	D3DClass* m_D3D;

	TextureShaderClass* m_TextureShader;
	ColorShaderClass* m_colour;
};

#endif