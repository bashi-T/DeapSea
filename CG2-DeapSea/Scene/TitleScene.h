#pragma once
#include"Input/Input.h"
#include"SceneManager.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Object3d.h"
#include"Sprites/Sprite.h"
#include"Objects/Particle.h"
#include "GameManager.h"
#include"GameObject/Cursor.h"
#include"GameObject/Whale.h"
#include"Managers/AudioManager.h"

class TitleScene:public SceneManager
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
	std::vector<Object3d*> objects3d;
	std::vector <Particle*> particles;
	std::vector <Sprite*> sprites;
	Cursor* cursor;
	Whale* whale;


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
};

