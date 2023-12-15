#include "PlayerSlashEffect.h"
#include "GameHelper.h"

PlayerSlashEffect::PlayerSlashEffect(const std::string& texName)
{
	particle = Emitter::Create(texName);
	particle->SetBloom();
}

void PlayerSlashEffect::Update(const Vector3& swordHandlePos, const Vector3& swordTipPos)
{
	//�������W�̍X�V
	UpdateEmitPos(swordHandlePos, swordTipPos);
	//�p�[�e�B�N������
	AddParticle();

	particle->Update();
}

void PlayerSlashEffect::Draw()
{
	particle->Draw();
}

void PlayerSlashEffect::UpdateEmitPos(const Vector3& swordHandlePos, const Vector3& swordTipPos)
{
	Vector3 swordTopButtonVec = swordTipPos - swordHandlePos; //���悩�玝�����܂ł̃x�N�g��

	for (int i = 0; i < emitPosLists.size(); i++) {
		//�������W���L�^���Ă���
		Vector3 objectPos = swordHandlePos + (swordTopButtonVec * ((float)i / (emitPosLists.size() - 1)));
		emitPosLists[i].push_back(objectPos);
		//����5�t���[�����̗v�f�Ōv�Z������̂ŁA������ߋ��̗v�f���폜
		const int oldFrameMaxSize = 5;
		if (emitPosLists[i].size() > oldFrameMaxSize) {
			emitPosLists[i].erase(emitPosLists[i].begin());
		}
	}
}

void PlayerSlashEffect::AddParticle()
{
	for (int i = 0; i < emitPosLists.size(); i++) {
		//�X�v���C����ԗp��vector�ɗv�f�����Ă���
		std::vector<Vector3> points = {};
		std::list<Vector3>::iterator itr = emitPosLists[i].begin();
		const int listSize = (int)emitPosLists[i].size();
		for (int j = 0; j < listSize; j++) {
			points.push_back(*itr);
			itr++;
		}

		//�X�v���C����Ԃł̒��߂�2�_���q���������ׂ�������Ŕ�s�@�_�𐶐�
		for (int j = 0; j < 10; j++) {
			float t = (float)j / 10; //0�`1�̐������Z�o
			//�������W���擾
			Vector3 emitpos = SplinePosition(points, listSize - 3, t);

			//�p�[�e�B�N������
			const float colorPower = (float)i / (emitPosLists.size() - 1);
			const DirectX::XMFLOAT4 startColor = { 0.02f * colorPower, 0.02f * colorPower, 0.2f * colorPower, 1.0f };
			const DirectX::XMFLOAT4 endColor = { 0.01f * colorPower, 0.01f * colorPower, 0.1f * colorPower, 1.0f };

			particle->InEmitter(30, emitpos, {}, {}, 1.0f, 0.5f, startColor, endColor);
		}
	}
}
