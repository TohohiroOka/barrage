#pragma once
#include "Object/2d/Sprite.h"
#include "GameInputManager.h"

/// <summary>
/// ���͐ݒ���
/// </summary>
class ActionInputConfig
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

public: //�ÓI�����o�֐�
	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	static void LoadTexture();

private: //�ÓI�����o�֐�
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
	ActionInputConfig();
	~ActionInputConfig();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

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
	void Reset();

	//getter
	bool GetIsInputConfigEnd() { return isInputConfigEnd; }

private: //�����o�֐�
	/// <summary>
	/// �I��
	/// </summary>
	void Select();

	/// <summary>
	/// ���͕ύX
	/// </summary>
	void InputChange();

public: //�ÓI�����o�ϐ�
	//�t�F�[�Y�̋����֐�
	static void (ActionInputConfig::* phaseFuncTable[])();
	//�ʏ�̐F
	static DirectX::XMFLOAT4 normalColor;
	//�ύX�ł��Ȃ���Ԃ̘g�̐F
	static DirectX::XMFLOAT4 noChangeColor;
	//�I�𒆂̐F
	static DirectX::XMFLOAT4 selectColor;

private: //�����o�ϐ�
	//���u���w�i�p���X�v���C�g
	std::unique_ptr<Sprite> backSprite;

	//�s�����X�v���C�g
	std::array<std::unique_ptr<Sprite>, GameInputManager::InputActionNum> actionNameSprites;
	//�L�[�E�{�^���X�v���C�g
	std::array<std::array<std::unique_ptr<Sprite>, GameInputManager::InputActionNum>, 2> configSprites;
	//�t���[���X�v���C�g
	std::array<std::array<std::unique_ptr<Sprite>, GameInputManager::InputActionNum>, 2> frameSprites;

	//�I�𒆂̍s���ԍ�
	int selectAction = 0;
	//�I�𒆂̓��͌`�Ԕԍ�
	int selectInputType = 0;

	//�t�F�[�Y
	Phase phase = SelectMode;

	//���͐ݒ�I���t���O
	bool isInputConfigEnd = false;
};