#include "Portal.h"
#include "Scene/InterfaceScene.h"
#include "Math/Vector2.h"
#include "Object/3d/collider/Collision.h"

Portal::Portal(const Vector3& position, InterfaceScene* changeScene)
{
	//�I�u�W�F�N�g����
	model = Model::CreateFromOBJ("warp");
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

	effect = std::make_unique<PortalEffect>(position,Vector2(12.0f, 21.0f));
}

Portal::~Portal()
{
}

void Portal::Update(const PlayerData& playerData)
{
	//�|�[�^���ɓ��������𖞂����Ă��邩�`�F�b�N
	isIntoPortal = CheckIntoPortalArea(playerData) && CheckLineSightInPortal(playerData) && playerData.onGround && playerData.action == PlayerActionName::MOVE_NORMAL;

	//�I�u�W�F�N�g�X�V
	object->Update();

	effect->AddParticle();
	effect->Update();
}

void Portal::Draw()
{
	//�I�u�W�F�N�g�`��
	object->Draw();

	effect->Draw();
}

void Portal::DrawLightView()
{
	object->DrawLightView();
}

bool Portal::CheckIntoPortalArea(const PlayerData& playerData)
{
	//�|�[�^���ɓ����͈͂ɂ��Ȃ����false
	if (playerData.pos.x < intoPortalRangeMin.x) { return false; }
	if (playerData.pos.z < intoPortalRangeMin.z) { return false; }
	if (playerData.pos.x > intoPortalRangeMax.x) { return false; }
	if (playerData.pos.z > intoPortalRangeMax.z) { return false; }

	//�S�Ă̍��ڂ��N���A�����true
	return true;
}

bool Portal::CheckLineSightInPortal(const PlayerData& playerData)
{
	//������̍��W���L�����Ă���
	const float playerRotaRadian = DirectX::XMConvertToRadians(playerData.rota.y);
	Vector3 lineSightPos = playerData.pos;
	lineSightPos.x += sinf(playerRotaRadian);
	lineSightPos.z += cosf(playerRotaRadian);

	//�����x�N�g���ƃv���C���[�ƃ|�[�^���̍��W�̍��̃x�N�g���̓��ς��v�Z
	Vector3 playerLineSightVec = Vector3{ lineSightPos.x, 0, lineSightPos.z } - Vector3{ playerData.pos.x, 0, playerData.pos.z };
	playerLineSightVec.normalize();

	//���������̃��C�𐶐�
	Segment lineSightRay;
	lineSightRay.start = { playerData.pos.x, playerData.pos.y, playerData.pos.z, 1 };
	lineSightRay.dir = { playerLineSightVec.x, playerLineSightVec.y, playerLineSightVec.z, 1 };
	//�|�[�^���̋��𐶐�
	Sphere portalSphere;
	portalSphere.center = { object->GetPosition().x, object->GetPosition().y, object->GetPosition().z, 1 };
	portalSphere.radius = object->GetScale().x * 1.1f;

	//�������C�ƃ|�[�^�������Փ˂��Ă��Ȃ����false
	if (!Collision::CheckRay2Sphere(lineSightRay, portalSphere)) { return false; }

	return true;
}
