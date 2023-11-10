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
#include "effect/BulletEffect.h"

using namespace DirectX;

const std::array<XMFLOAT4, 2> COLOR = { XMFLOAT4{ 0.0f,0.0f,0.8f,1.0f } ,{ 0.8f,0.0f,0.0f,1.0f } };

void Scene1::Initialize()
{
	Sprite::LoadTexture("gauge", "Resources/SpriteTexture/gauge.png");

	//�n�`����
	field = std::make_unique<Field>();

	player = std::make_unique<Player>();

	GameCamera::SetPlayer(player.get());
	debugCamera = DebugCamera::Create({ 300, 40, 0 });
	camera = std::make_unique<GameCamera>();
	player->SetGameCamera(camera.get());

	//�e�p�����J����������
	lightCamera = std::make_unique<LightCamera>(Vector3{ 205, 200, 204 }, Vector3{ 205, 0, 205 });
	const float projectionSize = 1.5f;
	lightCamera->SetProjectionNum({ projectionSize * (float)WindowApp::GetWindowWidth() / 5, projectionSize * (float)WindowApp::GetWindowHeight() / 5 },
		{ -projectionSize * (float)WindowApp::GetWindowWidth() / 5, -projectionSize * (float)WindowApp::GetWindowHeight() / 5 });

	Base3D::SetCamera(camera.get());
	Base3D::SetLightCamera(lightCamera.get());

	/*Sprite::LoadTexture("amm", "Resources/amm.jpg");
	sprite = Sprite::Create("amm", {}, { 1059.0f / 5.0f,1500.0f / 5.0f });
	sprite->SetTexSize({ 1059.0f,1500.0f });
	sprite->Update();*/

	boss = std::make_unique<Boss1>();

	ParticleManager::SetCamera(camera.get());

	stop = false;

	gameoverUi.Initialize();


	actionInputConfig = std::make_unique<ActionInputConfig>();

	BulletEffect::LoadResources();

}

void Scene1::Update()
{
	DirectInput* input = DirectInput::GetInstance();

	GameHelper::Instance()->SetStop(stop);

	if (!isInputConfigMode) {
		player->Update();
		field->Update();
		boss->SetTargetPos(player->GetPosition());
		boss->Update();

		CollisionCheck();

		//�J�����X�V
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

		//�f�o�b�O�p�V�[���؂�ւ�
		if (DirectInput::GetInstance()->ReleaseKey(DIK_1)) {
			TitleScene* titleScene = new TitleScene;
			SceneManager::SetNextScene(titleScene);
		}

		gameoverUi.Update();
		//�̗�0�ŃQ�[���I�[�o�[�\��
		//�f�o�b�O�p�Q�[���I�[�o�[�\��
		if (DirectInput::GetInstance()->TriggerKey(DIK_F4)) { gameoverUi.ResetGameOverUI(); }
		//if (DirectInput::GetInstance()->TriggerKey(DIK_4)) { gameoverUi.StartGameOverUI(); }
		if (player->GetIsDead() && !gameoverUi.GetIsGameOver()) { gameoverUi.StartGameOverUI(); }

		if (DirectInput::GetInstance()->TriggerKey(DIK_TAB) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_START)) {
			isInputConfigMode = true;
			actionInputConfig->Reset();
		}
	}
	else {
		//���͐ݒ�X�V
		actionInputConfig->Update();

		if (actionInputConfig->GetIsInputConfigEnd()) { isInputConfigMode = false; }
	}
}

void Scene1::Draw(const int _cameraNum)
{
	field->Draw();
	//gobject->ColliderDraw();

	player->Draw();
	boss->Draw();
}

void Scene1::DrawLightView(const int _cameraNum)
{
	player->DrawLightView();
}

void Scene1::NonPostEffectDraw(const int _cameraNum)
{
	//�X�v���C�g
	if (_cameraNum == 0) {
		DebugText::GetInstance()->DrawAll();
		//sprite->Draw();
	}

	gameoverUi.Draw();

	//���͐ݒ�`��
	if (isInputConfigMode) {
		actionInputConfig->Draw();
	}
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

	player->ImguiDraw();

	ImGui::SliderFloat("blend rate", &rate, 0.0f, 1.0f);
	ImGui::Checkbox("stop", &stop);

	ImGui::End();
}

void Scene1::FrameReset()
{
	boss->FrameReset();
}

void Scene1::CollisionCheck()
{
#pragma region �v���C���[�ƓG�̍U���̏Փ˔���
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

#pragma region �v���C���[�̍U���ƓG�̏Փ˔���
	{
		//�v���C���[�̍U��������ꍇ�̂ݔ��� 
		if (player->GetAttackAction()) {
			Sphere enemySphere;
			enemySphere.center = { boss->GetCenter()->GetPosition().x, boss->GetCenter()->GetPosition().y, boss->GetCenter()->GetPosition().z, 1.0f };
			enemySphere.radius = boss->GetCenter()->GetScale().x * 2;

			Sphere attackSphere;
			attackSphere.center = player->GetAttackAction()->GetAttackCollisionData().center;
			attackSphere.radius = player->GetAttackAction()->GetAttackCollisionData().radius * 2;

			//�U���������L���ɂ��Ă����画������
			if (Collision::CheckSphere2Sphere(enemySphere, attackSphere) && player->GetAttackAction()->GetIsCollisionValid()) {
				//�G�Ƀ_���[�W
				boss->Damage(player->GetAttackAction()->GetAttackCollisionData().power);

				//���t���[�����i�q�b�g����̂�h�����߁A���̍U���̏Փ˔����off�ɂ��Ă����B
				player->GetAttackAction()->SetIsCollisionValid(false);
			}
		}
	}
#pragma endregion

#pragma region �J�����̃��b�N�I���^�[�Q�b�g�ݒ�
	{
		//�J���������b�N�I���^�[�Q�b�g�����o���Ă���ꍇ�̂ݔ���
		if (camera->GetisLockonStart()) {
			//�G���W
			XMFLOAT2 pos = boss->GetCenter()->GetScreenPosition();

			//�G�̃X�N���[�����W�����o�Ώ۔͈͓��Ȃ珈��
			const float targetScreenDistance = 100;
			const bool isInsideTargetScreen = (pos.x <= WindowApp::GetWindowWidth() - targetScreenDistance && pos.x >= targetScreenDistance &&
				pos.y <= WindowApp::GetWindowHeight() - targetScreenDistance && pos.y >= targetScreenDistance);
			if (isInsideTargetScreen) {
				//���b�N�I���Ώۂ��m�肳����
				camera->Lockon(boss->GetCenter());
			}
		}
	}
#pragma endregion
}
