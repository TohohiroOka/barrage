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
	//�I������
	enum class PlayerSelect
	{
		SELECT_CONTINUE,	//�ĊJ
		SELECT_EXIT,	//�^�C�g����
	};

	//�E�B���h�E�T�C�Y
	const float WINDOW_WIDTH = 1500.f;// ����
	const float WINDOW_HEIGHT = 830.f;// �c��

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
	//�\���J�n�t���O
	bool isGameOver = false;
	//�v���C���[�I�𒆂̑I����
	PlayerSelect playerSelecting = PlayerSelect::SELECT_CONTINUE;

	std::unique_ptr<ChoiceEmphasisDrawer> choiceDrawer;


	//���o�t�F�[�Y
	enum class GAMEOVER_PHASE
	{
		PHASE_STANDBY,
		PHASE_MODELFADE,	//���f����������
		PHASE_UI_FADEIN,	//UI���t�F�[�h�C������
		PHASE_UI_SHOW,		//UI���I���\�ɂȂ�
		PHASE_DECISION,		//�I���������肳���
	};
	GAMEOVER_PHASE phase = GAMEOVER_PHASE::PHASE_STANDBY;

#pragma region time

	int			frame = 0;
	//�v���C���[���łɂ����鎞��
	const int	UI_FADEIN_FRAME = 30;
	//�t�F�[�h�C���ɂ����鎞��
	const int	PLAYER_FADE_FRAME = 60;

#pragma endregion

#pragma region display position

	//�摜����
	const int GAMEOVERTEXT_WIDTH	= 512;
	const int CONTINUE_EXIT_WIDTH	= 448;
	//�\�����W
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

	//�v���C���[�̃��f���f�[�^���Q��
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