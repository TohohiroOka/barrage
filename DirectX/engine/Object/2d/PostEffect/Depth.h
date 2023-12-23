#pragma once
#include "Object/2d/Sprite.h"

class Depth : public Sprite
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// �V���h�E�}�b�v����
	/// </summary>
	/// <returns>�V���h�E�}�b�v</returns>
	static std::unique_ptr<Depth> Create(const std::string& _texName);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const std::string& _texName);

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	void PreDrawScene();

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	void PostDrawScene();
};

