#include"Scene/GameManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameManager* gameManager = new GameManager;
	gameManager->Run();
	return 0;
}