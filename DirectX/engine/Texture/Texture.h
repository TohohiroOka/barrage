#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "DescriptorHeapManager.h"

class Texture
{
public:

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <param name="_fileName">�t�@�C����</param>
	/// <param name="_cmdList">dds�t�@�C������cmdList�������Ă���</param>
	static std::unique_ptr<Texture> Create(const std::string& _fileName, ID3D12GraphicsCommandList* _cmdList = nullptr);

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Texture() {};

	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Texture();

	/// <summary>
	/// �e�N�X�`���̓ǂݍ���
	/// </summary>
	/// <param name="_fileName">�摜�t�@�C���̖��O</param
	void LoadTexture(const std::string& _fileName);

	/// <summary>
	/// dds�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="_fileName">�摜�t�@�C���̖��O</param>
	/// <param name="_cmdList">�R�}���h���X�g</param>
	void LoadTextureFromDDSFile(const std::string& _fileName, ID3D12GraphicsCommandList* _cmdList);

private:
	
	//�f�o�C�X
	static ID3D12Device* device;

public:
	//�e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuffer = nullptr;
	//�e�N�X�`���̃f�X�N���v�^
	std::unique_ptr<DescriptorHeapManager> descriptor = nullptr;
	//dds�݂̂Ɏg�p����
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	Microsoft::WRL::ComPtr<ID3D12Resource> texConstBuffer = nullptr;

};

