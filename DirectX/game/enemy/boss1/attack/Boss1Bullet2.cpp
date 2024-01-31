#include "Boss1Bullet2.h"
#include "GameHelper.h"
#include "../Boss1Model.h"
#include "../game/enemy/BaseBoss.h"
#include "Math/Easing/Easing.h"
#include <iterator>

//xz軸、y軸の距離
const std::array<Vector3, Boss1Bullet2::outPosNum> Boss1Bullet2::outPos = {
Vector3{5.0f,10.0f,-10.0f},{10.0f,20.0f,-10.0f},{15.0f,10.0f,-10.0f},{20.0f,20.0f,-10.0f},{ 25.0f,10.0f,-10.0f},
{-5.0f,20.0f,-10.0f},{-10.0f,10.0f,-10.0f},{-15.0f,20.0f,-10.0f},{-20.0f,10.0f,-10.0f},{ -25.0f,20.0f,-10.0f}
};

const float bulletSpeed = 6.0f;

Boss1Bullet2::Boss1Bullet2()
{
	boss->SetPlayerDirection();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack2_end));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack2_start));
	boss->GetBaseModel()->AnimationReset();

	//状態
	state = State::start;

	//衝突判定
	useCollision = UseCollision::capsule;

	//オブジェクト
	model = Model::CreateFromOBJ("boss1/slashing");
	instanceObject = InstanceObject::Create(model.get());
	instanceObject->SetBloom(true);
	instanceObject->SetOutlineColor({ 0.4f,0.2f ,0.5f });

	//タイマー系
	timer = std::make_unique<Engine::Timer>();
	hitTimer = std::make_unique<Engine::Timer>();

	//敵の向き
	boss->SetPlayerDirection();

	//出現回数
	nowNum = 0;

	//狙う位置までの距離
	dist = 1.0f;

	//狙う位置
	inPlayerPos = boss->GetTargetPos();

	isAttackEnd = false;
	isAllBulletAlive = false;

	using namespace DirectX;
	XMFLOAT3 rota = boss->GetBaseModel()->GetObjectInst()->GetRotation();
	// スケール、回転、平行移動行列の計算
	DirectX::XMMATRIX matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rota.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rota.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rota.y));

	Vector3 bossPos = boss->GetBaseModel()->GetPosition();
	for (int i = 0; i < outPosNum; i++) {
		XMFLOAT3 pos = outPos[i];
		XMVECTOR matPos = XMVector3Transform({ pos.x,pos.y,pos.z }, matRot);

		outPosInfo[i].pos = Vector3{ matPos.m128_f32[0], matPos.m128_f32[1], matPos.m128_f32[2] } + bossPos;
		outPosInfo[i].isAlive = false;

		//エフェクト
		outPosInfo[i].effect = std::make_unique<BulletAttack2Effect>(outPosInfo[i].pos, Vector2{ 5.0f,5.0f });
	}

	//移動方向
	XMVECTOR matAllMoveVec = XMVector3Transform({ 0.0f,0.0f,1.0f }, matRot);
	allMoveVec = Vector3{ matAllMoveVec.m128_f32[0], matAllMoveVec.m128_f32[1], matAllMoveVec.m128_f32[2] };

	hitVec = outPosInfo[0].pos - outPosInfo[1].pos;
	hitVec = hitVec.normalize();
	hitVec *= 4.0f;

	//行動関数
	func_.emplace_back([this] {return Start(); });
	func_.emplace_back([this] {return Attack(); });
	func_.emplace_back([this] {return End(); });
}

void Boss1Bullet2::Update()
{
	if (!boss->GetIsWince() && !boss->GetIsBreak()) {
		if (int(state) >= 0 && int(state) < int(State::non)) {
			func_[int(state)]();
		}
	}else {
		for (auto& i : outPosInfo) {
			i.isAlive = false;
			i.effect->Finalize();
		}
		isEnd = true;
		return;
	}

	OutPosEffectAdd();

	for (auto& i : outPosInfo) {
		if (i.effect) {
			i.effect->Update();
		}
	}

	BulletUpdate();

	if (!isCollision) {
		hitTimer->Update();
		//判定を取らない時間以上になったら戻す
		if (*hitTimer.get() > 5) {
			isCollision = true;
		}
	}

	timer->Update();
}

void Boss1Bullet2::Draw()
{
	instanceObject->Draw(ObjectBase::DrawMode::add);
	for (auto& i : outPosInfo) {
		if (!i.isAlive) { continue; }
		i.effect->Draw();
	}
}

void Boss1Bullet2::FrameReset()
{
	instanceObject->FrameReset();
}

void Boss1Bullet2::GetAttackCollisionCapsule(std::vector<Capsule>& _info)
{
	for (auto& i : bullet) {
		Capsule add;
		add.startPosition = i.pos + hitVec;
		add.endPosition = i.pos - hitVec;
		add.radius = 1.0f;
		_info.emplace_back(add);
	}
}

void Boss1Bullet2::DeleteBullet(std::vector<int> _deleteNum)
{
	int vecNum = 0;
	for (int i=0;i<bulletNum;i++) {
		if (bulletNum != _deleteNum[vecNum]) { continue; }
		bullet[i].isAlive = false;
		vecNum++;
		if (_deleteNum.size() == vecNum) { break; }
	}
}

void Boss1Bullet2::Start()
{
	const float maxTime=50.0f;
	const float rate = *timer.get() / maxTime;

	const float oneTime = maxTime / outPosNum;
	for (int i = 0; i < outPosNum; i++) {
		if (*timer.get() < oneTime * i) { break; }
		outPosInfo[i].isAlive = true;
	}

	if (rate < 1.0f) { return; }
	state = State::attack;
	timer->Reset();
	isCollision = true;
}

void Boss1Bullet2::Attack()
{
	const float maxTime = 40.0f;
	const float rate = *timer.get() / maxTime;

	if ((rate * 10.0f) < nowNum) { return; }

	Vector3 pos = boss->GetBaseModel()->GetPosition();
	for (int i = 0; i < outPosNum; i++) {
		const int bulletNowNum = i + nowNum * outPosNum;
		bullet[bulletNowNum].isAlive = true;
		bullet[bulletNowNum].pos = outPosInfo[i].pos;
		bullet[bulletNowNum].moveVec = { allMoveVec.x * dist,-1.0f,allMoveVec.z * dist };
		bullet[bulletNowNum].moveVec = bullet[bulletNowNum].moveVec.normalize() * bulletSpeed;
		bullet[bulletNowNum].rota = VelocityRotate(bullet[bulletNowNum].moveVec);
		bullet[bulletNowNum].rota.x += 90.0f;
		bullet[bulletNowNum].rota.y += 90.0f;
	}
	nowNum++;
	dist += 0.5f;

	if (nowNum < outPosNum) { return; }
	isAttackEnd = true;
	state = State::end;
	timer->Reset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack2_end));
	for (auto& i : outPosInfo) {
		i.isAlive = false;
	}
}

void Boss1Bullet2::End()
{
	if (!boss->GetBaseModel()->GetIsAnimationEnd() && !isAllBulletAlive) { return; }
	
	//エフェクトの残り数チェック
	int num = 0;
	for (auto& i : outPosInfo) {
		if (i.effect->EffectNum() == 0) {
			num++;
		}
	}

	if (num != outPosNum) { return; }
	isEnd = true;
}

void Boss1Bullet2::OutPosEffectAdd()
{
	for (auto& i :outPosInfo) {
		if(!i.isAlive){continue;}
		i.effect->AddParticle();
	}
}

void Boss1Bullet2::BulletUpdate()
{
	int aliveNum = 0;
	for (auto& i : bullet) {
		if(!i.isAlive){continue;}
		i.pos += i.moveVec;

		const float dist = 10.0f;
		if (i.pos.x < -dist || i.pos.x > GameHelper::Instance()->GetStageSize() + dist ||
			i.pos.y < -dist || i.pos.y > GameHelper::Instance()->GetStageSize() + dist ||
			i.pos.z < -dist || i.pos.z > GameHelper::Instance()->GetStageSize() + dist) {
			i.isAlive = false;
			continue;
		}

		instanceObject->DrawInstance(i.pos, { 0.8f,1.0f,0.8f }, { i.rota.x,i.rota.y,i.rota.z }, { 0.4f,0.2f ,0.5f ,1.0f });

		//加算合成する用
		for (int num = 0; num < 3; num++) {
			float colorRate = (float(num + 1) / 9.0f) + 1.0f;
			instanceObject->DrawInstance(i.pos - i.moveVec * ((num * (1.0f / bulletSpeed)) + 0.5f), { 0.8f,1.0f,0.8f }, { i.rota.x, i.rota.y,i.rota.z }, { 0.4f * colorRate,0.2f * colorRate ,0.5f * colorRate ,0.4f / colorRate });
		}

		aliveNum++;
	}

	instanceObject->Update();

	if (aliveNum != 0 || !isAttackEnd) { return; }
	isAllBulletAlive = true;
	isCollision = false;
}
