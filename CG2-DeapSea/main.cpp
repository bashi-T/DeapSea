#include"Scene/GameManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameManager* gameManager = new GameManager;
	//_CrtSetBreakAlloc(219);
	gameManager->Run();
	//_CrtDumpMemoryLeaks();
	return 0;
}