#pragma once
#include "CharSprite.h"
#include "system/GameInputManager.h"

/// <summary>
/// �e�L�X�g����V�X�e��
/// </summary>
class TextCreator
{
public: //�����o�֐�
	TextCreator(const std::wstring& text, const DirectX::XMFLOAT2& leftTopPos, float scale, bool isStartDraw = true);
	~TextCreator();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//getter
	int GetTextLength() { return (int)charSprites.size(); }
	CharSprite* GetCharSprite(int num) { return charSprites[num].get(); }

private: //�����o�֐�
	/// <summary>
	/// �s�����̓R�}���h�����
	/// </summary>
	/// <param name="command">�R�}���h</param>
	/// <returns>�s������</returns>
	GameInputManager::InputAction ActionInputCommand(const std::wstring& command);

private: //�����o�ϐ�
	//�����X�v���C�g
	std::vector<std::unique_ptr<CharSprite>> charSprites;
};