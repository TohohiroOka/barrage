#include "Field.h"
#include "GameHelper.h"
#include "Object/3d/collider/Collision.h"
#include "Object/3d/collider/MeshCollider.h"
#include "Object/3d/collider/CollisionAttribute.h"

Field::Field()
{
	//地面
	wallModel = Model::CreateFromOBJ("plane");
	groundModel = Model::CreateFromOBJ("jimen");

	const float scale = GameHelper::Instance()->GetStageSize();
	groundObject = Object3d::Create(groundModel.get());
	groundObject->SetPosition({ 255.0f,1.0f,255.0f });
	groundObject->SetScale({ scale,scale,scale });
	groundObject->SetColor({ 1.0f,1.0f ,1.0f,1.0f });
	groundObject->UpdateWorldMatrix();
	groundObject->DeleteCollider();
	//シャドウマップで影を付ける
	groundObject->SetShadowMap(true);
	groundObject->SetBloom(true);
	// コライダーの追加
	MeshCollider* collider = new MeshCollider;
	groundObject->SetCollider(collider);
	collider->ConstructTriangles(groundObject->GetModel());
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);

	//壁
	const float distPos = scale - 1.0f;
	float distPosY = distPos / 1.9f;
	const std::array<DirectX::XMFLOAT3, 4> pos = { DirectX::XMFLOAT3{distPos,distPosY,0.0f},{distPos,distPosY,distPos * 2.0f} ,{distPos * 2.0f,distPosY,distPos},{0.0f,distPosY,distPos} };
	const std::array<DirectX::XMFLOAT3, 4> rota = { DirectX::XMFLOAT3{90.0f,0.0f,0.0f},{-90.0f,0.0f,0.0f} ,{0.0f,0.0f,90.0f} ,{0.0f,0.0f,-90.0f} };
	for (int i = 0; i < 4; i++) {
		wallObject[i] = Object3d::Create(wallModel.get());
		wallObject[i]->SetRotation(rota[i]);
		wallObject[i]->SetScale({ scale + 14.0f ,scale + 14.0f / 2.0f ,scale + 14.0f });
		wallObject[i]->SetPosition(pos[i]);

		// コライダーの追加
		MeshCollider* collider = new MeshCollider;
		wallObject[i]->SetCollider(collider);
		collider->ConstructTriangles(wallObject[i]->GetModel());
		collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	}

	//背景
	backGround = std::make_unique<BackGround>();

	timer = std::make_unique<Engine::Timer>();
	FieldLine::StaticInitialize();
}

void Field::Update(const DirectX::XMFLOAT3& _playerPos)
{
	groundObject->Update();

	for (auto& i : wallObject) {
		i->Update();
	}
	
	backGround->Update(_playerPos);

	timer->Update();
	if (RandomSign() == 1 && *timer.get() > 2) {
		timer->Reset();
		//一つ追加
		line.emplace_front(std::make_unique<FieldLine>());
	}

	//更新処理
	for (std::forward_list<std::unique_ptr<FieldLine>>::iterator it = line.begin();
		it != line.end(); it++) {
		(*it)->Update();
	}

	//消す
	line.remove_if([](std::unique_ptr<FieldLine>& x) {
		return x->GetIsEnd();
		}
	);

	FieldLine::StaticUpdate();
}

void Field::Draw()
{
	groundObject->Draw();
	backGround->Draw();
	FieldLine::StaticDraw();
}