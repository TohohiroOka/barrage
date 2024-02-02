#include "Gauge.h"

Gauge::Gauge(const DirectX::XMFLOAT2& position, const std::string& frameTexName, const DirectX::XMFLOAT2& barShiftNum, int maxGaugeNum, int gaugeNum, float decreaseGaugeLengthChangeSpeed, const DirectX::XMFLOAT4& color)
{
	this->maxGaugeNum = maxGaugeNum;
	this->gaugeNum = gaugeNum;
	this->decreaseLengthChangeSpeed = decreaseGaugeLengthChangeSpeed;

	//枠生成
	TextureManager::LoadTexture(frameTexName, "Resources/SpriteTexture/" + frameTexName + ".png");
	gaugeFrame = Sprite::Create(frameTexName, position, { 0, 0.5f });

	//バー生成
	//バーの長さと太さをセット(バーの長さ = フレームの長さ - 枠の分ずらす量 * 2(左右or上下))
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
	//ゲージ量を更新してバーの長さを変更
	const float newGaugeLength = (float)newGaugeNum / maxGaugeNum * length;
	gaugeBar->SetSize({ newGaugeLength, thickness });

	//減少量バーの長さを変更
	if (this->isDecreaseDiffMode || isDecreaseDiffMode) {
		//減少量の差分を計算
		const int decreaseNum = gaugeNum - newGaugeNum;
		//変更前のゲージの長さを計算
		const float beforeGaugeLength = (float)gaugeNum / maxGaugeNum * length;
		//減少中のゲージが余っている可能性があるので計算
		if (!this->isDecreaseDiffMode) { gaugeDecreaseDiff->SetPosition(gaugeBar->GetPosition()); }
		float beforeDecreaseLengthSurplus = (float)(gaugeDecreaseDiff->GetPosition().x + gaugeDecreaseDiff->GetSize().x) - (gaugeBar->GetPosition().x + beforeGaugeLength);
		beforeDecreaseLengthSurplus = max(beforeDecreaseLengthSurplus, 0);

		//減少中のバーの長さをセット
		const float decreaseLengthNum = (float)decreaseNum / maxGaugeNum * length + beforeDecreaseLengthSurplus;
		gaugeDecreaseDiff->SetPosition({ gaugeBar->GetPosition().x + gaugeBar->GetSize().x, gaugeBar->GetPosition().y });
		gaugeDecreaseDiff->SetSize({ decreaseLengthNum, thickness });
	}

	//変更前のゲージ量を更新
	gaugeNum = newGaugeNum;

	//ゲージ減少量を表示状態にする
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

	//ゲージ減少
	DirectX::XMFLOAT2 gaugeDecreaseLength = gaugeDecreaseDiff->GetSize();
	gaugeDecreaseLength.x -= decreaseLengthChangeSpeed;
	gaugeDecreaseLength.x = max(gaugeDecreaseLength.x, 0);
	gaugeDecreaseDiff->SetSize(gaugeDecreaseLength);

	//ゲージが回復などして、バーが減少中のバーを上回ったら、減少終了
	const float barRightPos = gaugeBar->GetPosition().x + gaugeBar->GetSize().x;
	const float decreaseDiffRightPos = gaugeDecreaseDiff->GetPosition().x + gaugeDecreaseDiff->GetSize().x;
	if (barRightPos >= decreaseDiffRightPos) {
		isDecreaseDiffMode = false;

		gaugeDecreaseDiff->SetSize({ 0, 0 });
	}

	//ゲージがなくなったら減少終了
	if (gaugeDecreaseLength.x <= 0) {
		isDecreaseDiffMode = false;
	}
}
