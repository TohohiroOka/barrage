#include "Field.h"
#include "GameHelper.h"
#include "Object/3d/collider/Collision.h"
#include "Object/3d/collider/MeshCollider.h"
#include "Object/3d/collider/CollisionAttribute.h"

using namespace DirectX;

Field::Field()
{
	//地面
	wallModel = Model::CreateFromOBJ("plane");
	groundModel = Model::CreateFromOBJ("jimen");

	const float scale = GameHelper::Instance()->GetStageSize();
	groundObject = Object3d::Create(groundModel.get());
	groundObject->SetPosition({ scale,1.0f,scale });
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
	const std::array<XMFLOAT3, 4> pos = { XMFLOAT3{distPos,distPosY,0.0f},{distPos,distPosY,distPos * 2.0f} ,{distPos * 2.0f,distPosY,distPos},{0.0f,distPosY,distPos} };
	const std::array<XMFLOAT3, 4> rota = { XMFLOAT3{90.0f,0.0f,0.0f},{-90.0f,0.0f,0.0f} ,{0.0f,0.0f,90.0f} ,{0.0f,0.0f,-90.0f} };
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

	//壁（段差）
	blockModel = Model::CreateFromOBJ("NormalCube");
	enclosure = InstanceObject::Create(blockModel.get());
	enclosure->SetLight(false);

	const float wallScale = 1.0f;
	const std::array<XMFLOAT3, 4> enclosurePos = { 
	XMFLOAT3{scale,wallScale,-wallScale*1.5f},{scale,wallScale,scale * 2.0f + wallScale * 1.5f},{-wallScale * 1.5f,wallScale,scale},{scale * 2.0f + wallScale * 1.5f,wallScale,scale} };
	const std::array<XMFLOAT3, 4> enclosureScale =
	{ XMFLOAT3{scale + 10.0f,wallScale,wallScale},
	{scale + 10.0f,wallScale,wallScale},
	{wallScale,wallScale,scale + 10.0f},
	{wallScale,wallScale,scale + 10.0f}
	};

	for (int i = 0; i < 8; i++) {
		XMMATRIX matScale = XMMatrixScaling(enclosureScale[i % 4].x, enclosureScale[i % 4].y, enclosureScale[i % 4].z);
		XMMATRIX matRot = XMMatrixIdentity();
		XMMATRIX matTrans = XMMatrixTranslation(enclosurePos[i % 4].x, enclosurePos[i % 4].y * (1 + 4 * (i / 4)), enclosurePos[i % 4].z);

		enclosureWorld[i] = XMMatrixIdentity();
		enclosureWorld[i] *= matScale;
		enclosureWorld[i] *= matRot;
		enclosureWorld[i] *= matTrans;
	}
}

void Field::Update(const DirectX::XMFLOAT3& _playerPos, const DirectX::XMFLOAT3& _cameraPos)
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

	WallDrawSet(_playerPos, _cameraPos);
	enclosure->Update();

	FieldLine::StaticUpdate();
}

void Field::Draw()
{
	groundObject->Draw();
	backGround->Draw();
	enclosure->Draw();
	FieldLine::StaticDraw();
}

void Field::WallDrawSet(const DirectX::XMFLOAT3& _playerPos, const DirectX::XMFLOAT3& _cameraPos)
{
	const XMFLOAT3 c_p = _playerPos - _cameraPos;
	//左
	if (c_p.x <= 0 || _playerPos.x > 30.0f) {
		enclosure->DrawInstance(enclosureWorld[2], { 0.5f,0.5f ,0.5f ,1.0f });
		enclosure->DrawInstance(enclosureWorld[6], { 0.5f,0.5f ,0.5f ,1.0f });
	}
	if (c_p.x >= 0 || _playerPos.x < GameHelper::Instance()->GetStageSize() * 2.0f - 30.0f) {
		enclosure->DrawInstance(enclosureWorld[3], { 0.5f,0.5f ,0.5f ,1.0f });
		enclosure->DrawInstance(enclosureWorld[7], { 0.5f,0.5f ,0.5f ,1.0f });
	}
	if (c_p.z <= 0 || _playerPos.z > 30.0f) {
		enclosure->DrawInstance(enclosureWorld[0], { 0.5f,0.5f ,0.5f ,1.0f });
		enclosure->DrawInstance(enclosureWorld[4], { 0.5f,0.5f ,0.5f ,1.0f });
	}
	if (c_p.z >= 0 || _playerPos.z < GameHelper::Instance()->GetStageSize() * 2.0f - 30.0f) {
		enclosure->DrawInstance(enclosureWorld[1], { 0.5f,0.5f ,0.5f ,1.0f });
		enclosure->DrawInstance(enclosureWorld[5], { 0.5f,0.5f ,0.5f ,1.0f });
	}
}

float Field::SetAlpha(const float _c_p, const float _p)
{
	float alpha = 1.0;

	if (_p > 40.0f) { return 1.0f; }
	if (fabs(_c_p) > 10) { return 0.0f; }

	alpha = fabs(_c_p) / 10.0f;
	alpha *= _p / 41.0f;

	return alpha;
}
