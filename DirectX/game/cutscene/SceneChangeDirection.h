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

	//�V�[���J�n
	static void PlayFadeIn();
	//�V�[���I��
	static void PlayFadeOut();

	static void Reset() { isDirectionEnd = false; }

	static bool IsDirectionEnd() { return isDirectionEnd; }


private:
	
	static int frame;
	//���[�h�ς��ۂ�
	static bool isLoaded;
	//�t�F�[�h�C����
	static bool isFadein;
	// �J�ڂɂ����鎞��
	static const int CHANGE_FADE_FRAME;
	//�J�ڊ����t���O
	static bool isDirectionEnd;
	//�h��Ԃ����
	static std::unique_ptr<Sprite> fadeSprite;
	//���o�Đ�����
	static bool isPlayingDirection;




};