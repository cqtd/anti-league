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

		void set_base(DWORD);
		DWORD get_base();
		
		DWORD read(DWORD address);
		D3DXMATRIX read_matrix(DWORD address);

		float ReadFloat(DWORD address);
		int ReadInt(DWORD address);
		bool ReadBool(DWORD address);
		
		char* ReadText(int address);
		char* ReadString(DWORD address);
	};
}

#endif