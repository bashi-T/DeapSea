#include"Scene/GameManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameManager* gameManager = new GameManager;
	_CrtSetBreakAlloc(127378);//enemyPopCommands
	_CrtSetBreakAlloc(127353);//enemyPopFile[0]
	//_CrtSetBreakAlloc(107732);//ground/ new Object3d;
	//_CrtSetBreakAlloc(14359);//sprite/ materialData->material.textureFilePath
	//_CrtSetBreakAlloc(139);
	gameManager->Run();
	//_CrtDumpMemoryLeaks();
	return 0;
}