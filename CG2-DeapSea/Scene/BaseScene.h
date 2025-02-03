#pragma once

enum Scene { TITLE, INGAME, GAMEOVER, CLEAR };

class BaseScene
{
protected:
public:
	static int sceneNo;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
	virtual ~BaseScene() = default;
	int GetSceneNo();
};
