#pragma once
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include <array>
#include <functional>

/// <summary>
/// ゲーム使用キー(ボタン)配置管理
/// </summary>
class GameInputManager
{
public: //enum
	/// <summary>
	/// 入力に対する行動名
	/// </summary>
	enum InputAction
	{
		MoveForward,	//移動(前)
		MoveBack,		//移動(後)
		MoveLeft,		//移動(左)
		MoveRight,		//移動(右)
		Avoid_Blink_Dash,//回避&ブリンク&ダッシュ
		Jump,			//ジャンプ
		Attack,			//攻撃
		MoveCameraUp,	//カメラ移動(上)
		MoveCameraDown,	//カメラ移動(下)
		MoveCameraLeft,	//カメラ移動(左)
		MoveCameraRight,//カメラ移動(右)
		Lockon,			//ロックオン

		InputActionNum,	//入力対応数
	};

public: //型
	/// <summary>
	/// 一つのキー入力のデータ
	/// </summary>
	struct KeyInputActionData
	{
		BYTE key;
		bool isChangeInput;
	};

	/// <summary>
	/// 一つのパッド入力のデータ
	/// </summary>
	struct PadInputActionData
	{
		int padButton;
		bool isChangeInput;
	};

public: //静的メンバ関数
	/// <summary>
	/// キー(ボタン)配置初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// キー配置変更
	/// </summary>
	/// <param name="inputAction">変更する行動</param>
	/// <param name="key">変更後のキー</param>
	/// <returns>変更できたか</returns>
	static bool ChangeInputKey(InputAction inputAction, BYTE key);

	/// <summary>
	/// パッドボタン配置変更
	/// </summary>
	/// <param name="inputAction">変更する行動</param>
	/// <param name="padButton">変更後のパッドボタン</param>
	/// <returns>変更できたか</returns>
	static bool ChangeInputPadButton(InputAction inputAction, int padButton);

	//getter
	//キー入力パッドボタン入力両方が欲しい場合に使用する取得関数
	static bool PushInputAction(InputAction inputAction) {
		return DirectInput::GetInstance()->PushKey(keyInputActions[inputAction].key) ||
			XInputManager::GetInstance()->PushButton(padInputActions[inputAction].padButton);
	}
	static bool TriggerInputAction(InputAction inputAction) {
		return DirectInput::GetInstance()->TriggerKey(keyInputActions[inputAction].key) ||
			XInputManager::GetInstance()->TriggerButton(padInputActions[inputAction].padButton);
	}

	//キー入力パッドボタン入力個別で欲しい場合に使用する取得関数
	static KeyInputActionData& GetKeyInputActionData(int inputAction) { return keyInputActions[inputAction]; }
	static PadInputActionData& GetPadInputActionData(int inputAction) { return padInputActions[inputAction]; }

	static float GetPadStickInputIncline() { return padStickInputIncline; }
	static DirectX::XMFLOAT2 GetPadLStickIncline() { return XInputManager::GetInstance()->GetPadLStickIncline(); }
	static DirectX::XMFLOAT2 GetPadRStickIncline() { return XInputManager::GetInstance()->GetPadRStickIncline(); }
	static float GetPadLStickAngle() { return XInputManager::GetInstance()->GetPadLStickAngle(); }
	static float GetPadRStickAngle() { return XInputManager::GetInstance()->GetPadRStickAngle(); }
	static float GetPadLStickRadian() { return DirectX::XMConvertToRadians(GetPadLStickAngle() - 90); }
	static float GetPadRStickRadian() { return DirectX::XMConvertToRadians(GetPadRStickAngle() - 90); }
	static bool GetIsCameraMoveYReverse() { return isCameraMoveYReverse; }
	static bool GetIsCameraMoveXReverse() { return isCameraMoveXReverse; }

private: //静的メンバ変数
	//キー入力データ
	static std::array<KeyInputActionData, InputActionNum> keyInputActions;
	//パッド入力データ
	static std::array<PadInputActionData, InputActionNum> padInputActions;
	//カメラ移動を逆にするか
	static bool isCameraMoveYReverse; //上下
	static bool isCameraMoveXReverse; //左右
	//パッドスティック入力判定傾き量
	static float padStickInputIncline;
};