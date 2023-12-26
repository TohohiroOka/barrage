#pragma once
#include "DirectXCommon.h"
#include "Scene/SceneManager.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "Object/2d/PostEffect/BaseRender.h"
#include "Object/2d/PostEffect/PostEffect.h"
#include "Object/2d/PostEffect/Bloom.h"
#include "Object/2d/PostEffect/Outline.h"
#include "Object/2d/PostEffect/Fog.h"
#include "Object/2d/PostEffect/DepthRender.h"
#include "Object/3d/ShadowMap.h"
#include "FrameRateKeep.h"

#include <sstream>
#include <iomanip>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

class WindowApp;

class MainEngine
{
public:

	MainEngine() = default;
	~MainEngine();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <returns>ゲームを続けるか</returns>
	bool Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// imgui描画
	/// </summary>
	void ImguiDraw();

	/// <summary>
	/// フレームレートキープ
	/// </summary>
	void FrameControl();

private:

	//DirectXCommonのインスタンス
	std::unique_ptr<DirectXCommon> dXCommon = nullptr;
	//Inputのインスタンス
	DirectInput* input = nullptr;
	//XInputManager
	XInputManager* xinput = nullptr;
	//GameSceneのインスタンス
	std::unique_ptr<SceneManager> scene = nullptr;
	//レンダーターゲット
	std::array<std::unique_ptr<BaseRender>, int(Engine::EngineUseRTVCreateTextureName::size)> render;
	//描画
	std::unique_ptr<Sprite> mainSprite;
	//ポストエフェクトのインスタンス
	std::unique_ptr<PostEffect> postEffect;
	//ポストエフェクト加工用
	std::unique_ptr<Bloom> bloom;
	//ポストエフェクト加工用
	std::unique_ptr<Outline> outline;
	//ポストエフェクト加工用
	std::unique_ptr<Fog> fog;
	//ポストエフェクトで使用する深度
	std::unique_ptr<DepthRender> depthRender;
	//シャドウマップ
	std::unique_ptr<DepthRender> shadowMapRender;
	//Fps固定用クラスのインスタンス
	std::unique_ptr<FrameRateKeep> fps = nullptr;
	//数字表示デバッグ用
	wchar_t str[256] = {};
};