#pragma once
#include "engine/Object/2d/Sprite.h"
#include "engine/Object/3d/Fbx.h"
#include "engine/Math/Vector2.h"

#include "ui/ChoiceEmphasisDrawer.h"
#include "QuestionSystem.h"

#include <memory>

class GameOver
{
private:
	//選択項目
	enum class PlayerSelect
	{
		SELECT_CONTINUE,	//再開
		SELECT_EXIT,	//タイトルへ
	};

	//ウィンドウサイズ
	const float WINDOW_WIDTH = 1500.f;// 横幅
	const float WINDOW_HEIGHT = 830.f;// 縦幅

public:

	GameOver() = default;
	~GameOver() = default;

	void Initialize();

	void Update();

	void Draw();

	void StartGameOverUI();

	void ResetGameOverUI();

	bool GetIsGameOver() { return isGameOver; }

	void SetPlayerObject(Fbx* playerObject);

private:
	//表示開始フラグ
	bool isGameOver = false;
	//プレイヤー選択中の選択肢
	PlayerSelect playerSelecting = PlayerSelect::SELECT_CONTINUE;

	std::unique_ptr<ChoiceEmphasisDrawer> choiceDrawer;


	//演出フェーズ
	enum class GAMEOVER_PHASE
	{
		PHASE_STANDBY,
		PHASE_MODELFADE,	//モデルが消える
		PHASE_UI_FADEIN,	//UIがフェードインする
		PHASE_UI_SHOW,		//UIが選択可能になる
		PHASE_DECISION,		//選択肢が決定される
	};
	GAMEOVER_PHASE phase = GAMEOVER_PHASE::PHASE_STANDBY;

#pragma region time

	int			frame = 0;
	//プレイヤー消滅にかかる時間
	const int	UI_FADEIN_FRAME = 30;
	//フェードインにかかる時間
	const int	PLAYER_FADE_FRAME = 60;

#pragma endregion

#pragma region display position

	//画像横幅
	const int GAMEOVERTEXT_WIDTH	= 512;
	const int CONTINUE_EXIT_WIDTH	= 448;
	//表示座標
	const Vector2 GAMEOVERTEXT_LT = Vector2(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
	const Vector2 CONTINUE_POS		= Vector2(WINDOW_WIDTH / 4.f, 600.f);
	const Vector2 EXIT_POS			= Vector2((WINDOW_WIDTH / 4.f) * 3.f, 600.f);

#pragma endregion

#pragma region resource

	std::unique_ptr<Sprite> gameoverText;
	std::unique_ptr<Sprite> continueText;
	std::unique_ptr<Sprite> exitText;
	std::unique_ptr<Sprite> gameoverBack;

#pragma endregion

#pragma region ref

	//プレイヤーのモデルデータを参照
	Fbx* playerObject = nullptr;

#pragma endregion

	std::unique_ptr<QuestionSystem> qsys;
	enum class USER_SELECT
	{
		SELECT_RETRY,
		SELECT_END,
	};

	const float glitchAnimScaleWide = 2.f;
	const float glitchAnimScaleSmall = 0.4f;
	bool isWide = false;
};