#pragma once
#include "Object/3d/Fbx.h"
#include "Math/Timer.h"
#include "Math/Vector3.h"
#include <memory>

class BaseBossModel
{
protected:
	struct ObjectInfo {
		bool isChange = false;
		std::unique_ptr<Engine::Timer> time;
		float maxTime;
		int changeNum = 0;
		Vector3 before = {};
		Vector3 after = {};
		Vector3 now = {};
	};

	struct AttachInfo {
		std::string name = "";
		ObjectInfo pos = {};
		ObjectInfo scale = {};
		ObjectInfo rota = {};
	};

public:
	virtual ~BaseBossModel() {};

	virtual void Update() {};

	virtual void Draw() {};

	virtual void DrawLightView() {};

	virtual void ChangesPos(const int _num, const float _maxTime, const Vector3& _pos) {};

	virtual void ChangesScale(const int _num, const float _maxTime, const Vector3& _scale) {};

	virtual void ModelReset(const float _maxTime = 15.0f) {};

	virtual void IsAttachEffect(int _num, bool _isAlive) {};

	virtual Vector3 GetPartsPos(const std::string& _num) = 0;

	DirectX::XMFLOAT3 GetModelMove() { return fbxObject->GetModelMove(); }
	void AnimationReset() { fbxObject->AnimationReset(); }
	void SetAnimation(const int _animationNumber) { fbxObject->SetUseAnimation(_animationNumber); }
	Base3D* GetObjectInst() { return fbxObject.get(); }
	void RrameReset() { fbxObject->FrameReset(); }
	void SetMovement(const int _number) { fbxObject->SetUseAnimation(_number); };
	void SetIsRoop(const bool _isRoop) { fbxObject->SetIsRoop(_isRoop); }
	bool GetIsAnimationEnd() { return fbxObject->GetIsAnimationEnd(); }
	void SetPosition(const Vector3& _pos) { bossPos = _pos; };
	Vector3 GetPosition() { return bossPos; };

protected:
	
	std::unique_ptr<FbxModel> model;
	std::unique_ptr<Fbx> fbxObject;
	std::vector<AttachInfo> attachInfo;

	//ç¿ïW
	Vector3 bossPos;
};