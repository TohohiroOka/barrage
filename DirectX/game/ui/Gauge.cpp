#include "Gauge.h"

Gauge::Gauge(const DirectX::XMFLOAT2& position, const std::string& frameTexName, const DirectX::XMFLOAT2& barShiftNum, int maxGaugeNum, int gaugeNum, float decreaseGaugeLengthChangeSpeed, const DirectX::XMFLOAT4& color)
{
	this->maxGaugeNum = maxGaugeNum;
	this->gaugeNum = gaugeNum;
	this->decreaseLengthChangeSpeed = decreaseGaugeLengthChangeSpeed;

	//�g����
	TextureManager::LoadTexture(frameTexName, "Resources/SpriteTexture/" + frameTexName + ".png");
	gaugeFrame = Sprite::Create(frameTexName, position, { 0, 0.5f });

	//�o�[����
	//�o�[�̒����Ƒ������Z�b�g(�o�[�̒��� = �t���[���̒��� - �g�̕����炷�� * 2(���Eor�㉺))
	this->length = gaugeFrame->GetSize().x - barShiftNum.x * 2;
	this->thickness = gaugeFrame->GetSize().y - barShiftNum.y * 2;
	TextureManager::LoadTexture("gauge", "Resources/SpriteTexture/gauge.png");
	const DirectX::XMFLOAT2 barPos = { position.x + barShiftNum.x, position.y };
	gaugeBar = Sprite::Create("gauge", barPos, { 0, 0.5f }, color);
	gaugeBar->SetSize({ length, thickness });
	const DirectX::XMFLOAT4 decreaseDiffColor = { 0.8f, 0.8f, 0.2f, 1.0f };
	gaugeDecreaseDiff = Sprite::Create("gauge", barPos, { 0, 0.5f }, decreaseDiffColor);
	gaugeDecreaseDiff->SetSize({ 0, thickness });
}

Gauge::~Gauge()
{
}

void Gauge::Update()
{
	ChangeDecreaseDiffLength();

	gaugeFrame->Update();
	gaugeBar->Update();

	if (!isDecreaseDiffMode) { return; }
	gaugeDecreaseDiff->Update();
}

void Gauge::Draw()
{
	gaugeBar->Draw();

	if (isDecreaseDiffMode) {
		gaugeDecreaseDiff->Draw();
	}

	gaugeFrame->Draw();
}

void Gauge::ChangeLength(int newGaugeNum, bool isDecreaseDiffMode)
{
	//�Q�[�W�ʂ��X�V���ăo�[�̒�����ύX
	const float newGaugeLength = (float)newGaugeNum / maxGaugeNum * length;
	gaugeBar->SetSize({ newGaugeLength, thickness });

	//�����ʃo�[�̒�����ύX
	if (this->isDecreaseDiffMode || isDecreaseDiffMode) {
		//�����ʂ̍������v�Z
		const int decreaseNum = gaugeNum - newGaugeNum;
		//�ύX�O�̃Q�[�W�̒������v�Z
		const float beforeGaugeLength = (float)gaugeNum / maxGaugeNum * length;
		//�������̃Q�[�W���]���Ă���\��������̂Ōv�Z
		if (!this->isDecreaseDiffMode) { gaugeDecreaseDiff->SetPosition(gaugeBar->GetPosition()); }
		float beforeDecreaseLengthSurplus = (float)(gaugeDecreaseDiff->GetPosition().x + gaugeDecreaseDiff->GetSize().x) - (gaugeBar->GetPosition().x + beforeGaugeLength);
		beforeDecreaseLengthSurplus = max(beforeDecreaseLengthSurplus, 0);

		//�������̃o�[�̒������Z�b�g
		const float decreaseLengthNum = (float)decreaseNum / maxGaugeNum * length + beforeDecreaseLengthSurplus;
		gaugeDecreaseDiff->SetPosition({ gaugeBar->GetPosition().x + gaugeBar->GetSize().x, gaugeBar->GetPosition().y });
		gaugeDecreaseDiff->SetSize({ decreaseLengthNum, thickness });
	}

	//�ύX�O�̃Q�[�W�ʂ��X�V
	gaugeNum = newGaugeNum;

	//�Q�[�W�����ʂ�\����Ԃɂ���
	if ((!this->isDecreaseDiffMode) && isDecreaseDiffMode) {
		this->isDecreaseDiffMode = isDecreaseDiffMode;

		const int decreaseStartLagTime = 50;
		decreaseStartLagTimer = decreaseStartLagTime;
	}
}

void Gauge::ChangeDecreaseDiffLength()
{
	if (!isDecreaseDiffMode) { return; }

	decreaseStartLagTimer--;
	if (decreaseStartLagTimer > 0) { return; }

	//�Q�[�W����
	DirectX::XMFLOAT2 gaugeDecreaseLength = gaugeDecreaseDiff->GetSize();
	gaugeDecreaseLength.x -= decreaseLengthChangeSpeed;
	gaugeDecreaseLength.x = max(gaugeDecreaseLength.x, 0);
	gaugeDecreaseDiff->SetSize(gaugeDecreaseLength);

	//�Q�[�W���񕜂Ȃǂ��āA�o�[���������̃o�[����������A�����I��
	const float barRightPos = gaugeBar->GetPosition().x + gaugeBar->GetSize().x;
	const float decreaseDiffRightPos = gaugeDecreaseDiff->GetPosition().x + gaugeDecreaseDiff->GetSize().x;
	if (barRightPos >= decreaseDiffRightPos) {
		isDecreaseDiffMode = false;

		gaugeDecreaseDiff->SetSize({ 0, 0 });
	}

	//�Q�[�W���Ȃ��Ȃ����猸���I��
	if (gaugeDecreaseLength.x <= 0) {
		isDecreaseDiffMode = false;
	}
}
