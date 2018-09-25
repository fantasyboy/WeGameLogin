#include "stdafx.h"
#include "Tools.h"

DWORD Tools::findPattern(DWORD startAddress, DWORD fileSize, unsigned char * pattern,const char mask[])
{
	DWORD pos = 0;
	int searchLen = strlen(mask) - 1;
	//从内存内逐个字节进行比较
	for (DWORD retAddress = startAddress; retAddress < startAddress + fileSize; retAddress++)
	{
		//判断当前地址是否有效
		if (IsBadReadPtr((const void *)retAddress, 1))
		{
			pos = 0;
			continue;
		}
		if (*(PBYTE)retAddress == pattern[pos] || mask[pos] == '?')
		{
			if (mask[pos + 1] == '\0')
			{
				return (retAddress - searchLen);
			}
			pos++;
		}
		else
		{
			pos = 0;
		}
	}
	return NULL;
}

MODULEINFO Tools::GetModuleInfo(std::string ModuleName)
{
	MODULEINFO info = { 0 };
	
	auto hModule = GetModuleHandleA(ModuleName.c_str());
	if (hModule) {
		GetModuleInformation(GetCurrentProcess(), hModule, &info, sizeof(MODULEINFO));
	}

	return info;
}

void Tools::log(const char * _format, ...)
{
	std::string temp;
	va_list marker = { 0 };
	va_start(marker, _format);
	size_t num_of_chars = _vscprintf(_format, marker);
	if (num_of_chars > temp.capacity())
	{
		temp.reserve(num_of_chars + 1);
	}
	vsprintf_s(const_cast<char*>(temp.c_str()), num_of_chars + 1, _format, marker);
	va_end(marker);

	OutputDebugStringA(temp.c_str());
}
