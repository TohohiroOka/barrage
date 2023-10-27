#include "Scene1.h"
#include "Scene/SceneManager.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "Object/2d/DebugText.h"
#include "WindowApp.h"
#include <imgui.h>
#include "Object/3d/collider/Collision.h"
#include "Object/3d/collider/MeshCollider.h"
#include "Object/3d/collider/CollisionAttribute.h"
#include "GameHelper.h"
#include "scene/TitleScene.h"

using namespace DirectX;

const std::array<XMFLOAT4, 2> COLOR = { XMFLOAT4{ 0.0f,0.0f,0.8f,1.0f } ,{ 0.8f,0.0f,0.0f,1.0f } };

void Scene1::Initialize()
{
	Sprite::LoadTexture("gauge", "Resources/SpriteTexture/gauge.png");

	//地形生成
	field = std::make_unique<Field>();

	player = std::make_unique<Player>();

	GameCamera::SetPlayer(player.get());
	debugCamera = DebugCamera::Create({ 300, 40, 0 });
	camera = GameCamera::Create();
	player->SetGameCamera(camera.get());

	//影用光源カメラ初期化
	lightCamera.reset(new LightCamera({ -50, 70, -50 }));
	lightCamera->SetProjectionNum({ 380, 550 }, { -380, -50 });

	Base3D::SetCamera(camera.get());
	Base3D::SetLightCamera(lightCamera.get());

	/*Sprite::LoadTexture("amm", "Resources/amm.jpg");
	sprite = Sprite::Create("amm", {}, { 1059.0f / 5.0f,1500.0f / 5.0f });
	sprite->SetTexSize({ 1059.0f,1500.0f });
	sprite->Update();*/

	boss = std::make_unique<Boss1>();

	ParticleManager::SetCamera(camera.get());

	fbxModel1 = FbxModel::Create("uma_j");
	fbxModel2 = FbxModel::Create("uma4");
	objmodel1 = Model::CreateFromOBJ("NormalCube");
	objmodel2 = Model::CreateFromOBJ("cone");
	fbxModel1->isAnimation = true;
	fbxModel2->isAnimation = true;
	//fbxModel->SetIsBoneDraw(true);
	fbx = Fbx::Create(fbxModel1.get());
	fbx->SetMotionBlendModel(fbxModel2.get());
	fbx->SetScale({ 1.0f,1.0f ,1.0f });
	fbx->SetLight(true);
	fbx->SetAnimation(true);

	std::array<std::string,9> bone = {
		//胴（上から）
		"mixamorig:HeadTop_End","mixamorig:Head","mixamorig:Spine1","mixamorig:Spine","mixamorig:Hips",
		//右手（外から）
		"mixamorig:LeftShoulder",
		//左手（外から）
		"mixamorig:RightShoulder",
		//右足
		"mixamorig:LeftUpLeg",
		//左足
		"mixamorig:RightUpLeg"
	};

	std::array<std::string, 4> boneT = {
		//足先
		"mixamorig:RightLeg","mixamorig:LeftLeg","mixamorig:LeftForeArm","mixamorig:RightForeArm"
	};

	for (int i = 0; i < bone.size(); i++) {
		fbx->SetBoneObject(bone[i],"normal", objmodel1.get());
	}
	for (int i = 0; i < 4; i++) {
		DirectX::XMMATRIX world = XMMatrixIdentity();
		if (i ==2) {
			// スケール、回転、平行移動行列の計算
			DirectX::XMMATRIX matScale = XMMatrixScaling(1.0f, 2.0f, 1.0f);
			DirectX::XMMATRIX matRot = XMMatrixIdentity();
			matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationY(XMConvertToRadians(-90.0f));
			DirectX::XMMATRIX matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

			world = XMMatrixIdentity(); // 変形をリセット
			world *= matScale; // ワールド行列にスケーリングを反映
			world *= matRot; // ワールド行列に回転を反映
			world *= matTrans; // ワールド行列に平行移動を反映
		} else if(i == 3) {
			// スケール、回転、平行移動行列の計算
			DirectX::XMMATRIX matScale = XMMatrixScaling(1.0f, 2.0f, 1.0f);
			DirectX::XMMATRIX matRot = XMMatrixIdentity();
			matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));
			matRot *= XMMatrixRotationY(XMConvertToRadians(90.0f));
			DirectX::XMMATRIX matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

			world = XMMatrixIdentity(); // 変形をリセット
			world *= matScale; // ワールド行列にスケーリングを反映
			world *= matRot; // ワールド行列に回転を反映
			world *= matTrans; // ワールド行列に平行移動を反映
		}
		fbx->SetBoneObject(boneT[i], "cube", objmodel2.get(), world);
	}

	isBlend = false;
	stop = false;

	gameoverUi.Initialize();
}

void Scene1::Update()
{
	DirectInput* input = DirectInput::GetInstance();

	GameHelper::Instance()->SetStop(stop);

	fbx->Update();
	player->Update();
	field->Update();
	boss->SetTargetPos(player->GetPosition());
	boss->Update();

	CollisionCheck();

	//カメラ更新
	if (isNormalCamera) {
		camera->Update();
		if (DirectInput::GetInstance()->TriggerKey(DIK_RETURN)) {
			isNormalCamera = !isNormalCamera;
			Base3D::SetCamera(debugCamera.get());
		}
	}
	else {
		debugCamera->Update();
		Base3D::SetCamera(debugCamera.get());
		if (DirectInput::GetInstance()->TriggerKey(DIK_RETURN)) {
			isNormalCamera = !isNormalCamera;
			Base3D::SetCamera(camera.get());
		}
	}
	//camera->Update();
	lightCamera->Update();

	//デバッグ用シーン切り替え
	if (DirectInput::GetInstance()->ReleaseKey(DIK_1)) {
		TitleScene* titleScene = new TitleScene;
		SceneManager::SetNextScene(titleScene);
	}

	gameoverUi.Update();
	//体力0でゲームオーバー表示
	//デバッグ用ゲームオーバー表示
	if (DirectInput::GetInstance()->TriggerKey(DIK_F4)) { gameoverUi.ResetGameOverUI(); }
	if (DirectInput::GetInstance()->TriggerKey(DIK_4)) { gameoverUi.StartGameOverUI(); }
}

void Scene1::Draw(const int _cameraNum)
{
	field->Draw();

	fbx->BoneDraw();
	//gobject->ColliderDraw();

	player->Draw();
	boss->Draw();
}

void Scene1::DrawLightView(const int _cameraNum)
{
	player->DrawLightView();
	field->Draw();
}

void Scene1::NonPostEffectDraw(const int _cameraNum)
{
	//スプライト
	if (_cameraNum == 0) {
		DebugText::GetInstance()->DrawAll();
		//sprite->Draw();
	}

	gameoverUi.Draw();
}

void Scene1::Finalize()
{
}

void Scene1::ImguiDraw()
{
	Vector3 ppos = player->GetPosition();
	XMFLOAT3 cameraPos = {};
	XMFLOAT3 cameraTarget = {};
	if (isNormalCamera) {
		cameraPos = camera->GetEye();
		cameraTarget = camera->GetTarget();
	}
	else {
		cameraPos = debugCamera->GetEye();
		cameraTarget = debugCamera->GetTarget();
	}

	ImGui::Begin("debug imgui");
	ImGui::SetWindowSize(ImVec2(300, 300), ImGuiCond_::ImGuiCond_FirstUseEver);

	ImGui::Text("Camera Pos    [ %f : %f : %f ]", cameraPos.x, cameraPos.y, cameraPos.z);
	ImGui::Text("Camera Target [ %f : %f : %f ]", cameraTarget.x, cameraTarget.y, cameraTarget.z);
	ImGui::Text("Player Pos    [ %f : %f : %f ]", ppos.x, ppos.y, ppos.z);
	if (camera->GetisLockon()) { ImGui::Text("lockon  true"); }
	else { ImGui::Text("lockon  false"); }
	ImGui::Text("Player Boss Length [ %f ]", boss->GetLength());
	ImGui::Text("%d : %d ", player->GetJumpMaxNum(), player->GetJumpCount());

	ImGui::SliderFloat("blend rate", &rate, 0.0f, 1.0f);
	ImGui::Checkbox("stop", &stop);

	ImGui::End();
}

void Scene1::FrameReset()
{
	boss->FrameReset();
	fbx->FrameReset();
}

void Scene1::CollisionCheck()
{
#pragma region プレイヤーと敵の攻撃の衝突判定
	{
		Sphere playerSphere;
		playerSphere.center = { player->GetPosition().x, player->GetPosition().y,player->GetPosition().z, 1.0f };
		playerSphere.radius = player->GetObject3d()->GetScale().x;

		std::vector<BaseAction::AttackCollision> bossAttackDatas;
		boss->GetBaseAction()->GetAttackCollision(bossAttackDatas);

		for (auto& i : bossAttackDatas) {
			Sphere attackSphere;
			attackSphere.center = { i.pos.x, i.pos.y, i.pos.z, 1.0f };
			attackSphere.radius = i.radius;
			if (Collision::CheckSphere2Sphere(playerSphere, attackSphere)) {
				player->Damage(10, i.pos);
				camera->ShakeStart(10, 10);
				break;
			}
		}
	}
#pragma endregion

#pragma region プレイヤーの攻撃と敵の衝突判定
	{
		//プレイヤーの攻撃がある && 攻撃が判定を有効にしていたら判定を取る
		if (player->GetAttackAction()) {
			if (player->GetAttackAction()->GetisCollisionValid()) {
				Sphere enemySphere;
				enemySphere.center = { boss->GetCenter()->GetPosition().x, boss->GetCenter()->GetPosition().y, boss->GetCenter()->GetPosition().z, 1.0f };
				enemySphere.radius = boss->GetCenter()->GetScale().x * 2;

				Sphere attackSphere;
				attackSphere.center = player->GetAttackAction()->GetAttackCollisionData().center;
				attackSphere.radius = player->GetAttackAction()->GetAttackCollisionData().radius * 2;
				if (Collision::CheckSphere2Sphere(enemySphere, attackSphere)) {
					boss->Damage(player->GetAttackAction()->GetAttackCollisionData().power);
				}
			}
		}
	}
#pragma endregion

#pragma region カメラのロックオンターゲット設定
	{
		//カメラがロックオンターゲットを検出している場合のみ判定
		if (camera->GetisLockonStart()) {
			//敵座標
			XMFLOAT2 pos = boss->GetCenter()->GetScreenPosition();

			//敵のスクリーン座標が検出対象範囲内なら処理
			const float targetScreenDistance = 100;
			const bool isInsideTargetScreen = (pos.x <= WindowApp::GetWindowWidth() - targetScreenDistance && pos.x >= targetScreenDistance &&
				pos.y <= WindowApp::GetWindowHeight() - targetScreenDistance && pos.y >= targetScreenDistance);
			if (isInsideTargetScreen) {
				//ロックオン対象を確定させる
				camera->Lockon(boss->GetCenter());
			}
		}
	}
#pragma endregion
}
