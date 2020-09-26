#include "d3d.h"

namespace core {
	
	namespace d3d
	{
		constexpr double PI = 3.14159265;
		
		IDirect3DDevice9Ex* pDevice;
		IDirect3D9Ex* pInstance;
		D3DPRESENT_PARAMETERS pParams;

		ID3DXFont* pFontVisualsLarge;
		ID3DXFont* pFontVisualsSmall;

		ID3DXFont* pFontUILarge;
		ID3DXFont* pFontUISmall;

		ID3DXLine* pLine;

		ImFont* m_default = nullptr;
		ImFont* g_font;
		ImFont* t_font;

		//LPD3DXLINE sLine;

		char buf[128];

		//Overlay FPS
		float flCurTick, flOldTick;
		int iTempFPS, iRenderFPS;

		BOOL init(HWND hWnd) {

			//Direct3D
			Direct3DCreate9Ex(D3D_SDK_VERSION, &pInstance);

			pParams.Windowed = TRUE;
			pParams.BackBufferFormat = D3DFMT_A8R8G8B8;
			pParams.BackBufferHeight = renderer::height;
			pParams.BackBufferWidth = renderer::width;
			pParams.MultiSampleQuality = DEFAULT_QUALITY;
			pParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
			pParams.EnableAutoDepthStencil = TRUE;
			pParams.AutoDepthStencilFormat = D3DFMT_D16;
			pParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

			pInstance->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pParams, nullptr, &pDevice);

			if (pDevice == nullptr) { return FALSE; }

			D3DXCreateFont(pDevice, 26, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT(R"(C:\Windows\Fonts\Ruda-Bold.ttf)"), &pFontVisualsLarge);
			D3DXCreateFont(pDevice, 18, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT(R"(C:\Windows\Fonts\Ruda-Bold.ttf)"), &pFontVisualsSmall);

			D3DXCreateFont(pDevice, 26, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Tahoma"), &pFontUILarge);
			D3DXCreateFont(pDevice, 18, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Tahoma"), &pFontUISmall);

			D3DXCreateLine(pDevice, &pLine);

			//ImGui
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO();
			ImFontConfig fontCfg = ImFontConfig();
			io.DeltaTime = 1.0f / 60.0f;

			fontCfg.RasterizerFlags = 0x01;
			fontCfg.OversampleH = fontCfg.OversampleV = 1;
			fontCfg.PixelSnapH = true;

			ImGui_ImplDX9_Init(hWnd, pDevice);

			return TRUE;
		}

		void render() {

			pParams.BackBufferHeight = renderer::height;
			pParams.BackBufferWidth = renderer::width;

			pDevice->Reset(&pParams);
			pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			pDevice->BeginScene();

			if (renderer::tWindow == GetForegroundWindow() || GetActiveWindow() == GetForegroundWindow()) {

				//Get FPS
				get_fps(&renderer::FPS);

				//Render
				renderer::DrawComponent(pDevice);
				renderer::DrawStat(pDevice);

				if (renderer::showMenu)
					renderer::DrawMenu(pDevice);
			}

			pDevice->EndScene();
			pDevice->PresentEx(nullptr, nullptr, nullptr, nullptr, 0);
		}

		void draw_string(float x, float y, const char* text, ImColor color, bool outlined, EUIFlag flag)
		{
			RECT rPosition;

			ID3DXFont* pFont;
			switch (flag)
			{
			case EUIFlag::UI_Large:
				pFont = pFontUILarge;
				break;
			case EUIFlag::UI_Small:
				pFont = pFontUISmall;
				break;
			case EUIFlag::Visual_Large:
				pFont = pFontVisualsLarge;
				break;
			case EUIFlag::Visual_Small:
				pFont = pFontVisualsSmall;
				break;

			default:
				pFont = pFontUILarge;
				break;
			}

			if (outlined)
			{
				rPosition.left = LONG(x + 1.0f);
				rPosition.top = LONG(y);

				pFont->DrawTextA(nullptr, text, strlen(text), &rPosition, DT_CENTER, D3DCOLOR_RGBA(1, 1, 1, 255));

				rPosition.left = LONG(x - 1.0f);
				rPosition.top = LONG(y);

				pFont->DrawTextA(nullptr, text, strlen(text), &rPosition, DT_CENTER, D3DCOLOR_RGBA(1, 1, 1, 255));

				rPosition.left = LONG(x);
				rPosition.top = LONG(y + 1.0f);

				pFont->DrawTextA(nullptr, text, strlen(text), &rPosition, DT_CENTER, D3DCOLOR_RGBA(1, 1, 1, 255));

				rPosition.left = LONG(x);
				rPosition.top = LONG(y - 1.0f);

				pFont->DrawTextA(nullptr, text, strlen(text), &rPosition, DT_CENTER, D3DCOLOR_RGBA(1, 1, 1, 255));
			}

			rPosition.left = LONG(x);
			rPosition.top = LONG(y);

			pFont->DrawTextA(nullptr, text, strlen(text), &rPosition, DT_NOCLIP, color);
		}

		void draw_string_outlined(char* string, float x, float y, int r, int g, int b, int a, ID3DXFont* pFont) {
			RECT rPosition;

			rPosition.left = LONG(x + 1.0f);
			rPosition.top = LONG(y);

			pFont->DrawTextA(nullptr, string, strlen(string), &rPosition, DT_NOCLIP, D3DCOLOR_RGBA(1, 1, 1, a));

			rPosition.left = LONG(x - 1.0f);
			rPosition.top = LONG(y);

			pFont->DrawTextA(nullptr, string, strlen(string), &rPosition, DT_NOCLIP, D3DCOLOR_RGBA(1, 1, 1, a));

			rPosition.left = LONG(x);
			rPosition.top = LONG(y + 1.0f);

			pFont->DrawTextA(nullptr, string, strlen(string), &rPosition, DT_NOCLIP, D3DCOLOR_RGBA(1, 1, 1, a));

			rPosition.left = LONG(x);
			rPosition.top = LONG(y - 1.0f);

			pFont->DrawTextA(nullptr, string, strlen(string), &rPosition, DT_NOCLIP, D3DCOLOR_RGBA(1, 1, 1, a));

			//DEVICE_SCALE_FACTOR factor = GetScaleFactorForDevice(DEVICE_PRIMARY);
			//if (factor != 0 && factor != DEVICE_SCALE_FACTOR::SCALE_100_PERCENT)
			//{
			//	x /= (float)(factor);
			//	x *= 100;

			//	y /= (float)(factor);
			//	y *= 100;
			//}

			rPosition.left = LONG(x);
			rPosition.top = LONG(y);

			pFont->DrawTextA(nullptr, string, strlen(string), &rPosition, DT_NOCLIP, D3DCOLOR_RGBA(r, g, b, a));
		}

		void get_fps(int* fps) {

			flCurTick = clock() * 0.001f;
			iTempFPS++;
			if ((flCurTick - flOldTick) > 1.0f) {
				iRenderFPS = iTempFPS;

				iTempFPS = 0;
				flOldTick = flCurTick;
			}

			*(int*)fps = iRenderFPS;
		}

		void draw_line(float x1, float y1, float x2, float y2, ImColor color, float thickness)
		{
			D3DXVECTOR2 dLine[2];

			pLine->SetWidth(thickness);
			pLine->SetAntialias(true);
			pLine->SetGLLines(true);

			dLine[0].x = x1;
			dLine[0].y = y1;

			dLine[1].x = x2;
			dLine[1].y = y2;

			pLine->Draw(dLine, 2, color);

			//ImGui::GetWindowDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
		}

		void draw_box(D3DXVECTOR2 pos, D3DXVECTOR2 size, ImColor color, float thickness)
		{
			struct Vertex
			{
				float x, y, z, ht;
				DWORD Color;
			}

			V[17] = {
				{ pos.x, pos.y + size.y, 0.0f, 0.0f, color },
				{ pos.x, pos.y, 0.0f, 0.0f, color },
				{ pos.x + size.x, pos.y + size.y, 0.0f, 0.0f, color },
				{ pos.x + size.x, pos.y, 0.0f, 0.0f, color }
			};

			pDevice->SetTexture(0, NULL);
			pDevice->SetPixelShader(0);
			pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, V, sizeof(Vertex));
		}

		//void DrawBox(Vector3f* begin, Vector3f* end, ImColor color, float thickness)
		//{
		//	Vector3f beginScr = renderer::WorldToScreen(*begin);
		//	Vector3f endScr = renderer::WorldToScreen(*end);

		//	auto dydx = -(end->Y - begin->X) / (end->Y - begin->Y);
		//}

		void draw_circle(int X, int Y, int radius, int numSides, ImColor color, float thickness)
		{
			D3DXVECTOR2 Line[128];
			float Step = (float)(PI * 2.0 / numSides);
			int Count = 0;
			for (float a = 0; a < PI * 2.0; a += Step)
			{
				float X1 = radius * cos(a) + X;
				float Y1 = radius * sin(a) + Y;
				float X2 = radius * cos(a + Step) + X;
				float Y2 = radius * sin(a + Step) + Y;

				Line[Count].x = X1;
				Line[Count].y = Y1;
				Line[Count + 1].x = X2;
				Line[Count + 1].y = Y2;

				Count += 2;
			}

			pLine->Begin();
			pLine->SetWidth(thickness);
			pLine->Draw(Line, Count, color);
			pLine->End();
		}

		void draw_circle(float x, float y, int radius, int numSides, ImColor color, float thickness)
		{
			draw_circle((int)x, (int)y, radius, numSides, color, thickness);
		}

		void draw_circle_on_world(Vector3f worldPos, float radius, int numSides, ImColor color, float thickness)
		{
			const auto X = worldPos.GetX();
			const auto Y = worldPos.GetY();
			const auto Z = worldPos.GetZ();

			D3DXVECTOR2 Line[128];
			const auto Step = (float)(PI * 2.0 / numSides);

			auto Count = 0;
			for (float a = 0; a < PI * 2.0; a += Step)
			{
				const auto X1 = cos(a) * radius + X;
				const auto Y1 = sin(a) * radius + Y;
				const auto X2 = cos(a + Step) * radius + X;
				const auto Y2 = sin(a + Step) * radius + Y;

				Vector3f p1 = renderer::WorldToScreen({ X1, Y1, Z });
				Vector3f p2 = renderer::WorldToScreen({ X2, Y2, Z });

				//W2S(new Vector3f(X1, Y1, Z), &p1);
				//W2S(new Vector3f(X2, Y2, Z), &p2);

				Line[Count].x = p1.X;
				Line[Count].y = p1.Y;
				Line[Count + 1].x = p2.X;
				Line[Count + 1].y = p2.Y;

				Count += 2;
			}

			pLine->Begin();
			pLine->SetWidth(thickness);
			pLine->Draw(Line, Count, color);
			pLine->End();
		}
	}
}