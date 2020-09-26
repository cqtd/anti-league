#include "memory.h"

namespace core {

	namespace memory {

		HANDLE hGame;
		DWORD baseAddress;

		void init(HANDLE handle)
		{
			hGame = handle;
		}

		void set_base(DWORD address)
		{
			baseAddress = address;
		}

		DWORD get_base()
		{
			return baseAddress;
		}

		float ReadFloat(DWORD address)
		{
			float out;
			SIZE_T read;
			auto success = ReadProcessMemory(hGame, (LPCVOID)address, &out, sizeof(float), &read);

			if (success && read > 0)
			{
				return out;
			}

			return 0;
		}

		int ReadInt(DWORD address)
		{
			int out;
			SIZE_T read;
			auto success = ReadProcessMemory(hGame, (LPCVOID)address, &out, sizeof(int), &read);

			if (success && read > 0)
			{
				return out;
			}

			return 0;
		}

		DWORD read(DWORD address)
		{
			DWORD out;
			auto success = ReadProcessMemory(hGame, (LPCVOID)address, &out, sizeof(DWORD), nullptr);

			return out;
		}

		bool ReadBool(DWORD address)
		{
			bool out;
			SIZE_T read;
			auto success = ReadProcessMemory(hGame, (LPCVOID)address, &out, sizeof(bool), &read);

			if (success && read > 0)
			{
				return out;
			}

			return false;
		}

		char* ReadString(DWORD address)
		{
			char** out = nullptr;

			if (address == -1)
				return (char*)"";

			char buffer = !0;
			char* stringToRead = new char[128];
			SIZE_T NumberOfBytesToRead = sizeof(buffer);
			SIZE_T NumberOfBytesActuallyRead;
			int i = 0;
			while (buffer != 0)
			{
				BOOL success = ReadProcessMemory(hGame, (LPCVOID)address, &buffer, NumberOfBytesToRead,
					&NumberOfBytesActuallyRead);
				if (!success || NumberOfBytesActuallyRead != NumberOfBytesToRead)
					return (char*)"";

				*out[i] = buffer;
				i++;
				address++;
			}

			return *out;
		}

		D3DXMATRIX read_matrix(DWORD address)
		{
			D3DXMATRIX matrix;
			SIZE_T read;
			ReadProcessMemory(hGame, (LPCVOID)address, &matrix, 64, &read);

			return matrix;
		}

		char* ReadText(int address)
		{
			if (address == -1)
				return (char*)"-1";
			char buffer = !0;
			char* stringToRead = new char[128];
			SIZE_T NumberOfBytesToRead = sizeof(buffer);
			SIZE_T NumberOfBytesActuallyRead;
			int i = 0;
			while (buffer != 0)
			{
				BOOL success = ReadProcessMemory(hGame, (LPCVOID)address, &buffer, NumberOfBytesToRead,
					&NumberOfBytesActuallyRead);
				if (!success || NumberOfBytesActuallyRead != NumberOfBytesToRead)
					return (char*)"-1";
				stringToRead[i] = buffer;
				i++;
				address++;
			}
			return stringToRead;
		}
	}
}