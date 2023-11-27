#include "Gauge.h"

const float Gauge::thickness = 16.0f;

Gauge::Gauge(const DirectX::XMFLOAT2& position, float length, int maxGaugeNum, int gaugeNum, float decreaseGaugeLengthChangeSpeed, const DirectX::XMFLOAT4& color)
{
	this->length = length;
	this->maxGaugeNum = maxGaugeNum;
	this->gaugeNum = gaugeNum;
	this->decreaseLengthChangeSpeed = decreaseGaugeLengthChangeSpeed;

	const std::string textureName = "gauge";
	
	const DirectX::XMFLOAT4 frameColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	gaugeFrame = Sprite::Create(textureName, position, { 0, 0.5f }, frameColor);
	gaugeFrame->SetSize({ length, thickness });
	gaugeBar = Sprite::Create(textureName, position, { 0, 0.5f }, color);
	gaugeBar->SetSize({ length, thickness });
	const DirectX::XMFLOAT4 decreaseDiffColor = { 0.6f, 0.6f, 0.1f, 1.0f };
	gaugeDecreaseDiff = Sprite::Create(textureName, position, { 0, 0.5f }, decreaseDiffColor);
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
	gaugeFrame->Draw();
	gaugeBar->Draw();

	if (!isDecreaseDiffMode) { return; }
	gaugeDecreaseDiff->Draw();
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
