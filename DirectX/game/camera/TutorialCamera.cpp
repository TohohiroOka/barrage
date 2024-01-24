#include "TutorialCamera.h"
#include "Math/Easing/Easing.h"

using namespace DirectX;

TutorialCamera::TutorialCamera()
	: GameCamera()
{
	//�e��Ԃ̓��e���Z�b�g
	func.emplace_back([this] { return NormalAction(); });
	func.emplace_back([this] { return ZoomEnemyBornAction(); });
	func.emplace_back([this] { return ZoomEndReturnAction(); });

	//�^�C�}�[�N���X
	easeTimer = std::make_unique<Engine::Timer>();
}

TutorialCamera::~TutorialCamera()
{
}

void TutorialCamera::Update()
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

void TutorialCamera::ChangePhase(TutorialCameraPhase phase)
{
	//�t�F�[�Y���X�V
	cameraPhase = phase;
	//�C�[�W���O�O���W���Z�b�g
	easeBeforePos = position;
	//��]�C�[�W���O�����Z�b�g
	easeBeforeRota = rotation;
	//�^�C�}�[���Z�b�g
	easeTimer->Reset();

	//�t�F�[�Y���ƂɕK�v�ȏ����Z�b�g
	if (phase == TutorialCameraPhase::ZOOM_ENEMY_BORN) {
		Vector3 distance = { 0, 10, -50 };
		easeAfterPos = zoomTargetPos + distance;
		easeAfterRota = VelocityRotate(zoomTargetPos - easeAfterPos);

		//�Y�[���O�̏����Z�b�g
		zoomBeforePos = position;
		zoomBeforeRota = rotation;
	}
	else if (phase == TutorialCameraPhase::ZOOM_END_RETURN) {
		easeAfterPos = zoomBeforePos;
		easeAfterRota = zoomBeforeRota;
	}

	Rotate360(easeAfterRota.x);
	Rotate360(easeAfterRota.y);
	Rotate360(easeAfterRota.z);
	//y����]�p��180�ȏ�̏ꍇ�͋t��]�ɂȂ�悤�ɒ���
	if (fabsf(easeAfterRota.y - easeBeforeRota.y) > 180) {
		if (easeAfterRota.y > easeBeforeRota.y) { easeAfterRota.y -= 360; }
		else { easeAfterRota.y += 360; }
	}
	if (fabsf(easeAfterRota.x - easeBeforeRota.x) > 180) {
		if (easeAfterRota.x > easeBeforeRota.x) { easeAfterRota.x -= 360; }
		else { easeAfterRota.x += 360; }
	}

	isPhaseActionEnd = false;
}

void TutorialCamera::NormalAction()
{
	//���b�N�I�����͂��󂯕t����
	LockonInput();

	//�g�����X�t�H�[���X�V
	UpdateTransform();
}

void TutorialCamera::ZoomEnemyBornAction()
{
	//�s�����I�����Ă����甲����
	if (isPhaseActionEnd) { return; }

	const float zoomTime = 90;
	easeTimer->Update();
	const float time = *easeTimer.get() / zoomTime;
	position.x = Easing::InOutQuart(easeBeforePos.x, easeAfterPos.x, time);
	position.y = Easing::InOutQuart(easeBeforePos.y, easeAfterPos.y, time);
	position.z = Easing::InOutQuart(easeBeforePos.z, easeAfterPos.z, time);

	rotation.x = Easing::InOutQuart(easeBeforeRota.x, easeAfterRota.x, time);
	rotation.y = Easing::InOutQuart(easeBeforeRota.y, easeAfterRota.y, time);
	rotation.z = Easing::InOutQuart(easeBeforeRota.z, easeAfterRota.z, time);

	if (*easeTimer.get() >= zoomTime) {
		isPhaseActionEnd = true;
	}
}

void TutorialCamera::ZoomEndReturnAction()
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
