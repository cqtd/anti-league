#ifndef _MAIN_H_
#define _MAIN_H_

#pragma once
#include "pch.h"

using namespace core;

////////////////////////////////////////////////////////////////////////////////////

const char          script_title[16]            = "anti league";
char                target_window_caption[64]   = "League of Legends (TM) Client";

HINSTANCE           hInst;
DWORD               procID;

HANDLE              hUpdate         = nullptr;
HANDLE              hLate           = nullptr;
HANDLE              hFixed          = nullptr;

BOOL                bIsUnloading    = FALSE;
BOOL                bInitialized    = FALSE;

WNDCLASSEXA         wcex;
MARGINS             MARGIN          = { 0, 0, renderer::width, renderer::height };

FILE*               stream;

////////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// entry point of this program.
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hPrevInstance"></param>
/// <param name="lpCmdLine"></param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow
);

/// <summary>
/// Window process callback of main window.
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CALLBACK WndProc(HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
);

/// <summary>
/// Initialize Window with DirectX and ImGUI renderer.
/// </summary>
/// <param name="hInstance"></param>
/// <returns></returns>
HWND WINAPI InitWindow(HINSTANCE hInstance);

/// <summary>
/// Updates dx9 render window for each 2 milliseconds.
/// </summary>
void update_window();

#endif