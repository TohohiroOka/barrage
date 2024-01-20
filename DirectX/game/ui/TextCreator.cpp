#include "TextCreator.h"
#include "CharActionInputSprite.h"

TextCreator::TextCreator(const std::wstring& text, const DirectX::XMFLOAT2& leftTopPos, float scale, bool isStartDraw)
{
	//改行用カウント
	DirectX::XMFLOAT2 pos = leftTopPos;

	//コマンドモード名
	enum class CommandMode
	{
		NormalMode,
		ButtonMode,
		NumberMode,
	};
	//コマンド用
	std::wstring command;
	CommandMode commandMode = CommandMode::NormalMode;


	//テキストの文字数の分回す
	for (int i = 0; i < text.size(); i++) {
		//テキストから一文字ずつ取り出す
		const std::wstring character = text.substr(i, 1);

		//空白は飛ばす
		if (character == L"　") {
			pos.x += CharSprite::charTextureSize * scale;
			continue; 
		}

		//「＠」が検出されたら改行
		if (character == L"＠") {
			pos.x = leftTopPos.x;
			pos.y += (CharSprite::charTextureSize + 12) * scale;
			continue;
		}

		//ボタンスプライト状態のとき
		if (commandMode == CommandMode::ButtonMode) {
			//「]」がでてくるまで文字をコマンドとして保存
			if (character != L"]") {
				command.push_back(*character.c_str());
				continue;
			}
			else {
				//ボタンスプライト生成
				GameInputManager::InputAction inputAction = ActionInputCommand(command);
				std::unique_ptr<CharActionInputSprite> newInputSprite = std::make_unique<CharActionInputSprite>(inputAction, pos, scale, isStartDraw);
				//次の文字を横にずらす
				pos.x += newInputSprite->GetSprite()->GetSize().x;
				charSprites.push_back(std::move(newInputSprite));

				//コマンドリセット
				command.clear();

				//ボタンスプライト状態終了
				commandMode = CommandMode::NormalMode;
			}
		}		
		//数字スプライト状態のとき
		else if (commandMode == CommandMode::NumberMode) {
			//「)」がでてくると数字スプライト状態終了
			if (character == L")") {
				//数字挿入終了
				numberTexts.back()->InsertEnd();

				commandMode = CommandMode::NormalMode;
				continue;
			}

			//文字スプライト生成
			std::unique_ptr<CharSprite> newCharSprite = std::make_unique<CharSprite>(character, pos, scale, isStartDraw);

			//次の文字を横にずらす
			pos.x += newCharSprite->GetSprite()->GetSize().x;
			charSprites.push_back(std::move(newCharSprite));

			//生成した文字スプライトの要素を数字テキストに貸し出す
			numberTexts.back()->InsertCharSprite(charSprites.back().get());
		}
		//通常状態のとき
		else {
			//「[」←小文字が検出されたらボタンスプライト状態に変更
			if (character == L"[") {
				commandMode = CommandMode::ButtonMode;
				continue;
			}
			//「(」←小文字が検出されたら数字スプライト状態に変更
			if (character == L"(") {
				commandMode = CommandMode::NumberMode;

				//数字テキスト生成
				std::unique_ptr<NumberText> newNumberText = std::make_unique<NumberText>();
				numberTexts.push_back(std::move(newNumberText));

				continue;
			}

			//文字スプライト生成
			std::unique_ptr<CharSprite> newCharSprite = std::make_unique<CharSprite>(character, pos, scale, isStartDraw);
			//次の文字を横にずらす
			pos.x += newCharSprite->GetSprite()->GetSize().x;
			charSprites.push_back(std::move(newCharSprite));
		}
	}
}

TextCreator::~TextCreator()
{
}

void TextCreator::Update()
{
	for (auto& charSprite : charSprites) {
		charSprite->Update();
	}
}

void TextCreator::Draw()
{
	for (auto& charSprite : charSprites) {
		charSprite->Draw();
	}
}

GameInputManager::InputAction TextCreator::ActionInputCommand(const std::wstring& command)
{
	//全コマンド共通部分を抜き出し
	std::wstring commandCommon = L"action_input_";

	//コマンドに書かれている行動を返す
	if (command == commandCommon + L"move_forward") {
		return GameInputManager::InputAction::MoveForward;
	}
	if (command == commandCommon + L"move_back") {
		return GameInputManager::InputAction::MoveBack;
	}
	if (command == commandCommon + L"move_left") {
		return GameInputManager::InputAction::MoveForward;
	}
	if (command == commandCommon + L"move_right") {
		return GameInputManager::InputAction::MoveForward;
	}
	if (command == commandCommon + L"avoid") {
		return GameInputManager::InputAction::Avoid_Blink_Dash;
	}
	if (command == commandCommon + L"blink") {
		return GameInputManager::InputAction::Avoid_Blink_Dash;
	}
	if (command == commandCommon + L"dash") {
		return GameInputManager::InputAction::Avoid_Blink_Dash;
	}
	if (command == commandCommon + L"jump") {
		return GameInputManager::InputAction::Jump;
	}
	if (command == commandCommon + L"light_attack") {
		return GameInputManager::InputAction::LightAttack;
	}
	if (command == commandCommon + L"strong_attack") {
		return GameInputManager::InputAction::StrongAttack;
	}
	if (command == commandCommon + L"camera_up_rota") {
		return GameInputManager::InputAction::CameraUpRota;
	}
	if (command == commandCommon + L"camera_down_rota") {
		return GameInputManager::InputAction::CameraDownRota;
	}
	if (command == commandCommon + L"camera_left_rota") {
		return GameInputManager::InputAction::CameraLeftRota;
	}
	if (command == commandCommon + L"camera_right_rota") {
		return GameInputManager::InputAction::CameraRightRota;
	}
	if (command == commandCommon + L"camera_lockon_rota") {
		return GameInputManager::InputAction::Lockon;
	}
	if (command == commandCommon + L"select") {
		return GameInputManager::InputAction::Select;
	}

	assert(0);
	return GameInputManager::InputAction::InputActionNum;
}
