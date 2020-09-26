#include "pch.h"
#include "renderer.h"
#include <cstdio>

namespace core {
	namespace renderer
	{
		 bool showMenu = false;

		 int FPS = 0;
		 int width = 0;
		 int height = 0;

		 HWND tWindow = nullptr;
		 HWND hWindow = nullptr;
		 HWND hMessage = nullptr;

		 RECT wSize;

		 DWORD base = 0x0;

		 D3DXMATRIX viewMatrix;

		 float color[3];

		 float statX;
		 float statY;

		 bool drawMinionLine = true;
		 bool drawMinionLastHit = true;
		 bool drawChampionLine = false;
		 bool drawFPS = true;
		 bool drawStat = false;

		 bool bIsInitialized = false;
		 char buffer[256];
		
		void PrintStat(const char* fmt, ...)
		{
			va_list argList;
			va_start(argList, fmt);
			vsprintf_s(buffer, fmt, argList);
			va_end(argList);

			d3d::draw_string(statX, statY, buffer, ImColor(255, 255, 255), true, d3d::EUIFlag::UI_Large);
			statY += 20;
		}

		void DrawStat(IDirect3DDevice9* pDevice)
		{
			statX = 20;
			statY = 20;

			if (drawFPS)
			{
				PrintStat("FPS : %d", FPS);
			}

			//if (drawStat)
			//{
			//	const auto screenPos = WorldToScreen(object::CObject::GetPosition(object::ObjectManager::LocalPlayer));
			//	PrintStat("POS : (%d, %d)", (int)screenPos.X, (int)screenPos.Y);

			//	PrintStat("Range : %d", (int)object::CObject::GetAttackRange(object::ObjectManager::LocalPlayer));
			//}

			//PrintStat("Target : %d, %s", target->target, object::ObjAIBase::GetChampionName(target->target));
			//PrintStat("ForceTarget : %d", target->forceTarget);
			//PrintStat("LMouseButton : %b", target->LMouseButtonDown);
		}

		void DrawComponent(IDirect3DDevice9* pDevice)
		{
			//if (drawMinionLine)
			//{
			//	using namespace object;
			//	using namespace d3d;

			//	const auto pos = CObject::GetPosition(ObjectManager::LocalPlayer);
			//	const auto scrPos = WorldToScreen(pos);

			//	auto minions = object::ObjectManager::get_minions();
			//	for (auto minion : minions)
			//	{
			//		const auto minion_pos = CObject::GetPosition(minion);
			//		const auto minion_screen_pos = WorldToScreen(minion_pos);

			//		if (pos.DistanceTo(minion_pos) < 1000.0f)
			//		{
			//			DrawLine(scrPos.X, scrPos.Y,
			//				minion_screen_pos.X, minion_screen_pos.Y
			//				, ImColor(255, 255, 255), 2.0f);
			//		}
			//	}
			//}

			//if (drawMinionLastHit)
			//{
			//	using namespace object;
			//	using namespace d3d;

			//	auto minions = orbwalk->lastHitMinions;
			//	for (auto minion : minions)
			//	{
			//		d3d::DrawWorldCircle(CObject::GetPosition(minion), 40.0f, 10, ImColor(0, 255, 0), 2.0f);
			//	}
			//}

			//if (drawChampionLine)
			//{
			//	using namespace object;
			//	using namespace d3d;

			//	const auto pos = CObject::GetPosition(ObjectManager::LocalPlayer);
			//	const auto scrPos = WorldToScreen(pos);

			//	for (auto hero : target->enemies)
			//	{
			//		if (hero == ObjectManager::LocalPlayer)
			//			continue;

			//		const auto heroPos = CObject::GetPosition(hero);
			//		const auto heroScrPos = WorldToScreen(heroPos);

			//		DrawLine(scrPos.X, scrPos.Y,
			//			heroScrPos.X, heroScrPos.Y
			//			, ImColor(255, 255, 255), 2.0f);
			//	}
			//}

			////Component::ComponentBase::Event_OnUpdate();
			//component::ComponentBase::Event_OnRender();
		}


		void DrawMenu(IDirect3DDevice9* pDevice)
		{
			const auto colors = ImGui::GetStyle().Colors;

			colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.62f, 0.93f, 0.06f, 1.00f);

			ImGui_ImplDX9_NewFrame();

			if (!bIsInitialized)
				ImGui::SetNextWindowSize(ImVec2(400, 800));

			ImGui::Begin("Jaby.zip | al | Version 10.19.335.4706 [PUBLIC]");

			if (ImGui::BeginChild("ESP", ImVec2(380, 100), true))
			{
				ImGui::Checkbox("Show FPS", &drawFPS);
				ImGui::Checkbox("Show Stat", &drawStat);
				ImGui::Checkbox("Show Champion Line", &drawChampionLine);
				ImGui::Checkbox("Show Minion Last Hit", &drawMinionLastHit);
			}
			ImGui::EndChild();

			//component::ComponentBase::Event_OnMenu();

			ImGui::End();
			ImGui::Render();

			bIsInitialized = true;
		}

		D3DXMATRIX GetViewProjectionMatrix()
		{
			return GetViewMatrix() * GetProjectionMatrix();
		}

		D3DXMATRIX GetViewMatrix()
		{
			return memory::ReadMatrix(memory::GetBase() + offset::instance::ViewMatrix);
		}

		D3DXMATRIX GetProjectionMatrix()
		{
			return memory::ReadMatrix(memory::GetBase() + offset::instance::ViewMatrix + 0x40);
		}

		Vector3f GetScreenResolution()
		{
			//int instance;
			//
			//Memory::ReadInt(Memory::baseAddress + Offset::Game::ViewMatrix, &instance);

			//int xx, yy;
			//
			//Memory::ReadInt(instance + ADDR_WIDTH, &xx);
			//Memory::ReadInt(instance + ADDR_HEIGHT, &yy);

			return { (float)width, (float)height, 0 };
			//return {(float)2560, (float)1440, 0};
		}

		Vector3f WorldToScreen(Vector3f pos)
		{
			const auto screen = GetScreenResolution();
			auto matrix = GetViewProjectionMatrix();

			D3DXVECTOR4 clipCoords;
			clipCoords.x = pos.X * matrix[0] + pos.Y * matrix[4] + pos.Z * matrix[8] + matrix[12];
			clipCoords.y = pos.X * matrix[1] + pos.Y * matrix[5] + pos.Z * matrix[9] + matrix[13];
			clipCoords.z = pos.X * matrix[2] + pos.Y * matrix[6] + pos.Z * matrix[10] + matrix[14];
			clipCoords.w = pos.X * matrix[3] + pos.Y * matrix[7] + pos.Z * matrix[11] + matrix[15];

			if (clipCoords[3] < 0.1f) return { 0, 0, 0 };

			Vector3f M;
			M.X = clipCoords.x / clipCoords.w;
			M.Y = clipCoords.y / clipCoords.w;
			M.Z = clipCoords.z / clipCoords.w;

			const auto xx = (screen.X / 2 * M.X) + (M.X + screen.X / 2);
			const auto yy = -(screen.Y / 2 * M.Y) + (M.Y + screen.Y / 2);

			return { xx, yy, 0 };
		}
	}
}