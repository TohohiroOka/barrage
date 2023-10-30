#pragma once
#include "Object/2d/Sprite.h"
#include "GameInputManager.h"

/// <summary>
/// 入力設定画面
/// </summary>
class ActionInputConfig
{
public: //enum
	/// <summary>
	/// フェーズ
	/// </summary>
	enum Phase 
	{
		SelectMode,
		InputChangeMode,
	};

public: //静的メンバ関数
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	static void LoadTexture();

private: //静的メンバ関数
	/// <summary>
	/// 行動名テクスチャ読み込み(行動名に対応させるため、入力行動型の名前を付ける)
	/// </summary>
	static void LoadActionNameTexture(int actionName, const std::string& fileName);

	/// <summary>
	/// キーテクスチャ読み込み(キー入力に対応させるため、BYTE型の名前を付ける)
	/// </summary>
	static void LoadKeyTexture(BYTE num, const std::string& fileName);

	/// <summary>
	/// パッドボタンテクスチャ読み込み(パッド入力に対応させるため、ボタン型の名前を付ける)
	/// </summary>
	static void LoadPadTexture(int num, const std::string& fileName);

public: //メンバ関数
	ActionInputConfig();
	~ActionInputConfig();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	//getter
	bool GetIsInputConfigEnd() { return isInputConfigEnd; }

private: //メンバ関数
	/// <summary>
	/// 選択
	/// </summary>
	void Select();

	/// <summary>
	/// 入力変更
	/// </summary>
	void InputChange();

public: //静的メンバ変数
	//フェーズの挙動関数
	static void (ActionInputConfig::* phaseFuncTable[])();
	//通常の色
	static DirectX::XMFLOAT4 normalColor;
	//変更できない状態の枠の色
	static DirectX::XMFLOAT4 noChangeColor;
	//選択中の色
	static DirectX::XMFLOAT4 selectColor;

private: //メンバ変数
	//仮置き背景用黒スプライト
	std::unique_ptr<Sprite> backSprite;

	//行動名スプライト
	std::array<std::unique_ptr<Sprite>, GameInputManager::InputActionNum> actionNameSprites;
	//キー・ボタンスプライト
	std::array<std::array<std::unique_ptr<Sprite>, GameInputManager::InputActionNum>, 2> configSprites;
	//フレームスプライト
	std::array<std::array<std::unique_ptr<Sprite>, GameInputManager::InputActionNum>, 2> frameSprites;

	//選択中の行動番号
	int selectAction = 0;
	//選択中の入力形態番号
	int selectInputType = 0;

	//フェーズ
	Phase phase = SelectMode;

	//入力設定終了フラグ
	bool isInputConfigEnd = false;
};