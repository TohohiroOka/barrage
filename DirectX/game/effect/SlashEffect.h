#pragma once
#include "Particle/Emitter.h"
#include "Object/3d/Object3d.h"
#include "Math/Vector3.h"

/// <summary>
/// �a�����o
/// </summary>
class SlashEffect
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="texName">�p�[�e�B�N���e�N�X�`����</param>
	/// <param name="emitNum">�����\������p�[�e�B�N���G�~�b�^�[��</param>
	/// <param name="frameParticleNum">1�t���[���O�̔������W�Ƃ̊Ԃɔ���������p�[�e�B�N����</param>
	/// <param name="maxFrame">�p�[�e�B�N����������</param>
	/// <param name="startScale">�������̑傫��</param>
	/// <param name="endScale">���Ŏ��̑傫��</param>
	/// <param name="startColor">�������̐F</param>
	/// <param name="endColor">���Ŏ��̐F</param>
	/// <param name="isParticleEmit">�p�[�e�B�N���𔭐������邩</param>
	SlashEffect(const std::string& texName, int emitNum, int frameParticleNum, float maxFrame, float startScale, float endScale,
		DirectX::XMFLOAT4 startColor, DirectX::XMFLOAT4 endColor, bool isParticleEmit = true);
	~SlashEffect() {};

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="point1">���̒[1</param>
	/// <param name="point2">���̒[2</param>
	void Update(const Vector3& point1, const Vector3& point2);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();


	//setter
	void SetEmitNum(int emitNum) { emitPosLists.resize(emitNum); }
	void SetFrameParticleNum(int frameParticleNum) { this->frameParticleNum = frameParticleNum; }
	void SetIsParticleEmit(bool isParticleEmit) { this->isParticleEmit = isParticleEmit; }
	void SetMaxFrame(float maxFrame) { this->maxFrame = maxFrame; }
	void SetStartScale(float startScale) { this->startScale = startScale; }
	void SetEndScale(float endScale) { this->endScale = endScale; }
	void SetStartColor(const DirectX::XMFLOAT4& startColor) { this->startColor = startColor; }
	void SetEndColor(const DirectX::XMFLOAT4& endColor) { this->endColor; }

private: //�����o�֐�
	/// <summary>
	/// �������W�X�V
	/// </summary>
	/// <param name="point1">���̒[1</param>
	/// <param name="point2">���̒[2</param>
	void UpdateEmitPos(const Vector3& point1, const Vector3& point2);

	/// <summary>
	/// �p�[�e�B�N������
	/// </summary>
	void AddParticle();

private: //�����o�ϐ�
	//�p�[�e�B�N�������G�~�b�^�[
	std::array<std::unique_ptr<Emitter>, 3> particle;
	//�������W�ۑ��plist
	std::vector<std::list<Vector3>> emitPosLists;
	//1�t���[���O�̔������W�Ƃ̊Ԃɔ���������p�[�e�B�N����
	int frameParticleNum;
	//�p�[�e�B�N���𔭐������邩
	bool isParticleEmit;

	//�p�[�e�B�N����������
	float maxFrame;
	//�������̑傫��
	float startScale;
	//���Ŏ��̑傫��
	float endScale;
	//�������̐F
	DirectX::XMFLOAT4 startColor;
	//���Ŏ��̐F
	DirectX::XMFLOAT4 endColor;
};

