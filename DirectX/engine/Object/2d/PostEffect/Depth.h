#pragma once
#include "Object/2d/Sprite.h"

class Depth : public Sprite
{
public: //�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color;	//�F(RGBA)
		XMMATRIX mat;	//3�ϊ��s��
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// �V���h�E�}�b�v����
	/// </summary>
	/// <returns>�V���h�E�}�b�v</returns>
	static std::unique_ptr<Depth> Create();

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	void PreDrawScene();

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	void PostDrawScene();

	//getter
	Texture* GetTex() { return texture.get(); }

private: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�e�N�X�`�����
	std::unique_ptr<Texture> texture;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

};

