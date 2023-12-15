#pragma once
#include "Particle/Emitter.h"
#include "Object/3d/Object3d.h"
#include "Math/Vector3.h"

/// <summary>
/// �v���C���[�a�����o
/// </summary>
class PlayerSlashEffect
{
public: //�����o�֐�
	PlayerSlashEffect(const std::string& texName);
	~PlayerSlashEffect(){};

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="swordHandlePos">���̎�������W</param>
	/// <param name="swordTipPos">������W</param>
	void Update(const Vector3& swordHandlePos, const Vector3& swordTipPos);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private: //�����o�֐�
	/// <summary>
	/// �������W�X�V
	/// </summary>
	/// <param name="swordHandlePos">���̎�������W</param>
	/// <param name="swordTipPos">������W</param>
	void UpdateEmitPos(const Vector3& swordHandlePos, const Vector3& swordTipPos);

	/// <summary>
	/// �p�[�e�B�N������
	/// </summary>
	void AddParticle();

private: //�ÓI�����o�ϐ� 
	//�����甭��������p�[�e�B�N����
	static const int EmitNum = 10;

private: //�����o�ϐ�
	//�p�[�e�B�N�������G�~�b�^�[
	std::unique_ptr<Emitter> particle;
	//�������W�ۑ��plist
	std::array<std::list<Vector3>, EmitNum> emitPosLists;
};

