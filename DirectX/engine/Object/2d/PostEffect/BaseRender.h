#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "Texture/TextureManager.h"

class BaseRender
{
public: //�ÓI�����o�֐�

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="_device"></param>
	/// <param name="_cmdList"></param>
	static void StaticInitialize(ID3D12Device* _device,ID3D12GraphicsCommandList* _cmdList);
	
public: //�����o�֐�

	BaseRender(const std::string& _texName);
	virtual ~BaseRender();
	
	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	virtual void PreDrawScene();

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	virtual void PostDrawScene();

	TextureManager* GetTexture() { return texture.get(); }

private:

	/// <summary>
	/// �[�x�o�b�t�@����
	/// </summary>
	void CreateDepthBuffer();

private:

	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�������F
	static const float clearColor[4];

	//�����_�[�^�[�Q�b�g�p�摜
	std::unique_ptr<TextureManager> texture;

	//DSV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//�[�x�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;
};

