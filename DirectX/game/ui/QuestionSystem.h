#pragma once
#include "TextCreator.h"
#include "Math/Timer.h"
#include <functional>

/// <summary>
/// ����𓚃V�X�e��
/// </summary>
class QuestionSystem
{
public: //enum
	//�t�F�[�Y
	enum class QuestionPhase
	{
		START_WAIT,	//�{�^���A�łɂ��I����h�����߂́A�������琔�t���[���ҋ@���
		SELECT_CHOICE, //�I�����
		CONFIRM_SELECTION, //�I���m��
	};

public: //�����o�֐�
	QuestionSystem(const std::vector<std::wstring>& choices);
	~QuestionSystem();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//getter
	int GetSelectNum() { return selectNum; }
	bool GetIsEnd() { return isEnd; }

private: //�����o�֐�
	/// <summary>
	/// �ҋ@��Ԃ̍X�V����
	/// </summary>
	void StartWaitUpdate();

	/// <summary>
	/// �I����Ԃ̍X�V����
	/// </summary>
	void SelectChoiceUpdate();

	/// <summary>
	/// �I����ύX����
	/// </summary>
	void ChangeSelectChoices();

	/// <summary>
	/// �I�������肷��
	/// </summary>
	void SelectChoice();

	/// <summary>
	/// �I���m��̍X�V����
	/// </summary>
	void ConfirmSelectionUpdate();

private: //�����o�ϐ�
	//�t�F�[�Y
	QuestionPhase phase = QuestionPhase::START_WAIT;
	//�e�t�F�[�Y�̓���
	std::vector<std::function<void()>> func;
	//�I�����e�L�X�g�X�v���C�g
	std::vector<std::unique_ptr<TextCreator>> choiceTextSprites;
	//�I�����̍���
	std::vector<float> textPosYs;
	//�w�i�X�v���C�g
	std::unique_ptr<Sprite> backSprite;
	//�I�������w�i�X�v���C�g
	std::unique_ptr<Sprite> choiceBackSprite;
	//���S���W
	DirectX::XMFLOAT2 centerPos;
	//�I������
	int choicesNum = 0;
	//�I��ԍ�
	int selectNum = 0;
	//�X�V�����p�^�C�}�[
	std::unique_ptr<Engine::Timer> timer;
	//�m��㉉�o���s������
	int selectDirectionCount = 0;
	//�I���t���O
	bool isEnd = false;
};