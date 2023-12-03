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

	//�V�[���J�n
	void PlayFadeIn();
	//�V�[���I��
	void PlayFadeOut();

	void Reset() { isDirectionEnd = false; }

	bool IsDirectionEnd() { return isDirectionEnd; }

	void Delete();

private:
	
	int frame;
	//���[�h�ς��ۂ�
	bool isLoaded;
	//�t�F�[�h�C����
	bool isFadein;
	// �J�ڂɂ����鎞��
	const int CHANGE_FADE_FRAME = 60;
	//�J�ڊ����t���O
	bool isDirectionEnd;
	//�h��Ԃ����
	std::unique_ptr<Sprite> fadeSprite;
	//���o�Đ�����
	bool isPlayingDirection;
};