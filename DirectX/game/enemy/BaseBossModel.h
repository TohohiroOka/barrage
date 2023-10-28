#pragma once
#include "Object/3d/Fbx.h"

class BaseBossModel
{
public:
	virtual ~BaseBossModel() {};

	virtual void Update() {};

	virtual void Draw() {};

protected:
	
	std::unique_ptr<FbxModel> model;
	std::unique_ptr<Fbx> fbxObject;
};

