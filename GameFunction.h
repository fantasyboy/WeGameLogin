#pragma once
#include <memory>

class GameFunction
{
	GameFunction();
	GameFunction(const GameFunction&) = delete;
	void operator=(const GameFunction&) = delete;

	static std::unique_ptr<GameFunction> m_pInstance;
public:
	~GameFunction();
	static GameFunction* GetInstance();

	bool InitWeGameEv();

	void LoginByPassword(const char *username,const char* password);
	void LaunchGame(DWORD GameId, DWORD Area);
	bool HookLaunchGame();
private:
	DWORD m_LoginMgr_Login_Obj;
	DWORD m_GameLibrary_LaunchGame_Obj;
};



