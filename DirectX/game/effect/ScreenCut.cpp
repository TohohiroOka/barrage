#include "ScreenCut.h"
#include "WindowApp.h"

ScreenCut::ScreenCut()
{
	//ì«Ç›çûÇ›
	for (int i = 0; i < panelNum; i++) {
		model[i] = Model::CreateFromOBJ("planeBreak/planeBreak_" + std::to_string(i + 1));
	}

	DirectX::XMFLOAT2 motoScale = { 192.0f / 2.0f,108.0f / 2.0f };
	DirectX::XMFLOAT2 center = { float(WindowApp::GetWindowWidth()) / 2.0f,float(WindowApp::GetWindowHeight()) / 2.0f };
	DirectX::XMFLOAT2 scale = { (center.x / motoScale.x) / 2.0f,(center.y / motoScale.y) / 2.0f };
	for (int i = 0; i < panelNum; i++) {
		panel[i] = ShaoeSprite::Create("main_after", {});
		panel[i]->SetModel(model[i].get());
		panel[i]->SetPosition(center);
		panel[i]->SetScale(scale);
	}

	timer = std::make_unique<Engine::Timer>();

	state = State::light;

	func_.emplace_back([this] {return Light(); });
	func_.emplace_back([this] {return PanelBreak(); });

	Initialize();
}

void ScreenCut::Initialize()
{
	isEffect = true;
}

void ScreenCut::Update()
{
	if (!isEffect) { return; }
	func_[int(state)]();
	timer->Update();

	for (auto& i : panel) {
		i->Update();
	}
}

void ScreenCut::Draw()
{
	for (auto& i : panel) {
		i->Draw();
	}
	//panel[3]->Draw();
}

void ScreenCut::Light()
{
	state = State::panelBreak;
}

void ScreenCut::PanelBreak()
{
	
}