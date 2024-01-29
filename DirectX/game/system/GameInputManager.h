#pragma once
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include <array>
#include <functional>

/// <summary>
/// �Q�[���g�p�L�[(�{�^��)�z�u�Ǘ�
/// </summary>
class GameInputManager
{
public: //enum
	/// <summary>
	/// ���͂ɑ΂���s����
	/// </summary>
	enum InputAction
	{
		MoveForward,	//�ړ�(�O)
		MoveBack,		//�ړ�(��)
		MoveLeft,		//�ړ�(��)
		MoveRight,		//�ړ�(�E)
		Avoid_Blink_Dash,//���&�u�����N&�_�b�V��
		Jump,			//�W�����v
		LightAttack,	//��U��
		StrongAttack,	//���U��
		CameraUpRota,	//�J������](��)
		CameraDownRota,	//�J������](��)
		CameraLeftRota,	//�J������](��)
		CameraRightRota,//�J������](�E)
		Lockon,			//���b�N�I��

		Select,			//�I��
		Pause,			//�|�[�Y

		InputActionNum,	//���͑Ή���
	};

public: //�^
	/// <summary>
	/// ��̃L�[���͂̃f�[�^
	/// </summary>
	struct KeyInputActionData
	{
		BYTE key;
		bool isChangeInput;
	};

	/// <summary>
	/// ��̃p�b�h���͂̃f�[�^
	/// </summary>
	struct PadInputActionData
	{
		int padButton;
		bool isChangeInput;
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// �L�[(�{�^��)�z�u������
	/// </summary>
	static void Initialize();

	/// <summary>
	/// �L�[�z�u�ύX
	/// </summary>
	/// <param name="inputAction">�ύX����s��</param>
	/// <param name="key">�ύX��̃L�[</param>
	/// <returns>�ύX�ł�����</returns>
	static bool ChangeInputKey(InputAction inputAction, BYTE key);

	/// <summary>
	/// �p�b�h�{�^���z�u�ύX
	/// </summary>
	/// <param name="inputAction">�ύX����s��</param>
	/// <param name="padButton">�ύX��̃p�b�h�{�^��</param>
	/// <returns>�ύX�ł�����</returns>
	static bool ChangeInputPadButton(InputAction inputAction, int padButton);

	//getter
	//�L�[���̓p�b�h�{�^�����͗������~�����ꍇ�Ɏg�p����擾�֐�
	static bool PushInputAction(InputAction inputAction) {
		return DirectInput::GetInstance()->PushKey(keyInputActions[inputAction].key) ||
			XInputManager::GetInstance()->PushButton(padInputActions[inputAction].padButton);
	}
	static bool TriggerInputAction(InputAction inputAction) {
		return DirectInput::GetInstance()->TriggerKey(keyInputActions[inputAction].key) ||
			XInputManager::GetInstance()->TriggerButton(padInputActions[inputAction].padButton);
	}

	//�L�[���̓p�b�h�{�^�����͌ʂŗ~�����ꍇ�Ɏg�p����擾�֐�
	static KeyInputActionData& GetKeyInputActionData(int inputAction) { return keyInputActions[inputAction]; }
	static PadInputActionData& GetPadInputActionData(int inputAction) { return padInputActions[inputAction]; }

	static float GetPadStickInputIncline() { return padStickInputIncline; }
	static DirectX::XMFLOAT2 GetPadLStickIncline() { return XInputManager::GetInstance()->GetPadLStickIncline(); }
	static DirectX::XMFLOAT2 GetPadRStickIncline() { return XInputManager::GetInstance()->GetPadRStickIncline(); }
	static float GetPadLStickAngle() { return XInputManager::GetInstance()->GetPadLStickAngle(); }
	static float GetPadRStickAngle() { return XInputManager::GetInstance()->GetPadRStickAngle(); }
	static float GetPadLStickRadian() { return DirectX::XMConvertToRadians(GetPadLStickAngle() - 90); }
	static float GetPadRStickRadian() { return DirectX::XMConvertToRadians(GetPadRStickAngle() - 90); }
	static bool GetIsCameraRotaYReverse() { return isCameraRotaYReverse; }
	static bool GetIsCameraRotaXReverse() { return isCameraRotaXReverse; }

	//setter
	static void SetIsCameraRotaYReverse(bool isCameraRotaYReverse) { GameInputManager::isCameraRotaYReverse = isCameraRotaYReverse; }
	static void SetIsCameraRotaXReverse(bool isCameraRotaXReverse) { GameInputManager::isCameraRotaXReverse = isCameraRotaXReverse; }

private: //�ÓI�����o�ϐ�
	//�L�[���̓f�[�^
	static std::array<KeyInputActionData, InputActionNum> keyInputActions;
	//�p�b�h���̓f�[�^
	static std::array<PadInputActionData, InputActionNum> padInputActions;
	//�J������]���t�ɂ��邩
	static bool isCameraRotaYReverse; //���E
	static bool isCameraRotaXReverse; //�㉺
	//�p�b�h�X�e�B�b�N���͔���X����
	static float padStickInputIncline;
};