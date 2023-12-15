#include "PlayerSlashEffect.h"
#include "GameHelper.h"

PlayerSlashEffect::PlayerSlashEffect(const std::string& texName)
{
	particle = Emitter::Create(texName);
	particle->SetBloom();
}

void PlayerSlashEffect::Update(const Vector3& swordHandlePos, const Vector3& swordTipPos)
{
	//発生座標の更新
	UpdateEmitPos(swordHandlePos, swordTipPos);
	//パーティクル発生
	AddParticle();

	particle->Update();
}

void PlayerSlashEffect::Draw()
{
	particle->Draw();
}

void PlayerSlashEffect::UpdateEmitPos(const Vector3& swordHandlePos, const Vector3& swordTipPos)
{
	Vector3 swordTopButtonVec = swordTipPos - swordHandlePos; //剣先から持つ部分までのベクトル

	for (int i = 0; i < emitPosLists.size(); i++) {
		//発生座標を記録しておく
		Vector3 objectPos = swordHandlePos + (swordTopButtonVec * ((float)i / (emitPosLists.size() - 1)));
		emitPosLists[i].push_back(objectPos);
		//直近5フレーム分の要素で計算をするので、それより過去の要素を削除
		const int oldFrameMaxSize = 5;
		if (emitPosLists[i].size() > oldFrameMaxSize) {
			emitPosLists[i].erase(emitPosLists[i].begin());
		}
	}
}

void PlayerSlashEffect::AddParticle()
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
		for (int j = 0; j < 10; j++) {
			float t = (float)j / 10; //0～1の数字を算出
			//生成座標を取得
			Vector3 emitpos = SplinePosition(points, listSize - 3, t);

			//パーティクル生成
			const float colorPower = (float)i / (emitPosLists.size() - 1);
			const DirectX::XMFLOAT4 startColor = { 0.02f * colorPower, 0.02f * colorPower, 0.2f * colorPower, 1.0f };
			const DirectX::XMFLOAT4 endColor = { 0.01f * colorPower, 0.01f * colorPower, 0.1f * colorPower, 1.0f };

			particle->InEmitter(30, emitpos, {}, {}, 1.0f, 0.5f, startColor, endColor);
		}
	}
}
