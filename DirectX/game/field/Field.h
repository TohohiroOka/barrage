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
	~Field(){
		FieldLine::Delete();
	};

	void Update(const DirectX::XMFLOAT3& _playerPos, const DirectX::XMFLOAT3& _cameraPos);

	void Draw();

	/// <summary>
	/// 壁の描画セット
	/// </summary>
	void WallDrawSet(const DirectX::XMFLOAT3& _playerPos, const DirectX::XMFLOAT3& _cameraPos);

	float SetAlpha(const float _c_p,const float _p);

	void FrameReset() {
		enclosure->FrameReset();
		backGround->FrameReset();
		FieldLine::FrameReset();
	}

private:

	//モデル
	std::unique_ptr<Model> wallModel;
	std::unique_ptr<Model> groundModel;
	//オブジェクト
	std::unique_ptr<Object3d> groundObject;

	//外に行かないようにする為の壁
	std::array<std::unique_ptr<Object3d>,4> wallObject;

	//背景
	std::unique_ptr<BackGround> backGround;

	//背景の線
	std::unique_ptr<Engine::Timer> timer;
	std::forward_list<std::unique_ptr<FieldLine>> line;

	std::unique_ptr<Model> blockModel;
	std::unique_ptr<InstanceObject> enclosure;
	std::array<DirectX::XMMATRIX, 8> enclosureWorld;
};

