#include "ScreenCut.h"
#include "WindowApp.h"
#include "Math/Easing/Easing.h"
#include <imgui.h>
#include "GameHelper.h"
#include "Object/3d/collider/Collision.h"

const std::array<DirectX::XMFLOAT2, ScreenCut::otherNum> ScreenCut::otherMin = {
DirectX::XMFLOAT2{0.832f,0.5f},{0.698f,0.5f},{0.514f,0.5f},{0.497f,0.5f},
{0.448f,0.5f},{0.252f,0.5f},{0.162f,0.5f},
{0.927f,0.5f},{0.744f,0.5f}, {0.658f,0.5f}, {0.515f,0.5f}, {0.515f,0.5f},
{0.472f,0.5f}, {0.285f,0.5f}, {0.218f,0.5f} };
const std::array<DirectX::XMFLOAT2, ScreenCut::otherNum> ScreenCut::otherMax = {
DirectX::XMFLOAT2{1.0f,1.0f},{0.723f,1.0f},{0.653f,1.0f},{0.463f,1.0f},
{0.274f,1.0f},{0.252f,1.0f},{0.0f,1.0f},
{1.0f,0.0f},{0.938f,0.0f},{0.687f,0.0f},{0.63f,0.0f},{0.448f,0.0f},
{0.289f,0.0f},{0.238f,0.0f},{0.0f,0.0f}};
const std::array<float, ScreenCut::panelNum> ScreenCut::rota = {
	100.0f,50.0f,300.0f,200.0f,100.0f,360.0f,280.0f,200.0f,150.0f,90.0f,100.0f,310.0f,270.0f,220.0f
};

ScreenCut::ScreenCut()
{
	for (int i = 1; i < 4; i++) {
		TextureManager::LoadTexture("sprite_triangle" + std::to_string(i), "Resources/SpriteTexture/panelBreak/triangle" + std::to_string(i) + ".png");
	}
	//ì«Ç›çûÇ›
	for (int i = 0; i < panelNum; i++) {
		model[i] = Model::CreateFromOBJ("planeBreak/planeBreak_" + std::to_string(i + 1));
	}

	for (int i = 0; i < panelNum; i++) {
		panel[i] = ShaoeSprite::Create("main_after", {});
		panel[i]->SetModel(model[i].get());
	}

	for (int i = 0; i < 3; i++) {
		effect[i] = ParticleManager::Create("sprite_triangle" + std::to_string(i + 1));
	}

	Initialize();

	cameraDist = 0.992f;

	func_.emplace_back([this] {return LightHalf(); });
	func_.emplace_back([this] {return LightOther(); });
	func_.emplace_back([this] {return PanelBreak(); });
}

void ScreenCut::Initialize()
{
	DirectX::XMFLOAT2 motoScale = { 192.0f / 2.0f,108.0f / 2.0f };
	DirectX::XMFLOAT2 center = { float(WindowApp::GetWindowWidth()) / 2.0f,float(WindowApp::GetWindowHeight()) / 2.0f };
	DirectX::XMFLOAT2 scale = { (center.x / motoScale.x) / 2.0f,(center.y / motoScale.y) / 2.0f };
	for (auto& i : panel) {
		i->SetPosition(center);
		i->SetScale(scale);
		i->SetRotation({});
		i->Update();
	}

	for (int i = 0; i < 3; i++) {
		effect[i]->ParticlAllDelete();
	}

	timer = 0;

	state = State::light_half;

	isEffect = false;

	effectPos = { 0.0f,center.y };
}

void ScreenCut::Update()
{
	if (isReset) {
		Initialize();
		isReset = false;
	}

	if (!isEffect) { return; }
	if (state != State::non) {
		func_[int(state)]();
	}
	timer++;

	for (auto& i : panel) {
		i->Update();
	}

	for (auto& i : effect) {
		i->Update();
	}
}

void ScreenCut::Draw()
{
	if (allDraw) {
		for (auto& i : panel) {
			i->Draw();
		}
	} else {
		panel[drawNum]->Draw();
	}
	for (auto& i : effect) {
		i->Draw();
	}
}

void ScreenCut::DrawImgui()
{
	ImGui::Checkbox("screenCut", &isEffect);
	ImGui::Checkbox("isReset", &isReset);
	ImGui::Checkbox("AllDraw", &allDraw);
	ImGui::SliderInt("DrawNum &d", &drawNum, 0, panelNum - 1);
	ImGui::SliderFloat("CameraDist &d", &cameraDist, 0.0f, 1.0f);
}

void ScreenCut::LightHalf()
{
	const float maxTime=5.0f;
	const float rate = timer / maxTime;
	effectPos.x = Easing::Lerp(-100.0f, float(WindowApp::GetWindowWidth()) + 100.0f, rate);

	for (int i = 0; i < 40; i++) {
		AddSpriteHalf(rate);
	}

	DirectX::XMFLOAT2 center = { float(WindowApp::GetWindowWidth()) / 2.0f,float(WindowApp::GetWindowHeight()) / 2.0f };
	const float centerDist = 20.0f;
	for (int i = 0; i < panelNum; i++) {
		DirectX::XMFLOAT2 _pos = panel[i]->GetPosition();
		if (i <= 7) {
			_pos.y = Easing::InQuint(center.y, center.y + centerDist, rate);
		} else {
			_pos.y = Easing::InQuint(center.y, center.y - centerDist, rate);
		}
		panel[i]->SetPosition(_pos);
	}

	if (rate < 1.0f) { return; }
	state = State::light_other;
	timer = 0;
}

void ScreenCut::LightOther()
{
	const float maxTime = 30.0f;
	const float rate = timer / maxTime;

	DirectX::XMFLOAT2 size = { float(WindowApp::GetWindowWidth()),float(WindowApp::GetWindowHeight()) };

	for (int i = 0; i < otherNum; i++) {
		effectPos.x = Easing::Lerp(size.x * otherMin[i].x, size.x * otherMax[i].x, rate);
		effectPos.y = Easing::Lerp(size.y * otherMin[i].y, size.y * otherMax[i].y, rate);
		AddSpriteOther(rate);
	}

	DirectX::XMFLOAT2 center = { float(WindowApp::GetWindowWidth()) / 2.0f,float(WindowApp::GetWindowHeight()) / 2.0f };
	const float centerDist = 20.0f;
	for (int i = 0; i < panelNum; i++) {
		DirectX::XMFLOAT2 _pos = panel[i]->GetPosition();
		if (i == 3 || i == 12) { continue; }
		else if (i < 3||(i>=8&&i<=11)) {
			_pos.x = Easing::InQuint(center.x, center.x + centerDist, rate);
		} else {
			_pos.x = Easing::InQuint(center.x, center.x - centerDist, rate);
		}
		panel[i]->SetPosition(_pos);
	}

	if (rate < 1.0f) { return; }
	timer = 0.0f;
	state = State::panelBreak;
}

void ScreenCut::PanelBreak()
{
	const float maxTime = 40.0f;
	const float rate = timer / maxTime;

	DirectX::XMFLOAT2 center = { float(WindowApp::GetWindowWidth()) / 2.0f,float(WindowApp::GetWindowHeight()) / 2.0f };
	const float centerDist = 1000.0f;

	for (int i = 0; i < panelNum; i++) {
		DirectX::XMFLOAT2 _pos = panel[i]->GetPosition();
		//yé≤
		if (i <= 7) {
			_pos.y = Easing::OutCubic(center.y, center.y + centerDist, rate);
		} else {
			_pos.y = Easing::OutCubic(center.y, center.y - centerDist, rate);
		}

		//xé≤
		//íÜêSÇ»ÇÃÇ≈à⁄ìÆÇµÇ»Ç¢
		if (i == 3 || i == 12) {}
		else if (i < 3 || (i >= 8 && i <= 11)) {
			_pos.x = Easing::OutCubic(center.x, center.x + centerDist, rate);
		} else {
			_pos.x = Easing::OutCubic(center.x, center.x - centerDist, rate);
		}

		float rotation= Easing::OutCubic(0.0f, rota[i], rate);
		panel[i]->SetPosition(_pos);
		panel[i]->SetRotation(rotation);
	}

	if (rate > 0.1f) {
		for (int i = 0; i < 3; i++) {
			effect[i]->ParticlAllDelete();
		}
	}

	if (rate < 1.0f) { return; }
	state = State::non;
}

void ScreenCut::AddSpriteHalf(const float _rate)
{
	using namespace DirectX;
	int num = RandomInt(29) / 10;
	XMFLOAT3 worldPos;
	const XMFLOAT2 dist = { (RandomFloat(3000.0f) - (3000.0f * _rate)) * 0.1f,0.0f };
	Collision::CalcScreenToWorld(&worldPos, ParticleManager::GetCamera(), effectPos + dist, cameraDist);

	const XMFLOAT4 _color = { 0.4f,0.1f,0.4f,1.0f };
	const float _scale = (RandomFloat(30.0f) + 20.0f) * 0.1f;
	//í«â¡
	effect[num]->Add(150, worldPos, {}, {}, _scale, _scale, _color, _color);
}

void ScreenCut::AddSpriteOther(const float _rate)
{
	using namespace DirectX;
	int num = RandomInt(29) / 10;
	XMFLOAT3 worldPos;
	Collision::CalcScreenToWorld(&worldPos, ParticleManager::GetCamera(), effectPos, cameraDist);
	const XMFLOAT2 dist = { 0.0f,0.0f };
	const XMFLOAT4 _color = { 0.4f,0.1f,0.4f,1.0f };
	const float _scale = (RandomFloat(30.0f - (30.0f * _rate)) + 5.0f) * 0.1f;
	//í«â¡
	effect[num]->Add(150, worldPos, {}, {}, _scale, _scale, _color, _color);
}