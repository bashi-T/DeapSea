#include"Scene/GameManager.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	MyEngine::GameManager* gameManager = new MyEngine::GameManager;
	
#ifdef _DEBUG
	//_CrtSetBreakAlloc(num);//
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	gameManager->Run();
	delete gameManager;
	return 0;
}