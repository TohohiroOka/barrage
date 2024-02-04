#pragma once

#include <memory>

#include "Object/2d/Sprite.h"
#include "ui/MessageBoxDrawer.h"
#include "ui/QuestionSystem.h"
#include "system/ActionInputConfigKey.h"
#include "system/ActionInputConfigPad.h"


//�V�[�������ǌp���͂��Ȃ�
class PauseScene
{
public:

	PauseScene() = default;
	~PauseScene() = default;

	void Init();

	void Update();

	void Draw();

	//�|�[�X�J�ڔ���
	void CheckPauseInput();

	//�|�[�X��ʂ�?
	bool GetIsPause() { return isPause; }

	//�V�[���J�ڎ��s�t���O
	bool GetIsChangeScene() { return isChangeScene; }

private:

	//�e�����
	bool UpInput();

	bool DownInput();

	bool Select();

	bool Back();

	bool CheckNowInputIsPad();

	bool isPause = false;

	bool isChangeScene = false;

	bool isConfig = false;
	bool isVolume = false;

	//�I����
	enum class SELECT
	{
		SELECT_CONTINUE,
		SELECT_CONFIG,
		SELECT_TITLE,
	};

	//�����[�h���s��
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