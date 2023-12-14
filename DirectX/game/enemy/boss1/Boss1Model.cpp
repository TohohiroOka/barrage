#include "Boss1Model.h"
#include "Math//Easing/Easing.h"

const float transformScale = 8.5f;

std::array<std::string, 2> bone = {
	"Hips","Spine"
};

std::array<std::string, 4> boneT = {
	"LowerLeg_L","LowerLeg_R","LowerArm_L","LowerArm_R"
};

Boss1Model::Boss1Model()
{
	objModel[int(ObjectType::core)] = Model::CreateFromOBJ("boss1/core");
	objModel[int(ObjectType::sword)] = Model::CreateFromOBJ("boss1/feet");

	model = FbxModel::Create("boss1","Hips");
	fbxObject = Fbx::Create(model.get());
	fbxObject->SetScale({ 0.0011f,0.0011f ,0.0011f });
	fbxObject->SetLight(true);
	fbxObject->SetAnimation(true);
	fbxObject->SetPosition({ GameHelper::Instance()->GetStageSize() / 2.0f,10.0f ,GameHelper::Instance()->GetStageSize() / 2.0f });
	fbxObject->SetIsModelDraw(false);
	fbxObject->SetIsBoneDraw(true);
	fbxObject->SetIsRoop(false);

	using namespace DirectX;
	attachInfo.resize(int(bone.size()) + int(boneT.size()) + 1);
	for (int i = 0; i < bone.size(); i++) {
		attachInfo[i].pos.now = {};
		attachInfo[i].scale.now = { 6.0f, 6.0f, 6.0f };
		attachInfo[i].rota.now = {};
		XMMATRIX world = DirectX::XMMatrixIdentity();
		XMMATRIX matScale = XMMatrixScaling(attachInfo[i].scale.now.x, attachInfo[i].scale.now.y, attachInfo[i].scale.now.z);
		world *= matScale;
		attachInfo[i].name = fbxObject->SetBoneObject(bone[i], "normal", objModel[int(ObjectType::core)].get(), world);
	}

	std::string getName = "null";

	for (int i = 0; i < boneT.size(); i++) {
		int number = int(bone.size()) + i;
		attachInfo[number].pos.now = {};
		XMMATRIX world = DirectX::XMMatrixIdentity();
		if (i < 2) {
			attachInfo[number].scale.now = { 6.0f, 9.0f, 6.0f };
			attachInfo[number].rota.now = {};
			XMMATRIX matScale = XMMatrixScaling(attachInfo[number].scale.now.x, attachInfo[number].scale.now.y, attachInfo[number].scale.now.z);
			world *= matScale;
		} else if (i == 2) {
			attachInfo[number].scale.now = { 4.0f, 5.0f, 4.0f };
			attachInfo[number].rota.now = { 0.0f ,-90.0f ,0.0f };
			XMMATRIX matScale = XMMatrixScaling(attachInfo[number].scale.now.x, attachInfo[number].scale.now.y, attachInfo[number].scale.now.z);
			XMMATRIX matRot = XMMatrixIdentity();
			matRot *= XMMatrixRotationZ(XMConvertToRadians(attachInfo[number].rota.now.z));
			matRot *= XMMatrixRotationX(XMConvertToRadians(attachInfo[number].rota.now.x));
			matRot *= XMMatrixRotationY(XMConvertToRadians(attachInfo[number].rota.now.y));

			world *= matScale;
			world *= matRot;
		} else if (i == 3) {
			attachInfo[number].scale.now = { 4.0f, 5.0f, 4.0f };
			attachInfo[number].rota.now = { 0.0f ,90.0f ,0.0f };
			XMMATRIX matScale = XMMatrixScaling(attachInfo[number].scale.now.x, attachInfo[number].scale.now.y, attachInfo[number].scale.now.z);
			XMMATRIX matRot = XMMatrixIdentity();
			matRot *= XMMatrixRotationZ(XMConvertToRadians(attachInfo[number].rota.now.z));
			matRot *= XMMatrixRotationX(XMConvertToRadians(attachInfo[number].rota.now.x));
			matRot *= XMMatrixRotationY(XMConvertToRadians(attachInfo[number].rota.now.y));

			world *= matScale;
			world *= matRot;

			getName = "larm1";
		}
		attachInfo[number].name = fbxObject->SetBoneObject(boneT[i], "cube", objModel[int(ObjectType::sword)].get(), world, true, getName);
	}

	{
		int number = int(bone.size()) + int(boneT.size());
		attachInfo[number].pos.now = { 0.0f, attachInfo[5].scale.now.y * transformScale, 0.0f };
		attachInfo[number].scale.now = {};
		attachInfo[number].rota.now = { 0.0f ,90.0f ,0.0f };
		XMMATRIX world = DirectX::XMMatrixIdentity();
		XMMATRIX matScale = XMMatrixScaling(attachInfo[number].scale.now.x, attachInfo[number].scale.now.y, attachInfo[number].scale.now.z);
		XMMATRIX matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(attachInfo[number].rota.now.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(attachInfo[number].rota.now.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(attachInfo[number].rota.now.y));
		XMMATRIX matTrans = XMMatrixTranslation(attachInfo[number].pos.now.x, attachInfo[number].pos.now.y, attachInfo[number].pos.now.z);

		world *= matScale;
		world *= matRot;
		world *= matTrans;

		getName = "larm2";
		attachInfo[number].name = fbxObject->SetBoneObject(boneT[3], "cube", objModel[int(ObjectType::sword)].get(), world, false, getName);
	}

	for (auto& i : attachInfo) {
		i.pos.time = std::make_unique<Engine::Timer>();
		i.scale.time = std::make_unique<Engine::Timer>();
	}

	fbxObject->GetBrneObject("normal")->SetOutline(true);
	fbxObject->GetBrneObject("cube")->SetOutline(true);
	fbxObject->GetBrneObject("normal")->SetOutlineColor({0.9f,0.1f,0.1f});
	fbxObject->GetBrneObject("cube")->SetOutlineColor({ 0.9f,0.1f,0.1f });

	swordEffect = std::make_unique<AttachEmitter>("effect");
}

void Boss1Model::Update()
{
	ChangeInfo();
	fbxObject->Update();
	swordEffect->Add(fbxObject->GetAttachPos("larm1"), fbxObject->GetAttachPos("larm2"));
	swordEffect->Update();
}

void Boss1Model::Draw()
{
	swordEffect->Draw();
	fbxObject->Draw();
}

void Boss1Model::DrawLightView()
{
	fbxObject->DrawLightView();
}

void Boss1Model::ChangesPos(const int _num, const float _maxTime, const Vector3& _pos)
{
	attachInfo[_num].pos.isChange = true;
	attachInfo[_num].pos.time->Reset();
	attachInfo[_num].pos.maxTime = _maxTime;
	attachInfo[_num].pos.before = attachInfo[_num].pos.now;
	attachInfo[_num].pos.after = _pos;
}

void Boss1Model::ChangesScale(const int _num, const float _maxTime, const Vector3& _scale)
{
	attachInfo[_num].scale.isChange = true;
	attachInfo[_num].scale.time->Reset();
	attachInfo[_num].scale.maxTime = _maxTime;
	attachInfo[_num].scale.before = attachInfo[_num].scale.now;
	attachInfo[_num].scale.after = _scale;

	if (_num == int(AttachName::LowerArm_R)) {
		ChangesPos(int(AttachName::LowerArm_R_Non), _maxTime,
		{ 0.0f, attachInfo[int(AttachName::LowerArm_R)].scale.now.y * transformScale, 0.0f });
	}
}

void Boss1Model::ModelReset()
{
	using namespace DirectX;
	for (int i = 0; i < bone.size(); i++) {
		attachInfo[i].pos.isChange = true;
		attachInfo[i].pos.after = {};
		attachInfo[i].scale.isChange = true;
		attachInfo[i].scale.after = { 6.0f, 6.0f, 6.0f };
	}

	for (int i = 0; i < boneT.size(); i++) {
		int number = int(bone.size()) + i;
		attachInfo[number].pos.isChange = true;
		attachInfo[number].pos.after = {};
		attachInfo[number].scale.isChange = true;
		if (i < 2) {
			attachInfo[number].scale.after = { 6.0f, 9.0f, 6.0f };
		} else if (i == 2) {
			attachInfo[number].scale.after = { 4.0f, 5.0f, 4.0f };
		} else if (i == 3) {
			attachInfo[number].scale.after = { 4.0f, 10.0f, 4.0f };
		}
	}

	{
		int number = int(bone.size()) + int(boneT.size());
		attachInfo[number].pos.isChange = true;
		attachInfo[number].pos.after = { 0.0f, attachInfo[5].scale.now.y * transformScale, 0.0f };
		attachInfo[number].scale.isChange = true;
		attachInfo[number].scale.after = { 0.0f, 0.0f, 0.0f };
	}
}

void Boss1Model::ChangeInfo()
{
	for (auto& i : attachInfo) {
		//座標変更
		if (i.pos.isChange) {
			if (i.pos.now == i.pos.after || *i.pos.time.get() > i.pos.maxTime) {
				i.pos.isChange = false;
			}else{
				float rate = *i.pos.time.get() / i.pos.maxTime;
				i.pos.now.x = Easing::InExpo(i.pos.before.x, i.pos.after.x, rate);
				i.pos.now.y = Easing::InExpo(i.pos.before.y, i.pos.after.y, rate);
				i.pos.now.z = Easing::InExpo(i.pos.before.z, i.pos.after.z, rate);
				i.pos.time->Update();
			}
		}
		//大きさ変更
		if (i.scale.isChange) {
			if (i.scale.now == i.scale.after || *i.scale.time.get() > i.scale.maxTime) {
				i.scale.isChange = false;
			} else {
				float rate = *i.scale.time.get() / i.scale.maxTime;
				i.scale.now.x = Easing::InExpo(i.scale.before.x, i.scale.after.x, rate);
				i.scale.now.y = Easing::InExpo(i.scale.before.y, i.scale.after.y, rate);
				i.scale.now.z = Easing::InExpo(i.scale.before.z, i.scale.after.z, rate);
				i.scale.time->Update();
			}
		}
	}

	//変更を適応
	using namespace DirectX;
	for (auto& i : attachInfo) {
		if (!i.pos.isChange && !i.scale.isChange) { continue; }
		XMMATRIX world = DirectX::XMMatrixIdentity();
		XMMATRIX matScale = XMMatrixScaling(i.scale.now.x, i.scale.now.y, i.scale.now.z);
		XMMATRIX matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(i.rota.now.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(i.rota.now.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(i.rota.now.y));
		XMMATRIX matTrans = XMMatrixTranslation(i.pos.now.x, i.pos.now.y, i.pos.now.z);

		world *= matScale;
		world *= matRot;
		world *= matTrans;

		fbxObject->ChangeBoneObjectInfo(i.name, world);
	}
}
