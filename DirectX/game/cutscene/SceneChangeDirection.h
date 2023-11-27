#pragma once
#include "Object/2d/Sprite.h"

class SceneChangeDirection
{
public:
	SceneChangeDirection() = default;
	~SceneChangeDirection() = default;

	static void Create();

	static void Init();

	static void Update();

	static void Draw();

	//シーン開始
	static void PlayFadeIn();
	//シーン終了
	static void PlayFadeOut();

	static void Reset() { isDirectionEnd = false; }

	static bool IsDirectionEnd() { return isDirectionEnd; }


private:
	
	static int frame;
	//ロード済か否か
	static bool isLoaded;
	//フェードインか
	static bool isFadein;
	// 遷移にかかる時間
	static const int CHANGE_FADE_FRAME;
	//遷移完了フラグ
	static bool isDirectionEnd;
	//塗りつぶすやつ
	static std::unique_ptr<Sprite> fadeSprite;
	//演出再生中か
	static bool isPlayingDirection;




};