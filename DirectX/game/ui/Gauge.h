#pragma once
#include "engine/Object/2d/Sprite.h"

/// <summary>
/// �Q�[�W
/// </summary>
class Gauge
{
public: //�����o�ϐ�
	Gauge(const DirectX::XMFLOAT2& position, float length, int maxGaugeNum, int gaugeNum, const DirectX::XMFLOAT4& color);
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
	/// <param name="decreaseNum">������</param>
	/// <param name="isDecreaseDiffMode">�Q�[�W�̌����ʂ�\�����邩</param>
	void ChangeLength(int newGaugeNum, bool isDecreaseDiffMode);

	bool GetIsDecreaseDiffMode() { return isDecreaseDiffMode; }

private: //�����o�֐�	
	/// <summary>
	/// �����ʂ�Z�����Ă���
	/// </summary>
	void ChangeDecreaseDiffLength();

private: //�ÓI�����o�ϐ�
	//����
	static const float thickness;

private: //�����o�ϐ�
	std::unique_ptr<Sprite> gaugeFrame;
	std::unique_ptr<Sprite> gaugeBar;
	std::unique_ptr<Sprite> gaugeDecreaseDiff;

	float maxLength;
	float length;
	int maxGaugeNum;
	int gaugeNum;

	int decreaseStartLagTimer = 0;
	bool isDecreaseDiffMode = false;
};