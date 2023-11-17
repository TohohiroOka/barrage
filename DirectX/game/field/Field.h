#pragma once
#include "Object/3d/Object3d.h"
#include <array>
#include <forward_list>

#include "BackGround.h"
#include "FieldLine.h"

class Field
{
public:
	Field();
	~Field(){};

	void Update(const DirectX::XMFLOAT3& _playerPos);

	void Draw();

	void FrameReset() {
		backGround->FrameReset();
		FieldLine::FrameReset();
	}

private:

	//���f��
	std::unique_ptr<Model> wallModel;
	std::unique_ptr<Model> groundModel;
	//�I�u�W�F�N�g
	std::shared_ptr<Object3d> groundObject;

	//�O�ɍs���Ȃ��悤�ɂ���ׂ̕�
	std::array<std::shared_ptr<Object3d>,4> wallObject;

	//�w�i
	std::unique_ptr<BackGround> backGround;

	//�w�i�̐�
	std::unique_ptr<Engine::Timer> timer;
	std::forward_list<std::unique_ptr<FieldLine>> line;
};

