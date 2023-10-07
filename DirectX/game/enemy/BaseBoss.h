#pragma once
#include "../Object/3d/Object3d.h"
#include "BaseAction.h"
#include <functional>

class BaseBoss
{
public:

	virtual void Initialize();

	virtual void Update();

	virtual void Draw() = 0;

	virtual void SetAction() = 0;

	Object3d* GetCenter() { return center.get(); }

protected:

	//中心点（描画無し）
	std::unique_ptr<Object3d> center;

	//行動
	std::unique_ptr<BaseAction> action;
	//アクション中
	bool isAction;
	//アクションの番号
	int actionNumber;
};

