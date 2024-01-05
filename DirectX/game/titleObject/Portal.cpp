#include "Portal.h"
#include "Scene/InterfaceScene.h"
#include "Math/Vector2.h"

Portal::Portal(const Vector3& position, InterfaceScene* changeScene)
{
	//オブジェクト生成
	model = Model::CreateFromOBJ("portal");
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
}

Portal::~Portal()
{
}

void Portal::Update(const PlayerData& playerData)
{
	//ポータルに入れる条件を満たしているかチェック
	isIntoPortal = CheckIntoPortal(playerData) && playerData.onGround && playerData.action == PlayerActionName::MOVE_NORMAL;

	//オブジェクト更新
	object->Update();
}

void Portal::Draw()
{
	//オブジェクト描画
	object->Draw();
}

void Portal::DrawLightView()
{
	object->DrawLightView();
}

bool Portal::CheckIntoPortal(const PlayerData& playerData)
{
	//ポータルに入れる範囲にいなければfalse
	if (playerData.pos.x < intoPortalRangeMin.x) { return false; }
	if (playerData.pos.z < intoPortalRangeMin.z) { return false; }
	if (playerData.pos.x > intoPortalRangeMax.x) { return false; }
	if (playerData.pos.z > intoPortalRangeMax.z) { return false; }

	//プレイヤーがポータルの方向を向いていなければfalse
	//視線先の座標を記憶しておく
	const float playerRotaRadian = DirectX::XMConvertToRadians(playerData.rota.y);
	Vector3 lineSightPos = playerData.pos;
	lineSightPos.x += sinf(playerRotaRadian);
	lineSightPos.z += cosf(playerRotaRadian);

	//視線ベクトルとプレイヤーとポータルの座標の差のベクトルの内積を計算
	Vector2 playerLineSightVec = Vector2{ playerData.pos.x, playerData.pos.z } - Vector2{ lineSightPos.x, lineSightPos.z };
	Vector2 playerToPortalVec = Vector2{ playerData.pos.x, playerData.pos.z } - Vector2{ object->GetPosition().x, object->GetPosition().z };
	playerLineSightVec.normalize();
	playerToPortalVec.normalize();

	//プレイヤーがポータルの入口を見ていなければfalse
	float dot = playerLineSightVec.dot(playerToPortalVec);
	if (dot < 0.75f) { return false; }

	//全ての項目をクリアすればtrue
	return true;
}
