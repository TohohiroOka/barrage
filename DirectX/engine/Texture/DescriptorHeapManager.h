#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <array>

class DescriptorHeapManager
{
private:
	//�f�X�N���v�^�̑傫��
	static const int DescriptorsSRVSize = 1024;

	struct DescriptorsSRV {
		//�f�X�N���v�^�q�[�v
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
		//�f�X�N���v�^�e�[�u���̐���
		std::array<bool, DescriptorsSRVSize> TableManager;
	};

	//�f�X�N���v�^�̑傫��
	static const int DescriptorsRTVSize = 32;

	struct DescriptorsRTV {
		//�f�X�N���v�^�q�[�v
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
		//�f�X�N���v�^�e�[�u���̐���
		std::array<bool, DescriptorsRTVSize> TableManager;
	};
	//�f�X�N���v�^�̑傫��
	static const int DescriptorsDSVSize = 32;

	struct DescriptorsDSV {
		//�f�X�N���v�^�q�[�v
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
		//�f�X�N���v�^�e�[�u���̐���
		std::array<bool, DescriptorsDSVSize> TableManager;
	};

	enum class DescriptorsType {
		srv,
		rtv,
		dsv,
		size,
	};

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="_cmdList">�R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// �������
	/// </summary>
	static void Finalize();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	DescriptorHeapManager() {};

	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~DescriptorHeapManager();

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[�̍쐬
	/// </summary>
	/// <param name="_texBuffer">�e�N�X�`���o�b�t�@</param>
	/// <param name="_srvDesc">�V�F�[�_�[���\�[�X�r���[�ݒ�</param>
	void CreateSRV(Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer, D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc);

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[�̍쐬
	/// </summary>
	/// <param name="_texBuffer">�e�N�X�`���o�b�t�@</param>
	void CreateRTV(Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer);

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[�̍쐬
	/// </summary>
	/// <param name="_texBuffer">�e�N�X�`���o�b�t�@</param>
	void CreateDSV(Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer);

	static ID3D12DescriptorHeap* GetDescriptorHeap() {
		return descHeapSRV.descHeap.Get();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvHandle() {
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV.descHeap->GetCPUDescriptorHandleForHeapStart(), heapNumberRTV,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() {
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapDSV.descHeap->GetCPUDescriptorHandleForHeapStart(), heapNumberDSV,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV));
	}

private:

	//�f�o�C�X
	static ID3D12Device* device;
	//�f�X�N���v�^�q�[�v�i�摜�j
	static DescriptorsSRV descHeapSRV;
	//�f�X�N���v�^�q�[�v�i�����_�[�^�[�Q�b�g�j
	static DescriptorsRTV descHeapRTV;
	//�f�X�N���v�^�q�[�v�i�[�x�j
	static DescriptorsDSV descHeapDSV;

public:

	int heapNumberSRV = -1;
	int heapNumberRTV = -1;
	int heapNumberDSV = -1;
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpu;
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpu;
};