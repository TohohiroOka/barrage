#pragma once

#include "Object/2d/Sprite.h"

#include <array>
#include <memory>

class MessageBoxDrawer
{
public:

	MessageBoxDrawer() = default;
	~MessageBoxDrawer() = default;

	void Init();

	void Update();

	void Draw();

	void StartDisplay(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size,bool isCtrlPointLT = true);

	void EndDisplay();

private:
	//�g��
	std::array<std::unique_ptr<Sprite>, 4>	cornerSprites;
	std::array<std::unique_ptr<Sprite>, 2>	verticalSprites;
	std::array<std::unique_ptr<Sprite>, 2>	horizontalSprites;
	std::unique_ptr<Sprite>					backSprite;

	//���b�Z�[�W�E�B���h�E�̏��
	DirectX::XMFLOAT2	size;			//�E�B���h�E�T�C�Y
	DirectX::XMFLOAT2	pos;			//����W
	bool				isCtrlPointLT;	//��_�͍��ォ

	//�`��p�̉摜�̏��
	const float SPRITE_WIDTH = 32.f;
	const float SPRITE_HEIGHT = 32.f;
	

	//�`��t���O
	bool isDisplay = false;

	//���񐶐����̃��[�h
	static bool isLoadedResources;

};