#pragma once

#include "camera/CutSceneCamera.h"

#include "Particle/Emitter.h"
#include "Object/2d/Sprite.h"

struct ParticleGrainState
{
	float aliveFrame = 0;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 vel;
	DirectX::XMFLOAT3 acc;
	float s_scale;
	float e_scale;
	DirectX::XMFLOAT4 s_color;
	DirectX::XMFLOAT4 e_color;
};

class BaseDefeatDirection
{
protected:


public:
	BaseDefeatDirection() = default;
	virtual ~BaseDefeatDirection() = default;

	virtual void Init();

	virtual void Update();

	virtual void Draw();

	virtual void Draw2D();

	void Reset();

	void StartDefeatDirection(const DirectX::XMFLOAT3& bossPosition);

	bool GetDirectionEnd() { return isEnded && isCommonEnded; }

protected:
	//�p����ŋ��ʂŎg�p����

	//�p�[�e�B�N���֐��ȗ���
	void InEmitter(Emitter* ptr, ParticleGrainState& pgState);

	void StartCommonDirection();

	//����
	int frame = 0;
	//���o�t���O
	bool isDirecting = false;
	//�I���t���O
	bool isEnded = false;
	//�{�X�̍��W��ێ�
	DirectX::XMFLOAT3 bossPosition;

private:
	//�S�V�[���Ŏg�p���鉉�o
	int commonDirectFrame = 0;
	//�t�F�[�h�C������
	const int TEXT_FADEIN_MAXFRAME = 30;
	bool isFadeInEnd = false;
	//��������
	const int TEXT_DISPLAY_MAXFRAME = 120;
	bool isDisplayEnd = false;
	//�t�F�[�h�A�E�g����
	const int TEXT_FADEOUT_MAXFRAME = 30;
	bool isFadeOutEnd = false;
	//���o�̍��v����
	const int COMMON_DIRECTION_MAXFRAME = TEXT_FADEIN_MAXFRAME + TEXT_FADEOUT_MAXFRAME + TEXT_DISPLAY_MAXFRAME;
	//�S�V�[�����ʉ��o��
	bool isCommonDirecting = false;
	//�S�V�[�����ʉ��o�I��
	bool isCommonEnded = false;
	//�e�L�X�g
	std::unique_ptr<Sprite> victoryTextUpper;

};