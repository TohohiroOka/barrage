#include "GameInputManager.h"

std::array<GameInputManager::KeyInputActionData, GameInputManager::InputActionNum> GameInputManager::keyInputActions;
std::array<GameInputManager::PadInputActionData, GameInputManager::InputActionNum> GameInputManager::padInputActions;
bool GameInputManager::isCameraMoveYReverse = false;
bool GameInputManager::isCameraMoveXReverse = true;
float GameInputManager::padStickInputIncline = 0.3f;

void GameInputManager::Initialize()
{
	//キー入力初期設定
	keyInputActions[MoveForward] = { DIK_W, true };
	keyInputActions[MoveBack] = { DIK_S, true };
	keyInputActions[MoveLeft] = { DIK_A, true };
	keyInputActions[MoveRight] = { DIK_D, true };
	keyInputActions[Avoid_Blink_Dash] = { DIK_Z, true };
	keyInputActions[Jump] = { DIK_SPACE, true };
	keyInputActions[Attack] = { DIK_Q, true };
	keyInputActions[MoveCameraUp] = { DIK_UP, false };
	keyInputActions[MoveCameraDown] = { DIK_DOWN, false };
	keyInputActions[MoveCameraLeft] = { DIK_LEFT, false };
	keyInputActions[MoveCameraRight] = { DIK_RIGHT, false };
	keyInputActions[Lockon] = { DIK_X, true };


	//パッド入力初期設定
	const int LStickNum = 16; //ボタン入力でない用の仮置きボタン番号(左スティック：16)
	const int RStickNum = 17; //ボタン入力でない用の仮置きボタン番号(右スティック：17)
	padInputActions[MoveForward] = { LStickNum, false };
	padInputActions[MoveBack] = { LStickNum, false };
	padInputActions[MoveLeft] = { LStickNum, false };
	padInputActions[MoveRight] = { LStickNum, false };
	padInputActions[Avoid_Blink_Dash] = { XInputManager::PAD_B, true };
	padInputActions[Jump] = { XInputManager::PAD_A, true };
	padInputActions[Attack] = { XInputManager::PAD_RB, true };
	padInputActions[MoveCameraUp] = { RStickNum, false };
	padInputActions[MoveCameraDown] = { RStickNum, false };
	padInputActions[MoveCameraLeft] = { RStickNum, false };
	padInputActions[MoveCameraRight] = { RStickNum, false };
	padInputActions[Lockon] = { XInputManager::PAD_RIGHT_STICK_PUSH, true };
}

bool GameInputManager::ChangeInputKey(InputAction inputAction, BYTE key)
{
	//変更後のキーが他のキーと被りがないかをチェック
	for (auto& keyInputAction : keyInputActions) {
		if (key == keyInputActions[inputAction].key) { continue; }
		if (key == keyInputAction.key) { return false; }
	}

	//指定したキーを割り当て
	keyInputActions[inputAction].key = key;

	return true;
}

bool GameInputManager::ChangeInputPadButton(InputAction inputAction, int padButton)
{
	//変更後のキーが他のキーと被りがないかをチェック
	for (auto& padInputAction : padInputActions) {
		if (padButton == padInputActions[inputAction].padButton) { continue; }
		if (padButton == padInputAction.padButton) { return false; }
	}

	//指定したキーを割り当て
	padInputActions[inputAction].padButton = padButton;

	return true;
}
