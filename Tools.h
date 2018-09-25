#pragma once
namespace Tools
{
	DWORD findPattern(DWORD startAddress, DWORD fileSize, unsigned char* pattern,const char mask[]);
	MODULEINFO GetModuleInfo(std::string ModuleName);
	void log(const char* _format, ...);
	template<typename T>
	bool ReadValue(DWORD address, T & value)
	{
		__try{
			value = *(T*)(address);
			return true;
		}
		__except (1) {
			return false;
		}
	}
};

