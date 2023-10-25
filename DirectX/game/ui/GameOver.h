#pragma once
#include "engine/Object/2d/Sprite.h"
#include "engine/Math/Vector2.h"

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

	void Initialize();

	void Update();

	void Draw();

	void StartGameOverUI();

	void ResetGameOverUI();

private:
	//�\���J�n�t���O
	bool isGameOver = false;
	//�v���C���[�I�𒆂̑I����
	PlayerSelect playerSelecting = PlayerSelect::SELECT_CONTINUE;

#pragma region time

	int			frame = 0;
	const int	GAMEOVER_DIRECTION_FRAME = 300;

#pragma endregion

#pragma region display position

	//�摜����
	const int GAMEOVERTEXT_WIDTH	= 512;
	const int CONTINUE_EXIT_WIDTH	= 448;
	//�\�����W
	const Vector2 GAMEOVERTEXT_LT	= Vector2(WINDOW_WIDTH / 2.f - GAMEOVERTEXT_WIDTH / 2.f, 200.f);
	const Vector2 CONTINUE_POS		= Vector2(WINDOW_WIDTH / 4.f, 600.f);
	const Vector2 EXIT_POS			= Vector2((WINDOW_WIDTH / 4.f) * 3.f, 600.f);

#pragma endregion

#pragma region resource

	std::unique_ptr<Sprite> gameoverText;
	std::unique_ptr<Sprite> continueText;
	std::unique_ptr<Sprite> exitText;

#pragma endregion


};