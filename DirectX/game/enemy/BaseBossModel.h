#pragma once
#include "Object/3d/Fbx.h"

class BaseBossModel
{
public:

	virtual void Update() {};

	virtual void Draw() {};

private:
	
	std::unique_ptr<FbxModel> model;
	std::unique_ptr<Fbx> fbxObject;
};

