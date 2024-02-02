#pragma once
#include "engine/Object/2d/Sprite.h"

/// <summary>
/// �Q�[�W
/// </summary>
class Gauge
{
public: //�����o�ϐ�
	Gauge(const DirectX::XMFLOAT2& position, const std::string& frameTexName, const DirectX::XMFLOAT2& barShiftNum, int maxGaugeNum, int gaugeNum, float decreaseGaugeLengthChangeSpeed, const DirectX::XMFLOAT4& color);
	~Gauge();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �Q�[�W�̒����ύX
	/// </summary>
	/// <param name="newGaugeNum">�V���ɐݒ肷��Q�[�W��</param>
	/// <param name="isDecreaseDiffMode">�Q�[�W�̌����ʂ�\�����邩</param>
	void ChangeLength(int newGaugeNum, bool isDecreaseDiffMode);

	//getter
	bool GetIsDecreaseDiffMode() { return isDecreaseDiffMode; }

private: //�����o�֐�	
	/// <summary>
	/// �����ʂ�Z�����Ă���
	/// </summary>
	void ChangeDecreaseDiffLength();

private: //�����o�ϐ�
	std::unique_ptr<Sprite> gaugeFrame;
	std::unique_ptr<Sprite> gaugeBar;
	std::unique_ptr<Sprite> gaugeDecreaseDiff;

	float length;
	float thickness;
	int maxGaugeNum;
	int gaugeNum;

	int decreaseStartLagTimer = 0;
	float decreaseLengthChangeSpeed = 0;
	bool isDecreaseDiffMode = false;
};