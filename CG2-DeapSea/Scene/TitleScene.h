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
	std::vector<std::unique_ptr<Particle>> particles;
	std::vector<std::unique_ptr<Sprite>> sprites;
	std::vector<std::unique_ptr<UIPlane>> uiPlanes;
	std::unique_ptr<Cursor> cursor_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Whale> whale_;
	bool isSceneTransition = false;
	bool isStageSelect = false;
	int numStage = 0;
	int sceneTransitionTime = 0;
	int nowStage = 0;
	int cooltime = 0;
	int floatingTime = 0;

	AudioManager::SoundData bgm = {};
	AudioManager::SoundData enterSound = {};
	AudioManager::SoundData moveSound = {};
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