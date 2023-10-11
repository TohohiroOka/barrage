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

	//モデル
	std::shared_ptr<TerrainModel> groundModel;
	//オブジェクト
	std::shared_ptr<HeightMap> groundObject;

	//外地形モデル
	std::shared_ptr<TerrainModel>outsideModel;
	//外地形オブジェクト
	std::shared_ptr<HeightMap> outsideObject;

	//外に行かないようにする為の壁
	std::unique_ptr<Model> wallModel;
	std::array<std::shared_ptr<Object3d>,4> wallObject;
};

