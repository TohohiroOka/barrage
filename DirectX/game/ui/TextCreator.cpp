#include "TextCreator.h"
#include "CharActionInputSprite.h"

TextCreator::TextCreator(const std::wstring& text, const DirectX::XMFLOAT2& leftTopPos, float scale, bool isStartDraw)
{
	//���s�p�J�E���g
	DirectX::XMFLOAT2 pos = leftTopPos;

	//�{�^���X�v���C�g�����p
	std::wstring command;
	bool isCommandMode = false;

	//�e�L�X�g�̕������̕���
	for (int i = 0; i < text.size(); i++) {
		//�e�L�X�g����ꕶ�������o��
		const std::wstring character = text.substr(i, 1);

		//�󔒂͔�΂�
		if (character == L"�@") { continue; }

		//�u���v�����o���ꂽ����s
		if (character == L"��") {
			pos.x = leftTopPos.x;
			pos.y += (CharSprite::charTextureSize + 10);
			continue;
		}

		//�{�^���X�v���C�g��Ԃ̂Ƃ�
		if (isCommandMode) {
			//�u]�v���łĂ���܂ŕ������R�}���h�Ƃ��ĕۑ�
			if (character != L"]") {
				command.push_back(*character.c_str());
				continue;
			}
			else {
				//�{�^���X�v���C�g����
				GameInputManager::InputAction inputAction = ActionInputCommand(command);
				std::unique_ptr<CharActionInputSprite> newInputSprite = std::make_unique<CharActionInputSprite>(inputAction, pos, scale, isStartDraw);
				//���̕��������ɂ��炷
				pos.x += newInputSprite->GetSprite()->GetSize().x;
				charSprites.push_back(std::move(newInputSprite));

				//�R�}���h���Z�b�g
				command.clear();

				//�{�^���X�v���C�g��ԏI��
				isCommandMode = false;
			}
		}
		//�ʏ��Ԃ̂Ƃ�
		else {
			//�u[�v�������������o���ꂽ��{�^���X�v���C�g��ԂɕύX
			if (character == L"[") {
				isCommandMode = true;
				continue;
			}

			//�����X�v���C�g����
			std::unique_ptr<CharSprite> newCharSprite = std::make_unique<CharSprite>(character, pos, scale, isStartDraw);
			//���̕��������ɂ��炷
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
	//�S�R�}���h���ʕ����𔲂��o��
	std::wstring commandCommon = L"action_input_";

	//�R�}���h�ɏ�����Ă���s����Ԃ�
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
