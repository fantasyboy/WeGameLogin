#include "stdafx.h"
#include "GameFunction.h"
#include "Tools.h"
std::unique_ptr<GameFunction> GameFunction::m_pInstance(new GameFunction());
GameFunction::GameFunction()
{
}

GameFunction::~GameFunction()
{
}

GameFunction * GameFunction::GetInstance()
{
	return m_pInstance.get();
}

bool GameFunction::InitWeGameEv()
{
	auto login_mgr =  Tools::GetModuleInfo("login_mgr.dll");
	if ((DWORD)login_mgr.lpBaseOfDll < 1) {
		Tools::log("HXL: login_mgr Module Failed! \n");
		return false;
	}

	BYTE szLoginPattern[47] = {
		0x50,0x51,0xA1,0x28,0x8C,0xFC,0x0A,0x33,
		0xC5,0x50,0x8D,0x45,0xF4,0x64,0xA3,0x00,
		0x00,0x00,0x00,0x83,0x25,0x24,0x20,0xFD,
		0x0A,0x00,0xC7,0x45,0xF0,0x28,0x20,0xFD,
		0x0A,0x8B,0x45,0xF0,0x83,0x20,0x00,0x83,
		0x65,0xFC,0x00,0x83,0x4D,0xFC,0xFF
	};

	auto Login_Pattern_Addr =  Tools::findPattern((DWORD)(login_mgr.lpBaseOfDll), login_mgr.SizeOfImage, szLoginPattern, "***????**************????****????**************");
	if (0 == Login_Pattern_Addr) {
		Tools::log("HXL: Login_Pattern_Addr Failed! \n");
		return false;
	}

	Tools::log("HXL: Login_Pattern_Addr= %x ! \n", Login_Pattern_Addr);

	DWORD TempLoginObj = { 0 };
	if (!Tools::ReadValue<DWORD>(Login_Pattern_Addr + 0x15, TempLoginObj)) {
		return false;
	}

	if (!Tools::ReadValue<DWORD>(TempLoginObj, m_LoginMgr_Login_Obj)) {
		return false;
	}
	Tools::log("HXL: m_LoginMgr_Login_Obj= %x ! \n", m_LoginMgr_Login_Obj);

	auto game_library = Tools::GetModuleInfo("game_library.dll");
	if ((DWORD)game_library.lpBaseOfDll < 1) {
		Tools::log("HXL: game_library Module Failed! \n");
		return false;
	}


	BYTE szLaunchGamePattern[47] = {
		0xA3,0xC4,0xE5,0x05,0x11,0xC6,0x45,0xFC,
		0x08,0xC6,0x45,0xFC,0x05,0x8D,0x45,0xF0,
		0x89,0x45,0xC0,0x8B,0x45,0xC0,0x83,0x38,
		0x00,0x74,0x0A,0x8B,0x45,0xC0,0x8B,0x08,
		0xE8,0x6B,0x90,0xDC,0xFF,0x83,0x4D,0xFC,
		0xFF,0x8B,0x45,0xBC,0x8B,0x4D,0xF4
	};

	auto Launch_Game_Addr = Tools::findPattern((DWORD)(game_library.lpBaseOfDll), game_library.SizeOfImage, szLaunchGamePattern, "*????********************??******????**********");
	if (0 == Launch_Game_Addr) {
		Tools::log("HXL: Launch_Game_Addr  Failed! \n");
		return false;
	}

	DWORD LaunchGameObj = { 0 };
	if (!Tools::ReadValue<DWORD>(Launch_Game_Addr + 0x1, LaunchGameObj)) {
		return false;
	}

	if (!Tools::ReadValue<DWORD>(LaunchGameObj, m_GameLibrary_LaunchGame_Obj)) {
		return false;
	}

	Tools::log("HXL: m_GameLibrary_LaunchGame_Obj= %x ! \n", m_GameLibrary_LaunchGame_Obj);
	return true;
}


void GameFunction::LoginByPassword(const char * username,const char * password)
{
	DWORD m_base = m_LoginMgr_Login_Obj;
	__try {
		__asm
		{
			push 0x203C01;
			MOV EAX, password;
			PUSH EAX;
			MOV EAX, username;
			PUSH EAX;
			push 0;
			mov eax, m_base;
			mov eax, [eax];
			mov ecx, m_base;
			call[eax + 0Ch];
		}
	}
	__except (1) {

	}
}

void GameFunction::LaunchGame(DWORD GameId, DWORD Area)
{

	wchar_t szTag[] = L"0";
	wchar_t szTag2[] = L"3100081-1";

	ULONG_PTR uTag1 = (ULONG_PTR)szTag;
	ULONG_PTR uTag2 = (ULONG_PTR)szTag2;

	DWORD m_base = m_GameLibrary_LaunchGame_Obj;
	__asm
	{
		PUSH uTag1;
		PUSH 0;
		PUSH uTag2;
		PUSH 4;
		PUSH 0x62AE7C6A;
		PUSH Area;
		PUSH GameId;
		MOV EAX, m_base;
		MOV EAX, [EAX];
		MOV ECX, m_base;
		CALL[EAX + 98h];
	}

}

bool GameFunction::HookLaunchGame()
{
	return false;
}
