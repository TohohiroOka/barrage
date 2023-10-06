#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>
#include <imgui.h>

class DirectXCommon
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private://�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// imgui������
	/// </summary>
	void InitImgui();

public://�ÓI�����o�֐�

	/// <summary>
	/// directXCommon����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<DirectXCommon> Create();

public://�����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <returns></returns>
	DirectXCommon() {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DirectXCommon();

	/// <summary>
	/// �[�x�o�b�t�@����
	/// </summary>
	void CreateDepth();

	/// <summary>
	/// �`��O�ݒ�
	/// </summary>
	void PreDraw();

	/// <summary>
	/// �`���ݒ�
	/// </summary>
	void PostDraw();

	/// <summary>
	/// �f�o�C�X��getter
	/// </summary>
	/// <returns>�f�o�C�X</returns>
	ID3D12Device* GetDevice() { return device.Get(); }

	/// <summary>
	/// �R�}���h���X�g��getter
	/// </summary>
	/// <returns>�R�}���h���X�g</returns>
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }

private://�����o�ϐ�

	//�f�o�C�X
	ComPtr<ID3D12Device> device;
	//�R�}���h���X�g
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	//�t�@�N�g���[
	ComPtr<IDXGIFactory6> dxgiFactory;
	//�R�}���h�A���P�[�^
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	//�R�}���h�L���[
	ComPtr<ID3D12CommandQueue> cmdQueue;
	//�X���b�v�`�F�[��
	ComPtr<IDXGISwapChain4> swapchain;
	//�o�b�N�o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	//�����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuffer;
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	//�[�x�r���[�p�̃f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	//�t�F���X
	ComPtr<ID3D12Fence> fence;
	//�R�}���h���X�g�����܂ł̃J�E���g
	UINT64 fenceVal = 0;
};