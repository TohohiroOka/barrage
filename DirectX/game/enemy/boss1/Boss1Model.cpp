#include "Boss1Model.h"

Boss1Model::Boss1Model()
{
	objModel[int(ObjectType::cube)] = Model::CreateFromOBJ("boss1/core");
	objModel[int(ObjectType::cone)] = Model::CreateFromOBJ("boss1/feet");

	model = FbxModel::Create("boss1","Hips");
	fbxObject = Fbx::Create(model.get());
	fbxObject->SetScale({ 0.0011f,0.0011f ,0.0011f });
	fbxObject->SetLight(true);
	fbxObject->SetAnimation(true);
	fbxObject->SetPosition({ GameHelper::Instance()->GetStageSize() / 2.0f,10.0f ,GameHelper::Instance()->GetStageSize() / 2.0f });
	fbxObject->SetIsModelDraw(false);
	fbxObject->SetIsBoneDraw(true);
	fbxObject->SetIsRoop(false);

	std::array<std::string, 2> bone = {
		"Hips","Spine"
	};

	std::array<std::string, 4> boneT = {
		"LowerLeg_L","LowerLeg_R","LowerArm_L","LowerArm_R"
	};

	using namespace DirectX;
	for (int i = 0; i < bone.size(); i++) {
		XMMATRIX world = DirectX::XMMatrixIdentity();
		XMMATRIX matScale = XMMatrixScaling(6.0f, 6.0f, 6.0f);
		world *= matScale;
		fbxObject->SetBoneObject(bone[i], "normal", objModel[int(ObjectType::cube)].get(), world);
	}

	for (int i = 0; i < 4; i++) {
		XMMATRIX world = DirectX::XMMatrixIdentity();
		if (i < 2) {
			XMMATRIX matScale = XMMatrixScaling(6.0f, 9.0f, 6.0f);
			world *= matScale;
		} else if (i == 2) {
			XMMATRIX matScale = XMMatrixScaling(4.0f, 5.0f, 4.0f);
			XMMATRIX matRot = XMMatrixIdentity();
			matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationY(XMConvertToRadians(-90.0f));
			DirectX::XMMATRIX matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

			world *= matScale;
			world *= matRot;
			world *= matTrans;
		} else if (i == 3) {
			XMMATRIX matScale = XMMatrixScaling(4.0f, 5.0f, 4.0f);
			XMMATRIX matRot = XMMatrixIdentity();
			matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationY(XMConvertToRadians(90.0f));
			XMMATRIX matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

			world *= matScale;
			world *= matRot;
			world *= matTrans;
		}
		fbxObject->SetBoneObject(boneT[i], "cube", objModel[int(ObjectType::cone)].get(), world);
	}

	fbxObject->GetBrneObject("normal")->SetOutline(true);
	fbxObject->GetBrneObject("cube")->SetOutline(true);
	fbxObject->GetBrneObject("normal")->SetOutlineColor({0.9f,0.1f,0.1f});
	fbxObject->GetBrneObject("cube")->SetOutlineColor({ 0.9f,0.1f,0.1f });

}

void Boss1Model::Update()
{
	fbxObject->Update();
}

void Boss1Model::Draw()
{
	fbxObject->Draw();
}

void Boss1Model::DrawLightView()
{
	fbxObject->DrawLightView();
}
