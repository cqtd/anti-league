#ifndef _D3D_H_
#define _D3D_H_

#pragma once
#include "pch.h"

namespace core {
	
	namespace d3d
	{
		enum class eUserInterfaceFlag
		{
			UI_Large,
			UI_Small,
			Visual_Large,
			Visual_Small,
		};

		extern ID3DXLine* pLine;

		extern ID3DXFont* pFontVisualsLarge;
		extern ID3DXFont* pFontVisualsSmall;

		extern ID3DXFont* pFontUILarge;
		extern ID3DXFont* pFontUISmall;

		extern ImFont* m_default;
		extern ImFont* g_font;
		extern ImFont* t_font;

		//extern LPD3DXLINE sLine;

		void render();
		void get_fps(int* fps);
		BOOL init(HWND hWnd);

		void draw_string(
			float x, float y, 
			const char* text, 
			
			ImColor color = ImColor(255, 255, 255), 
			bool outlined = false, 
			eUserInterfaceFlag flag = eUserInterfaceFlag::UI_Small
		);
		
		void draw_string_outlined(
			char* string, 
			float x, float y, 
			int r, int g, int b, int a, 
			ID3DXFont* pFont
		);

		void draw_line(
			float x1,
			float y1,
			float x2,
			float y2,
			ImColor color,
			float thickness
		);
		
		void draw_box(
			D3DXVECTOR2 pos, 
			D3DXVECTOR2 size, 
			ImColor color, 
			float thickness
		);
		
		//void DrawBox(
		//	Vector3f* begin, 
		//	Vector3f* end, 
		//	ImColor color, 
		//	float thickness
		//);
		
		void draw_circle(
			int x, int y, 
			int radius, 
			int numSides, 
			ImColor color, 
			float thickness
		);
		
		void draw_circle(
			float x, float y,
			int radius, 
			int numSides, 
			ImColor color, 
			float thickness
		);
		
		void draw_circle_on_world(
			Vector3f worldPos, 
			float radius, 
			int numSides, 
			ImColor color, 
			float thickness
		);

	};
}

#endif