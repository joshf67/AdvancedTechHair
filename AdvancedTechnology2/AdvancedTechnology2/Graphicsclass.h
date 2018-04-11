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

	//camera settings
	float x = 17;
	float y = 84;
	float z = 9;
	float Rotx = 90;
	float Roty = 0;
	float Rotz = 0;
	CameraClass* m_Camera;

	//copies of hair, how many sections, global force "wind"
	int instances = 10000;
	int sections = 30;
	float wind = -5;

	//model class to store bear and hair
	ModelClass* m_Model;
	ModelClass* m_Model2;


private:
	bool Render();

private:

	D3DClass* m_D3D;

	TextureShaderClass* m_TextureShader;
	ColorShaderClass* m_colour;
};

#endif