#include "QuestionSystem.h"
#include "WindowApp.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "Math/Easing/Easing.h"

QuestionSystem::QuestionSystem(const std::vector<std::wstring>& choices)
{
	//選択肢数をセット
	choicesNum = (int)choices.size();

	//選択肢のテキストスプライトを生成
	centerPos = { (float)WindowApp::GetWindowWidth() / 2, (float)WindowApp::GetWindowHeight() / 2 - (float)WindowApp::GetWindowHeight() * 0.05f };
	const float textScale = 1.2f;
	int charNumMax = 0;
	for (int i = 0; i < choicesNum; i++) {
		std::unique_ptr<TextCreator> newText;
		const DirectX::XMFLOAT2 pos = {
			centerPos.x - ((CharSprite::charTextureSize - 2) * textScale) / 2 * (float)(choices[i].size() - 1),
			(centerPos.y - ((CharSprite::charTextureSize + 15) * textScale / 2 * (float)(choices.size() - 1)) + (CharSprite::charTextureSize + 15) * textScale * i)
		};
		newText = std::make_unique<TextCreator>(choices[i], pos, textScale);
		choiceTextSprites.push_back(std::move(newText));

		//Y座標を記憶しておく
		textPosYs.push_back(pos.y);
		//最大文字数チェック
		if (charNumMax < (int)choices[i].size()) {
			charNumMax = (int)choices[i].size();
		}
	}

	//背景スプライト生成
	backSprite = Sprite::Create("white", centerPos, { 0.5f, 0.5f }, { 0.3f, 0.3f, 0.3f, 0.8f });
	DirectX::XMFLOAT2 backSize = { (CharSprite::charTextureSize - 2) * textScale * charNumMax + 80, (CharSprite::charTextureSize + 15) * (float)(choices.size()) * textScale + 30 };
	//枠が小さすぎるとしょぼいので、最小値を設定
	backSize.x = max(backSize.x, 350);
	backSize.y = max(backSize.y, 150);
	backSprite->SetSize(backSize);

	//選択強調背景用スプライト生成
	choiceBackSprite = Sprite::Create("white", DirectX::XMFLOAT2{ centerPos.x, textPosYs[selectNum] }, { 0.5f, 0.5f }, { 1, 0, 0, 0.8f });
	DirectX::XMFLOAT2 choiceBackSize = { backSize.x - 60, CharSprite::charTextureSize * textScale + 4 };
	choiceBackSprite->SetSize(choiceBackSize);

	//各状態の内容をセット
	func.emplace_back([this] { return StartWaitUpdate(); });
	func.emplace_back([this] { return SelectChoiceUpdate(); });
	func.emplace_back([this] { return ConfirmSelectionUpdate(); });

	//タイマークラス
	timer = std::make_unique<Engine::Timer>();
}

QuestionSystem::~QuestionSystem()
{
}

void QuestionSystem::Update()
{
	//各状態の内容を更新
	func[int(phase)]();

	//スプライト更新
	backSprite->Update();
	choiceBackSprite->Update();
	for (auto& choiceTextSprite : choiceTextSprites) {
		choiceTextSprite->Update();
	}
}

void QuestionSystem::Draw()
{
	backSprite->Draw();
	//待機状態のときは選択強調背景スプライトを描画しない
	if (!(phase == QuestionPhase::START_WAIT)) {
		choiceBackSprite->Draw();
	}
	for (auto& choiceTextSprite : choiceTextSprites) {
		choiceTextSprite->Draw();
	}
}

void QuestionSystem::StartWaitUpdate()
{
	//ボタン連打による選択を防ぐために待機させる時間
	const float waitTime = 30;
	//タイマー更新
	timer->Update();

	//タイマーが指定した時間になったら次のフェーズへ
	if (*timer.get() >= waitTime) {
		phase = QuestionPhase::SELECT_CHOICE;

		//タイマーをリセットしておく
		timer->Reset();
	}
}

void QuestionSystem::SelectChoiceUpdate()
{
	//選択を変更する
	ChangeSelectChoices();
	//選択を決定する
	SelectChoice();
}

void QuestionSystem::ChangeSelectChoices()
{
	//入力がなければ抜ける
	if (!(DirectInput::GetInstance()->TriggerKey(DIK_UP) || DirectInput::GetInstance()->TriggerKey(DIK_DOWN) ||
		DirectInput::GetInstance()->TriggerKey(DIK_W) || DirectInput::GetInstance()->TriggerKey(DIK_S) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_UP) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_DOWN) || 
		XInputManager::GetInstance()->TriggerLeftStickY(true) || XInputManager::GetInstance()->TriggerLeftStickY(false))) {
		return;
	}

	//上入力で一つ上の選択肢を選択する
	if (DirectInput::GetInstance()->TriggerKey(DIK_UP) || DirectInput::GetInstance()->TriggerKey(DIK_W) || 
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_UP) || XInputManager::GetInstance()->TriggerLeftStickY(true)) {
		if (selectNum <= 0) { return; }
		selectNum--;
	}
	//下入力で一つ下の選択肢を選択する
	else if (DirectInput::GetInstance()->TriggerKey(DIK_DOWN) || DirectInput::GetInstance()->TriggerKey(DIK_S) || 
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_DOWN) || XInputManager::GetInstance()->TriggerLeftStickY(false)) {
		if (selectNum >= choicesNum - 1) { return; }
		selectNum++;
	}

	//選択強調背景スプライトの座標を更新
	choiceBackSprite->SetPosition({ centerPos.x, textPosYs[selectNum] });
}

void QuestionSystem::SelectChoice()
{
	//入力がなければ抜ける
	if (!(DirectInput::GetInstance()->TriggerKey(DIK_SPACE) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A))) { return; }

	//選択確定フェーズへの移行
	phase = QuestionPhase::CONFIRM_SELECTION;
}

void QuestionSystem::ConfirmSelectionUpdate()
{
	//確定した演出1回にかかる時間
	const float selectDirectionTime = 10;
	//タイマー更新
	timer->Update();

	//色の透明度をイージングで変更
	const float time = *timer.get() / selectDirectionTime;
	DirectX::XMFLOAT4 color = choiceBackSprite->GetColor();
	color.w = Easing::Lerp(0.8f, 0, time);
	choiceBackSprite->SetColor(color);

	//タイマーが指定した時間になったらカウントを更新
	if (*timer.get() >= selectDirectionTime) {
		selectDirectionCount++;

		//タイマーをリセットしておく
		timer->Reset();
	}

	//カウントが指定した回数に到達したら質問終了
	const int selectDirectionNum = 6;
	if (selectDirectionCount >= selectDirectionNum) {
		isEnd = true;
	}
}
