#include "TitleCamera.h"
#include "Math/Easing/Easing.h"

using namespace DirectX;

TitleCamera::TitleCamera()
	: GameCamera()
{
	//�e��Ԃ̓��e���Z�b�g
	func.emplace_back([this] { return NormalAction(); });
	func.emplace_back([this] { return MovePortalFrontAction(); });
	func.emplace_back([this] { return ZoomPortalAction(); });
	func.emplace_back([this] { return StayAction(); });

	//�^�C�}�[�N���X
	easeTimer = std::make_unique<Engine::Timer>();
}

TitleCamera::~TitleCamera()
{
}

void TitleCamera::Update()
{
	//�e��Ԃ̓��e���X�V
	func[int(cameraPhase)]();

	//�J�������W��]�p�𔽉f���čX�V
	//���s�ړ��s��̌v�Z
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//���[���h�s����X�V
	UpdateMatWorld(matTrans);
	//���_�A�����_���X�V
	UpdateEyeTarget();
	Camera::Update();
}

void TitleCamera::ChangePhase(TitleCameraPhase phase)
{
	//�t�F�[�Y���X�V
	cameraPhase = phase;
	//�C�[�W���O�O���W���Z�b�g
	easeBeforePos = position;
	//�^�C�}�[���Z�b�g
	easeTimer->Reset();

	//�t�F�[�Y���ƂɕK�v�ȏ����Z�b�g
	if (phase == TitleCameraPhase::MOVE_PORTAL_FRONT) {
		Vector3 portalDistance = { 0, 10, -50 };
		easeAfterPos = portalPos + portalDistance;
	}
	else if (phase == TitleCameraPhase::ZOOM_PORTAL) {
		Vector3 portalDistance = { 0, 0, -15 };
		easeAfterPos = portalPos + portalDistance;
	}

	//��]�C�[�W���O�����Z�b�g
	easeBeforeRota = rotation;
	easeAfterRota = VelocityRotate(portalPos - easeAfterPos);
	Rotate360(easeAfterRota.x);
	Rotate360(easeAfterRota.y);
	Rotate360(easeAfterRota.z);
	//y����]�p��180�ȏ�̏ꍇ�͋t��]�ɂȂ�悤�ɒ���
	if (easeBeforeRota.y >= 180) {
		easeBeforeRota.y -= 360;
	}

	isPhaseActionEnd = false;
}

void TitleCamera::NormalAction()
{
	//���b�N�I�����͂��󂯕t����
	LockonInput();

	//�g�����X�t�H�[���X�V
	UpdateTransform();
}

void TitleCamera::MovePortalFrontAction()
{
	//�s�����I�����Ă����甲����
	if (isPhaseActionEnd) { return; }

	const float moveTime = 90;
	easeTimer->Update();
	const float time = *easeTimer.get() / moveTime;
	position.x = Easing::InOutQuart(easeBeforePos.x, easeAfterPos.x, time);
	position.y = Easing::InOutQuart(easeBeforePos.y, easeAfterPos.y, time);
	position.z = Easing::InOutQuart(easeBeforePos.z, easeAfterPos.z, time);

	rotation.x = Easing::InOutQuart(easeBeforeRota.x, easeAfterRota.x, time);
	rotation.y = Easing::InOutQuart(easeBeforeRota.y, easeAfterRota.y, time);
	rotation.z = Easing::InOutQuart(easeBeforeRota.z, easeAfterRota.z, time);

	if (*easeTimer.get() >= moveTime) {
		isPhaseActionEnd = true;
	}
}

void TitleCamera::ZoomPortalAction()
{
	//�s�����I�����Ă����甲����
	if (isPhaseActionEnd) { return; }

	const float zoomTime = 100;
	easeTimer->Update();
	const float time = *easeTimer.get() / zoomTime;
	position.x = Easing::InOutQuad(easeBeforePos.x, easeAfterPos.x, time);
	position.y = Easing::InOutQuad(easeBeforePos.y, easeAfterPos.y, time);
	position.z = Easing::InOutQuad(easeBeforePos.z, easeAfterPos.z, time);

	rotation.x = Easing::InOutQuad(easeBeforeRota.x, easeAfterRota.x, time);
	rotation.y = Easing::InOutQuad(easeBeforeRota.y, easeAfterRota.y, time);
	rotation.z = Easing::InOutQuad(easeBeforeRota.z, easeAfterRota.z, time);

	if (*easeTimer.get() >= zoomTime) {
		isPhaseActionEnd = true;
	}
}

void TitleCamera::StayAction()
{
	//�s�����I�����Ă����甲����
	if (isPhaseActionEnd) { return; }

	const float stayTime = 45;
	easeTimer->Update();

	if (*easeTimer.get() >= stayTime) {
		isPhaseActionEnd = true;
	}
}
