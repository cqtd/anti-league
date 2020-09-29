#include "main.h"

using namespace core;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/// 콘솔 활성화
	/// Activate console for logging.
	if (AllocConsole())
	{
		freopen_s(&stream, "CONIN$", "rb", stdin);
		freopen_s(&stream, "CONOUT$", "wb", stdout);
		freopen_s(&stream, "CONOUT$", "wb", stderr);
	}

	/// 윈도우 찾기
	/// Find target window
	while (renderer::get_width() < 640 && renderer::get_height() < 480)
	{
		renderer::tWindow = FindWindowA(nullptr, target_window_caption);

		GetWindowRect(renderer::tWindow, &renderer::wSize);

		const auto width = renderer::wSize.right - renderer::wSize.left;
		const auto height = renderer::wSize.bottom - renderer::wSize.top;

		renderer::set_resolution(width, height);
	}

	/// 윈도우 스레드 프로세스 ID 얻기
	/// Get process id of window thread
	GetWindowThreadProcessId(renderer::tWindow, &procID);

	/// 클래스 초기화
	/// initialize classes
	memory::init(OpenProcess(PROCESS_VM_READ, true, procID));

	// 컴포넌트 등록
	//orbwalk->Register();
	//target->Register();

	// 모듈 베이스 주소 얻기
	const auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &moduleEntry))
		{
			//memory->baseAddress = (DWORD)moduleEntry.modBaseAddr;
			memory::set_base((DWORD)moduleEntry.modBaseAddr);
		}

		CloseHandle(hSnapshot);
	}

	// 오버레이 윈도우 초기화
	renderer::hWindow = init_window((HINSTANCE)hInstance);
	d3d::init(renderer::hWindow);

	// 윈도우 초기화 실패시 종료
	if (renderer::hWindow == nullptr)
	{
		exit(1);
	}

	MSG msg;

	// 윈도우 켜기
	ShowWindow(renderer::hWindow, SW_SHOW);
	bInitialized = TRUE;

	// Thread 생성
	hUpdate = CreateThread(nullptr, 0,(LPTHREAD_START_ROUTINE)(loop::update), nullptr, 0, nullptr);
	hLate = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(loop::late_update), nullptr, 0, nullptr);
	hFixed = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(loop::fixed_update), nullptr, 0, nullptr);

	// Window 업데이트 스레드
	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(update_window), nullptr, 0, nullptr);

	// 로드된 상태
	while (!bIsUnloading)
	{
		if (PeekMessage(&msg, renderer::hWindow, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
			TranslateMessage(&msg);
		}

		// 언로드 될 때
		if (bIsUnloading)
		{
			std::this_thread::sleep_for(std::chrono::seconds(3));
			//exit(0);
		}
	}

	// 윈도우 끄기
	DestroyWindow(renderer::hWindow);
	UnregisterClassA(script_title, static_cast<HINSTANCE>(hInstance));
	FreeLibraryAndExitThread(static_cast<HMODULE>(hInstance), 0);
}

void update_window()
{
	while (!bIsUnloading)
	{
		auto* const h_window = FindWindowA(nullptr, target_window_caption);

		// escape routine
		if (!h_window && renderer::hWindow && !bIsUnloading)
		{
			ShowWindow(renderer::hWindow, SW_HIDE);
			bIsUnloading = TRUE;
		}

		if (h_window)
		{
			GetWindowRect(h_window, &renderer::wSize);
			renderer::width = renderer::wSize.right - renderer::wSize.left;
			renderer::height = renderer::wSize.bottom - renderer::wSize.top;

			const DWORD style_flag = GetWindowLong(h_window, GWL_STYLE);

			if (style_flag & WS_BORDER)
			{
				renderer::wSize.top += 23;
				renderer::height -= 23;
			}

			if (renderer::hWindow)
			{
				MoveWindow(renderer::hWindow, renderer::wSize.left, renderer::wSize.top, renderer::width,
				           renderer::height, true);
			}
		}

		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}

HWND WINAPI init_window(HINSTANCE hInstance)
{
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbClsExtra = NULL;
	wcex.cbWndExtra = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hbrBackground = static_cast<HBRUSH>(CreateSolidBrush(RGB(0, 0, 0)));
	wcex.hInstance = hInst;
	wcex.lpfnWndProc = window_proc_callback;
	wcex.lpszClassName = script_title;
	wcex.lpszMenuName = script_title;
	wcex.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassExA(&wcex))
	{
		exit(1);
	}

	renderer::hWindow = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
	                                    (script_title), script_title, WS_POPUP, 1, 1, renderer::width, renderer::height,
	                                    nullptr, nullptr, nullptr, nullptr);

	SetLayeredWindowAttributes(renderer::hWindow, 0, 255, LWA_ALPHA);
	SetLayeredWindowAttributes(renderer::hWindow, RGB(0, 0, 0), 0, ULW_COLORKEY);

	return renderer::hWindow;
}

LRESULT CALLBACK window_proc_callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:

		DwmExtendFrameIntoClientArea(hWnd, &margins);
		break;

	case WM_PAINT:
		d3d::render();
		break;

	case WM_DESTROY:

		//Cleanup Resources
		ImGui::Shutdown();
		DeleteObject(wcex.hbrBackground);
		DestroyCursor(wcex.hCursor);
		DestroyIcon(wcex.hIcon);
		DestroyIcon(wcex.hIconSm);

		PostQuitMessage(1);
		exit(0);

	default:
		ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
