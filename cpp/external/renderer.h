#ifndef _RENDERER_H_
#define _RENDERER_H_

#pragma once
#include "pch.h"

#include <d3d9.h>
#include <d3dx9.h>
#include "Vector3f.hpp"

namespace core {
	namespace renderer
	{
		extern bool showMenu;
		
		extern int FPS;
		extern int width;
		extern int height;

		extern HWND tWindow;
		extern HWND hWindow;
		extern HWND hMessage;

		extern RECT wSize;

		extern DWORD base;

		extern D3DXMATRIX viewMatrix;

		extern float color[3];

		extern float statX;
		extern float statY;

		extern bool drawMinionLine;
		extern bool drawMinionLastHit;
		extern bool drawChampionLine;
		extern bool drawFPS;
		extern bool drawStat;

		extern bool bIsInitialized;
		extern char buffer[256];

		void PrintStat(const char*, ...);
		void DrawStat(IDirect3DDevice9* pDevice);
		void DrawComponent(IDirect3DDevice9* pDevice);
		void DrawMenu(IDirect3DDevice9* pDevice);

		D3DXMATRIX GetViewProjectionMatrix();
		D3DXMATRIX GetViewMatrix();
		D3DXMATRIX GetProjectionMatrix();
		Vector3f GetScreenResolution();
		Vector3f WorldToScreen(Vector3f pos);

		
	};
}

#endif