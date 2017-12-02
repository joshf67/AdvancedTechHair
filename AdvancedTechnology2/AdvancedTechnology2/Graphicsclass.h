#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
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

	float x = 0;
	float y = 0;
	float z = -10;
	int instances = 10000;
	int sections = 10;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	float wind = 0;

private:
	bool Render();

private:

	D3DClass* m_D3D;

	TextureShaderClass* m_TextureShader;
};

#endif