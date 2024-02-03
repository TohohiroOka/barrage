#pragma once
#include "Object/2d/Sprite.h"
#include "GameInputManager.h"

/// <summary>
/// 入力設定画面
/// </summary>
class ActionInputConfigBase
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

	/// <summary>
	/// 入力変更種類
	/// </summary>
	enum InputType 
	{
		Config,
		CameraRota,

		InputTypeNum,
	};

	/// <summary>
	/// カメラ回転種類名
	/// </summary>
	enum CameraRotaName
	{
		CameraUpDown,
		CameraLeftRight,

		CameraRotaTypeNum,
	};

	/// <summary>
	/// 説明役割名
	/// </summary>
	enum InfoName
	{
		Select_Button,
		Select_Text,
		Back_Button,
		Back_Text,
		Button_Choice_Text,

		InfoNameNum,
	};

public: //静的メンバ関数
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	static void LoadTexture();

protected: //静的メンバ関数
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
	ActionInputConfigBase();
	~ActionInputConfigBase();

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
	virtual void Reset();

	//getter
	bool GetIsInputConfigEnd() { return isInputConfigEnd; }

protected: //メンバ関数
	/// <summary>
	/// 選択
	/// </summary>
	virtual void SelectModeUpdate() = 0;

	/// <summary>
	/// 入力変更
	/// </summary>
	virtual void InputChangeModeUpdate() = 0;

	/// <summary>
	/// カメラの回転方向を変更
	/// </summary>
	void CameraRotationChange();

public: //静的メンバ変数
	//フェーズの挙動関数
	static void (ActionInputConfigBase::* phaseFuncTable[])();
	//通常の色
	static DirectX::XMFLOAT4 normalColor;
	//変更できない状態の枠の色
	static DirectX::XMFLOAT4 noChangeColor;
	//選択中の色
	static DirectX::XMFLOAT4 selectColor;

protected: //メンバ変数
	//仮置き背景用黒スプライト
	std::unique_ptr<Sprite> backSprite;

	//行動名スプライト
	std::array<std::unique_ptr<Sprite>, GameInputManager::Lockon + 1> actionNameSprites;
	//キー・ボタンスプライト(要素2は、カメラ回転用なので要素は2個)
	std::array<std::vector<std::unique_ptr<Sprite>>, InputTypeNum> configSprites;
	//フレームスプライト
	std::array<std::vector<std::unique_ptr<Sprite>>, InputTypeNum> frameSprites;
	//説明スプライト
	std::array<std::unique_ptr<Sprite>, InfoNameNum> infoSprites;

	//選択中の行動番号
	int selectAction = 0;
	//選択中の入力形態番号
	int selectInputType = 0;

	//フェーズ
	Phase phase = SelectMode;

	//入力設定終了フラグ
	bool isInputConfigEnd = false;
};