#pragma once

#include <memory>

#include "Object/2d/Sprite.h"
#include "ui/MessageBoxDrawer.h"
#include "ui/QuestionSystem.h"
#include "system/ActionInputConfigKey.h"
#include "system/ActionInputConfigPad.h"


//シーンだけど継承はしない
class PauseScene
{
public:

	PauseScene() = default;
	~PauseScene() = default;

	void Init();

	void Update();

	void Draw();

	//ポース遷移判定
	void CheckPauseInput();

	//ポース画面か?
	bool GetIsPause() { return isPause; }

	//シーン遷移実行フラグ
	bool GetIsChangeScene() { return isChangeScene; }

private:

	//各種入力
	bool UpInput();

	bool DownInput();

	bool Select();

	bool Back();

	bool CheckNowInputIsPad();

	bool isPause = false;

	bool isChangeScene = false;

	bool isConfig = false;
	bool isVolume = false;

	//選択肢
	enum class SELECT
	{
		SELECT_CONTINUE,
		SELECT_CONFIG,
		SELECT_TITLE,
	};

	//他モード実行中
	bool isSelected = false;


	std::unique_ptr<MessageBoxDrawer> msDrawer;

	std::unique_ptr<Sprite> pauseText;

	std::unique_ptr<QuestionSystem> pauseQsys;
	std::vector<std::wstring> choice;

	std::unique_ptr<ActionInputConfigKey> aicKey;
	std::unique_ptr<ActionInputConfigPad> aicPad;

	std::unique_ptr<Sprite> screenBack;

	bool oldPause = false;

};