#pragma once
#include "Object/3d/HeightMap.h"
#include "Object/3d/Object3d.h"
#include <array>

class Field
{
public:
	Field();
	~Field(){};

	void Update();

	void Draw();

private:

	//���f��
	std::unique_ptr<Model> wallModel;
	//�I�u�W�F�N�g
	std::shared_ptr<Object3d> groundObject;

	//�O�n�`���f��
	std::shared_ptr<TerrainModel>outsideModel;
	//�O�n�`�I�u�W�F�N�g
	std::shared_ptr<HeightMap> outsideObject;

	//�O�ɍs���Ȃ��悤�ɂ���ׂ̕�
	std::array<std::shared_ptr<Object3d>,4> wallObject;
};

