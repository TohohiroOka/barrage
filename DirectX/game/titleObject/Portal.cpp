#include "Portal.h"
#include "Scene/InterfaceScene.h"
#include "Math/Vector2.h"
#include "Object/3d/collider/Collision.h"

Portal::Portal(const Vector3& position, InterfaceScene* changeScene)
{
	//オブジェクト生成
	model = Model::CreateFromOBJ("warp");
	object = Object3d::Create(model.get());
	object->SetPosition(position);
	const float size = 10.0f;
	object->SetScale({ size, size, size });
	object->SetShadowMap(true);

	//ポータルに入れる範囲の最小値と最大値をセット
	intoPortalRangeMin.x = position.x - size;
	intoPortalRangeMin.z = position.z - size - 7.5f;
	intoPortalRangeMax.x = position.x + size;
	intoPortalRangeMax.z = position.z - size / 2;

	//変更後のシーンをセット
	this->changeScene = changeScene;

	effect = std::make_unique<PortalEffect>(position,Vector2(12.0f, 21.0f));
}

Portal::~Portal()
{
}

void Portal::Update(const PlayerData& playerData)
{
	//ポータルに入れる条件を満たしているかチェック
	isIntoPortal = CheckIntoPortalArea(playerData) && CheckLineSightInPortal(playerData) && playerData.onGround && playerData.action == PlayerActionName::MOVE_NORMAL;

	//オブジェクト更新
	object->Update();

	effect->AddParticle();
	effect->Update();
}

void Portal::Draw()
{
	//オブジェクト描画
	object->Draw();

	effect->Draw();
}

void Portal::DrawLightView()
{
	object->DrawLightView();
}

bool Portal::CheckIntoPortalArea(const PlayerData& playerData)
{
	//ポータルに入れる範囲にいなければfalse
	if (playerData.pos.x < intoPortalRangeMin.x) { return false; }
	if (playerData.pos.z < intoPortalRangeMin.z) { return false; }
	if (playerData.pos.x > intoPortalRangeMax.x) { return false; }
	if (playerData.pos.z > intoPortalRangeMax.z) { return false; }

	//全ての項目をクリアすればtrue
	return true;
}

bool Portal::CheckLineSightInPortal(const PlayerData& playerData)
{
	//視線先の座標を記憶しておく
	const float playerRotaRadian = DirectX::XMConvertToRadians(playerData.rota.y);
	Vector3 lineSightPos = playerData.pos;
	lineSightPos.x += sinf(playerRotaRadian);
	lineSightPos.z += cosf(playerRotaRadian);

	//視線ベクトルとプレイヤーとポータルの座標の差のベクトルの内積を計算
	Vector3 playerLineSightVec = Vector3{ lineSightPos.x, 0, lineSightPos.z } - Vector3{ playerData.pos.x, 0, playerData.pos.z };
	playerLineSightVec.normalize();

	//視線方向のレイを生成
	Segment lineSightRay;
	lineSightRay.start = { playerData.pos.x, playerData.pos.y, playerData.pos.z, 1 };
	lineSightRay.dir = { playerLineSightVec.x, playerLineSightVec.y, playerLineSightVec.z, 1 };
	//ポータルの球を生成
	Sphere portalSphere;
	portalSphere.center = { object->GetPosition().x, object->GetPosition().y, object->GetPosition().z, 1 };
	portalSphere.radius = object->GetScale().x * 1.1f;

	//視線レイとポータル球が衝突していなければfalse
	if (!Collision::CheckRay2Sphere(lineSightRay, portalSphere)) { return false; }

	return true;
}
