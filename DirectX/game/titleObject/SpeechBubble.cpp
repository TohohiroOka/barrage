#include "SpeechBubble.h"
#include "engine/Camera/Camera.h"

SpeechBubble::SpeechBubble(Camera* camera, const std::string& modelName, const DirectX::XMFLOAT3& position, float scale)
{
	//カメラをセット
	this->camera = camera;

	//モデル読み込み
	model = Model::CreateFromOBJ(modelName);

	//オブジェクト生成
	object = Object3d::Create(model.get());
	object->SetCalcWorldMat(true);
	object->SetPosition(position);
	object->SetScale({ scale, scale, scale });
}

void SpeechBubble::Update()
{
	//Y軸ビルボード行列で更新処理
	//視点座標
	DirectX::XMVECTOR eyePosition = XMLoadFloat3(&camera->GetEye());
	//注視点座標
	DirectX::XMVECTOR targetPosition = XMLoadFloat3(&camera->GetTarget());
	//仮の上方向(ゲームカメラは一回転も可能にさせるため軸をずらしている。よってY軸ビルボード用に仮の真上軸が必要)
	DirectX::XMFLOAT3 baseUp(0, 1, 0);
	DirectX::XMVECTOR upVector = XMLoadFloat3(&baseUp);

	//カメラZ軸(視線方向)
	DirectX::XMVECTOR cameraAxisZ = DirectX::XMVectorSubtract(targetPosition, eyePosition);

	//0ベクトルだと向きが定まらないので除外
	assert(!DirectX::XMVector3Equal(cameraAxisZ, DirectX::XMVectorZero()));
	assert(!DirectX::XMVector3IsInfinite(cameraAxisZ));
	assert(!DirectX::XMVector3Equal(upVector, DirectX::XMVectorZero()));
	assert(!DirectX::XMVector3IsInfinite(upVector));

	//ベクトルを正規化
	cameraAxisZ = DirectX::XMVector3Normalize(cameraAxisZ);

	//カメラのX軸(右方向)
	DirectX::XMVECTOR cameraAxisX;
	//X軸は上方向→Z軸の外積で求まる
	cameraAxisX = DirectX::XMVector3Cross(upVector, cameraAxisZ);
	//ベクトルを正規化
	cameraAxisX = DirectX::XMVector3Normalize(cameraAxisX);

	//カメラの軸(上方向)
	DirectX::XMVECTOR cameraAxisY;
	//Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = DirectX::XMVector3Cross(cameraAxisZ, cameraAxisX);

	//カメラ回転行列
	DirectX::XMMATRIX matCameraRot;
	//カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

	//転地により逆行列(逆回転)を計算
	DirectX::XMMATRIX matView = DirectX::XMMatrixTranspose(matCameraRot);

	//視点座標に-1を掛けた座標
	DirectX::XMVECTOR reverseEyePosition = DirectX::XMVectorNegate(eyePosition);
	//カメラの位置からワールド原点へのベクトル(カメラ座標系)
	DirectX::XMVECTOR tX = DirectX::XMVector3Dot(cameraAxisX, reverseEyePosition);	//X成分
	DirectX::XMVECTOR tY = DirectX::XMVector3Dot(cameraAxisY, reverseEyePosition);	//Y成分
	DirectX::XMVECTOR tZ = DirectX::XMVector3Dot(cameraAxisZ, reverseEyePosition);	//Z成分
	//一つのベクトルにまとめる
	DirectX::XMVECTOR translation = DirectX::XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//ビュー行列に平行移動成分を設定
	matView.r[3] = translation;

	//Y軸回りビルボード行列の計算
	//カメラX軸, Y軸, Z軸
	DirectX::XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X軸は共通
	ybillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のY軸
	ybillCameraAxisY = DirectX::XMVector3Normalize(upVector);
	//Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = DirectX::XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	//Y軸回りビルボード行列
	DirectX::XMMATRIX matBillboardY;
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

	//ワールド行列の合成
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(object->GetScale().x, object->GetScale().y, object->GetScale().z);
	DirectX::XMMATRIX rot = DirectX::XMMatrixIdentity();
	rot = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(object->GetRotation().z));
	rot = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(object->GetRotation().x));
	rot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(object->GetRotation().y));
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(object->GetPosition().x, object->GetPosition().y, object->GetPosition().z);
	world *= scale;
	world *= rot;
	world *= matBillboardY;
	world *= trans;

	//Y軸ビルボード行列をセット
	object->SetMatWorld(world);
}

void SpeechBubble::Draw()
{
	object->Draw();
}
