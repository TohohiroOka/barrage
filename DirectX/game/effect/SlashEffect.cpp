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

	//要素数を確保
	emitPosLists.resize(emitNum);

	for (auto& i : particle) {
		i = Emitter::Create(texName);
		i->SetBloom();
	}
}

void SlashEffect::Update(const Vector3& point1, const Vector3& point2)
{
	//発生座標の更新
	UpdateEmitPos(point1, point2);
	//パーティクル発生
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
	Vector3 swordTopButtonVec = point2 - point1; //剣先から持つ部分までのベクトル

	for (int i = 0; i < emitPosLists.size(); i++) {
		//発生座標を記録しておく
		Vector3 objectPos = point1 + (swordTopButtonVec * ((float)i / (emitPosLists.size() - 1)));
		emitPosLists[i].push_back(objectPos);
		//直近5フレーム分の要素で計算をするので、それより過去の要素を削除
		const int oldFrameMaxSize = 5;
		if (emitPosLists[i].size() > oldFrameMaxSize) {
			emitPosLists[i].erase(emitPosLists[i].begin());
		}
	}
}

void SlashEffect::AddParticle()
{
	for (int i = 0; i < emitPosLists.size(); i++) {
		//スプライン補間用にvectorに要素を入れておく
		std::vector<Vector3> points = {};
		std::list<Vector3>::iterator itr = emitPosLists[i].begin();
		const int listSize = (int)emitPosLists[i].size();
		for (int j = 0; j < listSize; j++) {
			points.push_back(*itr);
			itr++;
		}

		//スプライン補間での直近の2点を繋いだ線を細かく刻んで飛行機雲を生成
		for (int j = 0; j < frameParticleNum; j++) {
			float t = (float)j / frameParticleNum; //0～1の数字を算出
			//生成座標を取得
			Vector3 emitpos = SplinePosition(points, listSize - 3, t);

			//剣先の色を濃く、根本の色を薄くする
			const float colorPower = 1.0f - ((float)i / (emitPosLists.size() - 1));
			const DirectX::XMFLOAT4 sColor = { Easing::InQuint(startColor.x, 0.05f ,colorPower), Easing::InQuint(startColor.y, 0.05f ,colorPower),  Easing::InQuint(startColor.z, 0.05f ,colorPower), 1.0f };
			const DirectX::XMFLOAT4 eColor = { Easing::InQuint(endColor.x, 0.001f ,colorPower), Easing::InQuint(endColor.y, 0.001f ,colorPower),  Easing::InQuint(endColor.z, 0.001f ,colorPower), 1.0f };

			//パーティクル生成
			for (auto& i : particle) {
				if (i->GetCreateNum() >= 1024) { continue; }
				i->InEmitter(maxFrame, emitpos, {}, {}, startScale, endScale, sColor, eColor);
			}
		}
	}
}
