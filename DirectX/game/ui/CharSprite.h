#pragma once
#include "Object/2d/Sprite.h"

/// <summary>
/// �����X�v���C�g
/// </summary>
class CharSprite
{
public: //�����o�֐�
	CharSprite(const std::wstring& character, const DirectX::XMFLOAT2& pos, float scale, bool isDraw = true);
	~CharSprite();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �����ύX
	/// </summary>
	/// <param name="character">����</param>
	void ChangeCharacter(const std::wstring& character);

	//getter
	Sprite* GetSprite() { return charSprite.get(); }
	bool GetIsDraw() { return isDraw; }
	const std::wstring& GetCharacter() { return character; }

	//setter
	void SetIsDraw(bool isDraw) { this->isDraw = isDraw; }

private: //�����o�֐�
	/// <summary>
	/// �����e�L�X�g�e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="character">����</param>
	void LoadCharTexture(const std::string& character);

	/// <summary>
	/// wstring�^�̕������string�^�ɕϊ�
	/// </summary>
	/// <param name="oWString">�ϊ��Ώۂ̕�����</param>
	/// <returns></returns>
	std::string WStringToString(const std::wstring& oWString);

public: //�ÓI�����o�ϐ�(�擾���₷���悤��public)
	//�����e�N�X�`���̑傫��
	static const float charTextureSize;

protected: //�����o�ϐ�
	//�����X�v���C�g
	std::unique_ptr<Sprite> charSprite;
	//����
	std::wstring character;
	//�傫��
	float scale;
	//�`�悷�邩
	bool isDraw = true;
};