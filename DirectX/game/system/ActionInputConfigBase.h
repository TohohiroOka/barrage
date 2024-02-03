#pragma once
#include "Object/2d/Sprite.h"
#include "GameInputManager.h"

/// <summary>
/// ���͐ݒ���
/// </summary>
class ActionInputConfigBase
{
public: //enum
	/// <summary>
	/// �t�F�[�Y
	/// </summary>
	enum Phase 
	{
		SelectMode,
		InputChangeMode,
	};

	/// <summary>
	/// ���͕ύX���
	/// </summary>
	enum InputType 
	{
		Config,
		CameraRota,

		InputTypeNum,
	};

	/// <summary>
	/// �J������]��ޖ�
	/// </summary>
	enum CameraRotaName
	{
		CameraUpDown,
		CameraLeftRight,

		CameraRotaTypeNum,
	};

	/// <summary>
	/// ����������
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

public: //�ÓI�����o�֐�
	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	static void LoadTexture();

protected: //�ÓI�����o�֐�
	/// <summary>
	/// �s�����e�N�X�`���ǂݍ���(�s�����ɑΉ������邽�߁A���͍s���^�̖��O��t����)
	/// </summary>
	static void LoadActionNameTexture(int actionName, const std::string& fileName);

	/// <summary>
	/// �L�[�e�N�X�`���ǂݍ���(�L�[���͂ɑΉ������邽�߁ABYTE�^�̖��O��t����)
	/// </summary>
	static void LoadKeyTexture(BYTE num, const std::string& fileName);

	/// <summary>
	/// �p�b�h�{�^���e�N�X�`���ǂݍ���(�p�b�h���͂ɑΉ������邽�߁A�{�^���^�̖��O��t����)
	/// </summary>
	static void LoadPadTexture(int num, const std::string& fileName);

public: //�����o�֐�
	ActionInputConfigBase();
	~ActionInputConfigBase();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	virtual void Reset();

	//getter
	bool GetIsInputConfigEnd() { return isInputConfigEnd; }

protected: //�����o�֐�
	/// <summary>
	/// �I��
	/// </summary>
	virtual void SelectModeUpdate() = 0;

	/// <summary>
	/// ���͕ύX
	/// </summary>
	virtual void InputChangeModeUpdate() = 0;

	/// <summary>
	/// �J�����̉�]������ύX
	/// </summary>
	void CameraRotationChange();

public: //�ÓI�����o�ϐ�
	//�t�F�[�Y�̋����֐�
	static void (ActionInputConfigBase::* phaseFuncTable[])();
	//�ʏ�̐F
	static DirectX::XMFLOAT4 normalColor;
	//�ύX�ł��Ȃ���Ԃ̘g�̐F
	static DirectX::XMFLOAT4 noChangeColor;
	//�I�𒆂̐F
	static DirectX::XMFLOAT4 selectColor;

protected: //�����o�ϐ�
	//���u���w�i�p���X�v���C�g
	std::unique_ptr<Sprite> backSprite;

	//�s�����X�v���C�g
	std::array<std::unique_ptr<Sprite>, GameInputManager::Lockon + 1> actionNameSprites;
	//�L�[�E�{�^���X�v���C�g(�v�f2�́A�J������]�p�Ȃ̂ŗv�f��2��)
	std::array<std::vector<std::unique_ptr<Sprite>>, InputTypeNum> configSprites;
	//�t���[���X�v���C�g
	std::array<std::vector<std::unique_ptr<Sprite>>, InputTypeNum> frameSprites;
	//�����X�v���C�g
	std::array<std::unique_ptr<Sprite>, InfoNameNum> infoSprites;

	//�I�𒆂̍s���ԍ�
	int selectAction = 0;
	//�I�𒆂̓��͌`�Ԕԍ�
	int selectInputType = 0;

	//�t�F�[�Y
	Phase phase = SelectMode;

	//���͐ݒ�I���t���O
	bool isInputConfigEnd = false;
};