#pragma once
#include"Input/Input.h"
#include"SceneManager.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Object3d.h"
#include"Sprites/Sprite.h"
#include"Objects/Particle.h"
#include "GameManager.h"
#include"GameObject/Cursor.h"
#include"GameObject/UIPlane.h"
#include"GameObject/Whale.h"
#include"GameObject/Player.h"
#include"Managers/AudioManager.h"

class TitleScene :public SceneManager
{
public:
	//~TitleScene();
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
private:
	//Input* input;
	//Object3dCommon* object3dCommon;
	//SRVManager* srvManager;
	//Camera* camera;
	std::vector <Particle*> particles;
	std::vector <Sprite*> sprites;
	std::vector <UIPlane*>uiPlanes;
	Cursor* cursor;
	Whale* whale;
	Player* player;
	bool isSceneTransition;
	bool isStageSelect;
	int numStage;
	int sceneTransitionTime = 0;
	int nowStage;
	int cooltime = 0;
	int floatTime = 0;

	AudioManager::SoundData bgm;
	AudioManager::SoundData enterSound;
	AudioManager::SoundData moveSound;
	//std::string PNGs[6];
	std::string Planes[6]
	{
		"plane/plane.obj",
		"plane/plane.obj",
		"plane/plane.obj",
		"plane/plane.obj",
		"plane/plane.obj",
		"plane/plane.obj",
	};

};