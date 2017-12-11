#pragma once
#include "Graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(x, y, z);
	m_Camera->SetRotation(Rotx, Roty, Rotz);

	// Create the model object.
	m_Model = new Hair(m_D3D->GetDevice(), D3DXVECTOR3(0, 50, 0), D3DXVECTOR4(0.2f,0.2f,0.1f,1), 0.1f, sections, instances, 0.98f, 10);
	m_Model2 = new Bear(m_D3D->GetDevice(), D3DXVECTOR3(0, 0, -100), D3DXVECTOR4(1,1,1,1), 1);
	if (!m_Model)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	if (!m_Model2) {
		MessageBox(hwnd, L"Could not initialize the model2 object.", L"Error", MB_OK);
		return false;
	}

	m_colour = new ColorShaderClass;
	if (!m_colour)
	{
		return false;
	}

	result = m_colour->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the colour shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}


	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the texture shader object.
	if (m_colour)
	{
		m_colour->Shutdown();
		delete m_colour;
		m_colour = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the model object.
	if (m_Model2)
	{
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;


	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

void GraphicsClass::CreateNewHair() {
	m_Model->Shutdown();
	m_Model->Initialize(m_D3D->GetDevice(), D3DXVECTOR3(0, 50, 0), D3DXVECTOR4(0.2f, 0.2f, 0.1f, 1), sections, instances);
	((Bear*)m_Model2)->regenHair(D3DXVECTOR4(0.2f, 0.2f, 0.1f, 1), sections);
}

bool GraphicsClass::Render()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Update(wind);
	if (wind != 0) {
		if (wind > 0) {
			//wind -= 0.5f;
		}
		else {
			//wind += 0.5f;
		}
	}
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.
	//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetVertexCount(), m_Model->GetInstanceCount(), worldMatrix, viewMatrix,
	//	projectionMatrix, m_Model->GetTexture());
	result = m_colour->Render(m_D3D->GetDeviceContext(), m_Model->GetVertexCount(), m_Model->GetIndexCount(), m_Model->GetInstanceCount() , 2, worldMatrix, viewMatrix,
		projectionMatrix);
	if (!result)
	{
		return false;
	}

	m_Model2->Render(m_D3D->GetDeviceContext());

	result = m_colour->Render(m_D3D->GetDeviceContext(), m_Model2->GetVertexCount(), m_Model2->GetIndexCount(), m_Model2->GetInstanceCount() , 2, worldMatrix, viewMatrix,
	projectionMatrix);
	if (!result)
	{
		return false;
	}

	for (int a = 0; a < ((Bear*)m_Model2)->m_hairCount; a++) {
		((Bear*)m_Model2)->hairs[a].hair->Update(wind);
		((Bear*)m_Model2)->hairs[a].hair->Render(m_D3D->GetDeviceContext());

		result = m_colour->Render(m_D3D->GetDeviceContext(), ((Bear*)m_Model2)->hairs[a].hair->GetVertexCount(), ((Bear*)m_Model2)->hairs[a].hair->GetIndexCount(), ((Bear*)m_Model2)->hairs[a].hair->GetInstanceCount(), 2, worldMatrix, viewMatrix,
			projectionMatrix);
		if (!result)
		{
			return false;
		}
	}

	/*

	result = m_colour->Render(m_D3D->GetDeviceContext(), m_Model3->GetIndexCount(), m_Model3->GetInstanceCount(), worldMatrix, viewMatrix,
		projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = m_colour->Render(m_D3D->GetDeviceContext(), m_Model4->GetIndexCount(), m_Model4->GetInstanceCount(), worldMatrix, viewMatrix,
		projectionMatrix);
	if (!result)
	{
		return false;
	}
	*/

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}