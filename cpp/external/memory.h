#ifndef _MEMORY_H_
#define _MEMORY_H_

#pragma once
#include "pch.h"
#include <TlHelp32.h>
#include <ostream>
#include <iostream>

namespace core {
	namespace memory
	{
		extern HANDLE hGame;
		extern DWORD baseAddress;

		void init(HANDLE handle);

		void SetBase(DWORD);
		DWORD GetBase();

		float ReadFloat(DWORD address);
		int ReadInt(DWORD address);
		DWORD Read(DWORD address);
		bool ReadBool(DWORD address);
		char* ReadString(DWORD address);
		D3DXMATRIX ReadMatrix(DWORD address);
		char* ReadText(int address);
	};
}

#endif