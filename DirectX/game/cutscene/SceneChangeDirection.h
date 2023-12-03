#pragma once
#include "Object/2d/Sprite.h"
#include "Singleton.h"

class SceneChangeDirection : public Singleton<SceneChangeDirection>
{
	friend class Singleton<SceneChangeDirection>;

public:
	SceneChangeDirection(){};
	~SceneChangeDirection() {};

	void Create();

	void Init();

	void Update();

	void Draw();

	//シーン開始
	void PlayFadeIn();
	//シーン終了
	void PlayFadeOut();

	void Reset() { isDirectionEnd = false; }

	bool IsDirectionEnd() { return isDirectionEnd; }

	void Delete();

private:
	
	int frame;
	//ロード済か否か
	bool isLoaded;
	//フェードインか
	bool isFadein;
	// 遷移にかかる時間
	const int CHANGE_FADE_FRAME = 60;
	//遷移完了フラグ
	bool isDirectionEnd;
	//塗りつぶすやつ
	std::unique_ptr<Sprite> fadeSprite;
	//演出再生中か
	bool isPlayingDirection;
};