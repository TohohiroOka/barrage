#pragma once
#include "ActionInputConfigBase.h"

/// <summary>
/// ���͐ݒ���(�L�[)
/// </summary>
class ActionInputConfigKey : public ActionInputConfigBase
{
public: //�����o�֐�
	ActionInputConfigKey();
	~ActionInputConfigKey();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset() override;

private: //�����o�֐�
	/// <summary>
	/// �I��
	/// </summary>
	void SelectModeUpdate() override;

	/// <summary>
	/// ���͕ύX
	/// </summary>
	void InputChangeModeUpdate() override;
};