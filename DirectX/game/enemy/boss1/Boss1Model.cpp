#include "Boss1Model.h"

Boss1Model::Boss1Model()
{
	objModel[int(ObjectType::cube)] = Model::CreateFromOBJ("core");
	objModel[int(ObjectType::cone)] = Model::CreateFromOBJ("feet");

	model = FbxModel::Create("boss1","Hips");
	fbxObject = Fbx::Create(model.get());
	fbxObject->SetScale({ 0.001f,0.001f ,0.001f });
	fbxObject->SetLight(true);
	fbxObject->SetAnimation(true);
	fbxObject->SetPosition({ 255.0f / 2.0f,10.0f ,255.0f / 2.0f });
	fbxObject->SetIsModelDraw(false);
	fbxObject->SetIsBoneDraw(true);

	//std::array<std::string, 5> bone = {
	//"mixamorig:HeadTop_End","mixamorig:Head","mixamorig:Spine1","mixamorig:Spine","mixamorig:Hips",
	////"mixamorig:LeftShoulder",
	////"mixamorig:RightShoulder",
	////"mixamorig:LeftUpLeg",
	////"mixamorig:RightUpLeg"
	//};

	//std::array<std::string, 4> boneT = {
	//	"mixamorig:RightLeg","mixamorig:LeftLeg","mixamorig:LeftForeArm","mixamorig:RightForeArm"
	//};

	std::array<std::string, 2> bone = {
		"Hips","Spine"
	};

	std::array<std::string, 4> boneT = {
		"LowerLeg_L","LowerLeg_R","LowerArm_L","LowerArm_R"
	};

	using namespace DirectX;
	for (int i = 0; i < bone.size(); i++) {
		XMMATRIX world = DirectX::XMMatrixIdentity();
		XMMATRIX matScale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
		world *= matScale;
		fbxObject->SetBoneObject(bone[i], "normal", objModel[int(ObjectType::cube)].get(), world);
	}

	for (int i = 0; i < 4; i++) {
		XMMATRIX world = DirectX::XMMatrixIdentity();
		if (i < 2) {
			XMMATRIX matScale = XMMatrixScaling(5.0f, 9.0f, 5.0f);
			world *= matScale;
		} else if (i == 2) {
			XMMATRIX matScale = XMMatrixScaling(3.0f, 5.0f, 3.0f);
			XMMATRIX matRot = XMMatrixIdentity();
			matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationY(XMConvertToRadians(-90.0f));
			DirectX::XMMATRIX matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

			world *= matScale;
			world *= matRot;
			world *= matTrans;
		} else if (i == 3) {
			XMMATRIX matScale = XMMatrixScaling(3.0f, 5.0f, 3.0f);
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
