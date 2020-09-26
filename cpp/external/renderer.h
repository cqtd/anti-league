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

		void set_resolution(const int width, const int height);
		void set_width(const int width);
		int get_width();

		void set_height(const int height);
		int get_height();

		void print(const char*, ...);

		void draw_stat(IDirect3DDevice9* pDevice);
		void draw_component(IDirect3DDevice9* pDevice);
		void draw_menu(IDirect3DDevice9* pDevice);

		D3DXMATRIX get_view_projection_matrix();
		D3DXMATRIX get_view_matrix();
		D3DXMATRIX get_projection_matrix();
		Vector3f get_screen_resolution();
		Vector3f world_to_screen(Vector3f pos);

		
	};
}

#endif