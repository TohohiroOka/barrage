#include "CharSprite.h"

const float CharSprite::charTextureSize = 32.0f;

CharSprite::CharSprite(const std::wstring& character, const DirectX::XMFLOAT2& pos, float scale, bool isDraw)
{
	//������ۑ����Ă���
	this->character = character;
	//wstring�^��string�^�ɕϊ�
	std::string str = WStringToString(character);

	//�e�N�X�`���ǂݍ���
	LoadCharTexture(str);

	//�X�v���C�g����
	charSprite = Sprite::Create(str, pos, { 0.5f, 0.5f });
	charSprite->SetScale(scale);

	//�`�悷�邩�Z�b�g
	this->isDraw = isDraw;
}

CharSprite::~CharSprite()
{
}

void CharSprite::Update()
{
	if (!isDraw) { return; }

	charSprite->Update();
}

void CharSprite::Draw()
{
	if (!isDraw) { return; }

	charSprite->Draw();
}

void CharSprite::LoadCharTexture(const std::string& character)
{
	std::string directoryPass = "Resources/SpriteTexture/character/";
	TextureManager::LoadTexture(character, directoryPass + character + ".png");
}

std::string CharSprite::WStringToString(const std::wstring& oWString)
{
	int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char*)NULL, 0, NULL, NULL);

	//�o�b�t�@�̎擾
	CHAR* cpMultiByte = new CHAR[iBufferSize];

	//wstring �� SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
		, iBufferSize, NULL, NULL);

	//string�̐���
	std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

	//�o�b�t�@�̔j��
	delete[] cpMultiByte;

	//�ϊ����ʂ�Ԃ�
	return oRet;
}
