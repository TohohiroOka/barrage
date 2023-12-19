#include "SlashEffect.h"
#include "GameHelper.h"
#include "Math/Easing/Easing.h"

SlashEffect::SlashEffect(const std::string& texName, int emitNum, int frameParticleNum, float maxFrame, float startScale, float endScale, 
	DirectX::XMFLOAT4 startColor, DirectX::XMFLOAT4 endColor, bool isParticleEmit)
{
	this->frameParticleNum = frameParticleNum;
	this->maxFrame = maxFrame;
	this->startScale = startScale;
	this->endScale = endScale;
	this->startColor = startColor;
	this->endColor = endColor;
	this->isParticleEmit = isParticleEmit;

	//�v�f�����m��
	emitPosLists.resize(emitNum);

	for (auto& i : particle) {
		i = Emitter::Create(texName);
		i->SetBloom();
	}
}

void SlashEffect::Update(const Vector3& point1, const Vector3& point2)
{
	//�������W�̍X�V
	UpdateEmitPos(point1, point2);
	//�p�[�e�B�N������
	if (isParticleEmit) {
		AddParticle();
	}

	for (auto& i : particle) {
		if (i->GetCreateNum() == 0) { continue; }
		i->Update();
	}
}

void SlashEffect::Draw()
{
	for (auto& i : particle) {
		if (i->GetCreateNum() == 0) { continue; }
		i->Draw();
	}
}

void SlashEffect::UpdateEmitPos(const Vector3& point1, const Vector3& point2)
{
	Vector3 swordTopButtonVec = point2 - point1; //���悩�玝�����܂ł̃x�N�g��

	for (int i = 0; i < emitPosLists.size(); i++) {
		//�������W���L�^���Ă���
		Vector3 objectPos = point1 + (swordTopButtonVec * ((float)i / (emitPosLists.size() - 1)));
		emitPosLists[i].push_back(objectPos);
		//����5�t���[�����̗v�f�Ōv�Z������̂ŁA������ߋ��̗v�f���폜
		const int oldFrameMaxSize = 5;
		if (emitPosLists[i].size() > oldFrameMaxSize) {
			emitPosLists[i].erase(emitPosLists[i].begin());
		}
	}
}

void SlashEffect::AddParticle()
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
		for (int j = 0; j < frameParticleNum; j++) {
			float t = (float)j / frameParticleNum; //0�`1�̐������Z�o
			//�������W���擾
			Vector3 emitpos = SplinePosition(points, listSize - 3, t);

			//����̐F��Z���A���{�̐F�𔖂�����
			const float colorPower = 1.0f - ((float)i / (emitPosLists.size() - 1));
			const DirectX::XMFLOAT4 sColor = { Easing::InQuint(startColor.x, 0.05f ,colorPower), Easing::InQuint(startColor.y, 0.05f ,colorPower),  Easing::InQuint(startColor.z, 0.05f ,colorPower), 1.0f };
			const DirectX::XMFLOAT4 eColor = { Easing::InQuint(endColor.x, 0.001f ,colorPower), Easing::InQuint(endColor.y, 0.001f ,colorPower),  Easing::InQuint(endColor.z, 0.001f ,colorPower), 1.0f };

			//�p�[�e�B�N������
			for (auto& i : particle) {
				if (i->GetCreateNum() >= 1024) { continue; }
				i->InEmitter(maxFrame, emitpos, {}, {}, startScale, endScale, sColor, eColor);
			}
		}
	}
}
