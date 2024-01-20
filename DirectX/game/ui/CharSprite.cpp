#include "CharSprite.h"

const float CharSprite::charTextureSize = 32.0f;

CharSprite::CharSprite(const std::wstring& character, const DirectX::XMFLOAT2& pos, float scale, bool isDraw)
{
	//文字を保存しておく
	this->character = character;
	//子クラスでL""を設定することがあるため抜ける
	if (character == L"") { return; }

	//wstring型をstring型に変換
	std::string str = WStringToString(character);

	//テクスチャ読み込み
	LoadCharTexture(str);

	//スプライト生成
	charSprite = Sprite::Create(str, pos, { 0.5f, 0.5f });
	charSprite->SetScale(scale);
	//大きさを保存
	this->scale = scale;

	//描画するかセット
	this->isDraw = isDraw;
}

CharSprite::~CharSprite()
{
}

void CharSprite::Update()
{
	//描画しない設定なら抜ける
	if (!isDraw) { return; }

	charSprite->Update();
}

void CharSprite::Draw()
{
	//描画しない設定なら抜ける
	if (!isDraw) { return; }

	charSprite->Draw();
}

void CharSprite::ChangeCharacter(const std::wstring& character)
{
	//文字を保存しておく
	this->character = character;
	//子クラスでL""を設定することがあるため抜ける
	if (character == L"") { return; }

	//wstring型をstring型に変換
	std::string str = WStringToString(character);

	//テクスチャ読み込み
	LoadCharTexture(str);

	//スプライト生成
	charSprite->SetTexture(str);
	charSprite->SetScale(scale);
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

	//バッファの取得
	CHAR* cpMultiByte = new CHAR[iBufferSize];

	//wstring → SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
		, iBufferSize, NULL, NULL);

	//stringの生成
	std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

	//バッファの破棄
	delete[] cpMultiByte;

	//変換結果を返す
	return oRet;
}
