#pragma once
#include "ActionInputConfigBase.h"

/// <summary>
/// 入力設定画面(パッド)
/// </summary>
class ActionInputConfigPad : public ActionInputConfigBase
{
public: //メンバ関数
	ActionInputConfigPad();
	~ActionInputConfigPad();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset() override;

private: //メンバ関数
	/// <summary>
	/// 選択
	/// </summary>
	void SelectModeUpdate() override;

	/// <summary>
	/// 入力変更
	/// </summary>
	void InputChangeModeUpdate() override;
};