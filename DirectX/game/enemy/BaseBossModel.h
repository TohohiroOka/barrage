#pragma once
#include "Object/3d/Fbx.h"

class BaseBossModel
{
public:
	virtual ~BaseBossModel() {};

	virtual void Update() {};

	virtual void Draw() {};

	virtual void DrawLightView() {};

	DirectX::XMFLOAT3 GetModelMove() { return fbxObject->GetModelMove(); }
	void AnimationReset() { fbxObject->AnimationReset(); }
	void SetAnimation(const int _animationNumber) { fbxObject->SetUseAnimation(_animationNumber); }
	Base3D* GetObjectInst() { return fbxObject.get(); }
	void RrameReset() { fbxObject->FrameReset(); }
	void SetMovement(const int _number) { fbxObject->SetUseAnimation(_number); };
	void SetIsRoop(const bool _isRoop) { fbxObject->SetIsRoop(_isRoop); }
	bool GetIsAnimationEnd() { return fbxObject->GetIsAnimationEnd(); }

protected:
	
	std::unique_ptr<FbxModel> model;
	std::unique_ptr<Fbx> fbxObject;
};

