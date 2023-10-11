#include "Field.h"
#include "GameHelper.h"
#include "Object/3d/collider/Collision.h"
#include "Object/3d/collider/MeshCollider.h"
#include "Object/3d/collider/CollisionAttribute.h"

Field::Field()
{
	const std::string jimen = "jimen.png";
	const std::string kabe = "kabe.png";

	//地面
	float groundScale = 2.0f;
	groundModel = TerrainModel::Create("heightmap3.bmp", 30.0f,
		{ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 1.0f, jimen, kabe);
	groundObject = HeightMap::Create(groundModel.get());
	groundObject->SetScale({ groundScale ,groundScale ,groundScale });
	groundObject->UpdateWorldMatrix();
	groundObject->DeleteCollider();
	// コライダーの追加
	MeshCollider* collider = new MeshCollider;
	groundObject->SetCollider(collider);
	collider->ConstructTriangles(groundObject->GetModel());
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);

	//外枠
	float outsideScale = 6.0f;
	outsideModel = TerrainModel::Create("heightmap6.bmp", 3.0f,
		{ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 1.0f, kabe, kabe);
	outsideObject = HeightMap::Create(outsideModel.get());
	outsideObject->SetScale({ outsideScale ,outsideScale ,outsideScale });
	float outPos = (outsideScale * 256.0f / 2.0f) - groundScale * 256.0f / 2.0f;
	outsideObject->SetPosition({ -outPos,0.0f,-outPos });
	outsideObject->UpdateWorldMatrix();

	//壁
	wallModel=Model::CreateFromOBJ("plane");
	float distPos = 255.0f;
	float distPosY = distPos / 1.9f;
	const std::array<DirectX::XMFLOAT3, 4> pos = { DirectX::XMFLOAT3{distPos,distPosY,0.0f},{distPos,distPosY,distPos * 2.0f} ,{distPos * 2.0f,distPosY,distPos},{0.0f,distPosY,distPos} };
	const std::array<DirectX::XMFLOAT3, 4> rota = { DirectX::XMFLOAT3{90.0f,0.0f,0.0f},{-90.0f,0.0f,0.0f} ,{0.0f,0.0f,90.0f} ,{0.0f,0.0f,-90.0f} };
	for (int i = 0; i < 4; i++) {
		wallObject[i] = Object3d::Create(wallModel.get());
		wallObject[i]->SetRotation(rota[i]);
		wallObject[i]->SetScale({ 270.0f ,270.0f / 2.0f ,270.0f });
		wallObject[i]->SetPosition(pos[i]);

		// コライダーの追加
		MeshCollider* collider = new MeshCollider;
		wallObject[i]->SetCollider(collider);
		collider->ConstructTriangles(wallObject[i]->GetModel());
		collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	}
}

void Field::Update()
{
	groundObject->Update();
	outsideObject->Update();

	for (auto& i : wallObject) {
		i->Update();
	}
}

void Field::Draw()
{
	groundObject->Draw();
	outsideObject->Draw();

	//for (auto& i : wallObject) {
	//	i->Draw();
	//}
}