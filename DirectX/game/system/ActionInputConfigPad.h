#pragma once
#include "ActionInputConfigBase.h"

/// <summary>
/// ���͐ݒ���(�p�b�h)
/// </summary>
class ActionInputConfigPad : public ActionInputConfigBase
{
public: //�����o�֐�
	ActionInputConfigPad();
	~ActionInputConfigPad();

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