#include "Portal.h"
#include "Scene/InterfaceScene.h"
#include "Math/Vector2.h"

Portal::Portal(const Vector3& position, InterfaceScene* changeScene)
{
	//�I�u�W�F�N�g����
	model = Model::CreateFromOBJ("portal");
	object = Object3d::Create(model.get());
	object->SetPosition(position);
	const float size = 10.0f;
	object->SetScale({ size, size, size });
	object->SetShadowMap(true);

	//�|�[�^���ɓ����͈͂̍ŏ��l�ƍő�l���Z�b�g
	intoPortalRangeMin.x = position.x - size;
	intoPortalRangeMin.z = position.z - size - 7.5f;
	intoPortalRangeMax.x = position.x + size;
	intoPortalRangeMax.z = position.z - size / 2;

	//�ύX��̃V�[�����Z�b�g
	this->changeScene = changeScene;
}

Portal::~Portal()
{
}

void Portal::Update(const PlayerData& playerData)
{
	//�|�[�^���ɓ��������𖞂����Ă��邩�`�F�b�N
	isIntoPortal = CheckIntoPortal(playerData) && playerData.onGround && playerData.action == PlayerActionName::MOVE_NORMAL;

	//�I�u�W�F�N�g�X�V
	object->Update();
}

void Portal::Draw()
{
	//�I�u�W�F�N�g�`��
	object->Draw();
}

void Portal::DrawLightView()
{
	object->DrawLightView();
}

bool Portal::CheckIntoPortal(const PlayerData& playerData)
{
	//�|�[�^���ɓ����͈͂ɂ��Ȃ����false
	if (playerData.pos.x < intoPortalRangeMin.x) { return false; }
	if (playerData.pos.z < intoPortalRangeMin.z) { return false; }
	if (playerData.pos.x > intoPortalRangeMax.x) { return false; }
	if (playerData.pos.z > intoPortalRangeMax.z) { return false; }

	//�v���C���[���|�[�^���̕����������Ă��Ȃ����false
	//������̍��W���L�����Ă���
	const float playerRotaRadian = DirectX::XMConvertToRadians(playerData.rota.y);
	Vector3 lineSightPos = playerData.pos;
	lineSightPos.x += sinf(playerRotaRadian);
	lineSightPos.z += cosf(playerRotaRadian);

	//�����x�N�g���ƃv���C���[�ƃ|�[�^���̍��W�̍��̃x�N�g���̓��ς��v�Z
	Vector2 playerLineSightVec = Vector2{ playerData.pos.x, playerData.pos.z } - Vector2{ lineSightPos.x, lineSightPos.z };
	Vector2 playerToPortalVec = Vector2{ playerData.pos.x, playerData.pos.z } - Vector2{ object->GetPosition().x, object->GetPosition().z };
	playerLineSightVec.normalize();
	playerToPortalVec.normalize();

	//�v���C���[���|�[�^���̓��������Ă��Ȃ����false
	float dot = playerLineSightVec.dot(playerToPortalVec);
	if (dot < 0.75f) { return false; }

	//�S�Ă̍��ڂ��N���A�����true
	return true;
}
