#pragma once

enum Scene { TITLE, INGAME };

class SceneManager
{
protected:
public:
	static int sceneNo;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
	virtual ~SceneManager();
	int GetSceneNo();
};

