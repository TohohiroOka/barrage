#include "Boss1Model.h"

Boss1Model::Boss1Model()
{
	objModel[int(ObjectType::cube)] = Model::CreateFromOBJ("NormalCube");
	objModel[int(ObjectType::cone)] = Model::CreateFromOBJ("cone");

	model = FbxModel::Create("boss1");
	fbxObject = Fbx::Create(model.get());
	fbxObject->SetScale({ 1.0f,1.0f ,1.0f });
	fbxObject->SetLight(true);
	fbxObject->SetAnimation(true);
	fbxObject->SetPosition({ 255.0f / 2.0f,10.0f ,255.0f / 2.0f });
	//fbxObject->SetIsModelDraw(false);
	fbxObject->SetIsBoneDraw(true);

	std::array<std::string, 9> bone = {
	"mixamorig:HeadTop_End","mixamorig:Head","mixamorig:Spine1","mixamorig:Spine","mixamorig:Hips",
	"mixamorig:LeftShoulder",
	"mixamorig:RightShoulder",
	"mixamorig:LeftUpLeg",
	"mixamorig:RightUpLeg"
	};

	std::array<std::string, 4> boneT = {
		"mixamorig:RightLeg","mixamorig:LeftLeg","mixamorig:LeftForeArm","mixamorig:RightForeArm"
	};

	for (int i = 0; i < bone.size(); i++) {
		fbxObject->SetBoneObject(bone[i], "normal", objModel[int(ObjectType::cube)].get());
	}

	using namespace DirectX;
	for (int i = 0; i < 4; i++) {
		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
		if (i == 2) {
			XMMATRIX matScale = XMMatrixScaling(1.0f, 2.0f, 1.0f);
			XMMATRIX matRot = XMMatrixIdentity();
			matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationY(XMConvertToRadians(-90.0f));
			DirectX::XMMATRIX matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

			world = XMMatrixIdentity();
			world *= matScale;
			world *= matRot;
			world *= matTrans;
		} else if (i == 3) {
			XMMATRIX matScale = XMMatrixScaling(1.0f, 2.0f, 1.0f);
			XMMATRIX matRot = XMMatrixIdentity();
			matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationY(XMConvertToRadians(90.0f));
			XMMATRIX matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

			world = XMMatrixIdentity();
			world *= matScale;
			world *= matRot;
			world *= matTrans;
		}
		fbxObject->SetBoneObject(boneT[i], "cube", objModel[int(ObjectType::cube)].get(), world);
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
