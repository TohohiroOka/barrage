#pragma once
#include "TextCreator.h"
#include "Math/Timer.h"
#include <functional>

/// <summary>
/// 質問解答システム
/// </summary>
class QuestionSystem
{
public: //enum
	//フェーズ
	enum class QuestionPhase
	{
		START_WAIT,	//ボタン連打による選択を防ぐための、生成から数フレーム待機状態
		SELECT_CHOICE, //選択状態
		CONFIRM_SELECTION, //選択確定
	};

public: //メンバ関数
	QuestionSystem(const std::vector<std::wstring>& choices);
	~QuestionSystem();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	int GetSelectNum() { return selectNum; }
	bool GetIsEnd() { return isEnd; }

private: //メンバ関数
	/// <summary>
	/// 待機状態の更新処理
	/// </summary>
	void StartWaitUpdate();

	/// <summary>
	/// 選択状態の更新処理
	/// </summary>
	void SelectChoiceUpdate();

	/// <summary>
	/// 選択を変更する
	/// </summary>
	void ChangeSelectChoices();

	/// <summary>
	/// 選択を決定する
	/// </summary>
	void SelectChoice();

	/// <summary>
	/// 選択確定の更新処理
	/// </summary>
	void ConfirmSelectionUpdate();

private: //メンバ変数
	//フェーズ
	QuestionPhase phase = QuestionPhase::START_WAIT;
	//各フェーズの動き
	std::vector<std::function<void()>> func;
	//選択肢テキストスプライト
	std::vector<std::unique_ptr<TextCreator>> choiceTextSprites;
	//選択肢の高さ
	std::vector<float> textPosYs;
	//背景スプライト
	std::unique_ptr<Sprite> backSprite;
	//選択強調背景スプライト
	std::unique_ptr<Sprite> choiceBackSprite;
	//中心座標
	DirectX::XMFLOAT2 centerPos;
	//選択肢数
	int choicesNum = 0;
	//選択番号
	int selectNum = 0;
	//更新処理用タイマー
	std::unique_ptr<Engine::Timer> timer;
	//確定後演出を行った回数
	int selectDirectionCount = 0;
	//終了フラグ
	bool isEnd = false;
};