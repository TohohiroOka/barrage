#include "GameInputManager.h"

std::array<GameInputManager::KeyInputActionData, GameInputManager::InputActionNum> GameInputManager::keyInputActions;
std::array<GameInputManager::PadInputActionData, GameInputManager::InputActionNum> GameInputManager::padInputActions;
bool GameInputManager::isCameraRotaYReverse = false;
bool GameInputManager::isCameraRotaXReverse = false;
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
	keyInputActions[LightAttack] = { DIK_Q, true };
	keyInputActions[StrongAttack] = { DIK_E, true };
	keyInputActions[CameraUpRota] = { DIK_UP, false };
	keyInputActions[CameraDownRota] = { DIK_DOWN, false };
	keyInputActions[CameraLeftRota] = { DIK_LEFT, false };
	keyInputActions[CameraRightRota] = { DIK_RIGHT, false };
	keyInputActions[Lockon] = { DIK_X, true };
	keyInputActions[Select] = { DIK_SPACE, false };
	keyInputActions[Pause] = { DIK_ESCAPE, false };

	//パッド入力初期設定
	const int LStickNum = 16; //ボタン入力でない用の仮置きボタン番号(左スティック：16)
	const int RStickNum = 17; //ボタン入力でない用の仮置きボタン番号(右スティック：17)
	padInputActions[MoveForward] = { LStickNum, false };
	padInputActions[MoveBack] = { LStickNum, false };
	padInputActions[MoveLeft] = { LStickNum, false };
	padInputActions[MoveRight] = { LStickNum, false };
	padInputActions[Avoid_Blink_Dash] = { XInputManager::PAD_B, true };
	padInputActions[Jump] = { XInputManager::PAD_A, true };
	padInputActions[LightAttack] = { XInputManager::PAD_RB, true };
	padInputActions[StrongAttack] = { XInputManager::PAD_RT, true };
	padInputActions[CameraUpRota] = { RStickNum, false };
	padInputActions[CameraDownRota] = { RStickNum, false };
	padInputActions[CameraLeftRota] = { RStickNum, false };
	padInputActions[CameraRightRota] = { RStickNum, false };
	padInputActions[Lockon] = { XInputManager::PAD_RIGHT_STICK_PUSH, true };
	padInputActions[Select] = { XInputManager::PAD_A, true };
	padInputActions[Pause] = { XInputManager::PAD_START, true };
}

bool GameInputManager::ChangeInputKey(InputAction inputAction, BYTE key)
{
	//変更後のキーが他のキーと被りがないかをチェック
	for (int i = 0; i < InputActionNum; i++) {
		if (i == (int)Select) { continue; }
		if (key == keyInputActions[inputAction].key) { continue; }
		if (key == keyInputActions[i].key) { return false; }
	}

	//指定したキーを割り当て
	keyInputActions[inputAction].key = key;

	return true;
}

bool GameInputManager::ChangeInputPadButton(InputAction inputAction, int padButton)
{
	//変更後のキーが他のキーと被りがないかをチェック
	for (int i = 0; i < InputActionNum; i++) {
		if (i == (int)Select) { continue; }
		if (padButton == padInputActions[inputAction].padButton) { continue; }
		if (padButton == padInputActions[i].padButton) { return false; }
	}

	//指定したキーを割り当て
	padInputActions[inputAction].padButton = padButton;

	return true;
}
