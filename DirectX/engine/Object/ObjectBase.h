#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "Pipeline/GraphicsPipelineManager.h"
#include "Texture/Texture.h"

class ObjectBase
{
protected:// �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

protected:

	enum class DrawMode {
		alpha,
		add,
	};

public:

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="_device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// �R�}���h���X�g�̃Z�b�g
	/// </summary>
	/// <param name="_cmdList">�R�}���h���X�g</param>
	static void SetCmdList(ID3D12GraphicsCommandList* _cmdList) { ObjectBase::cmdList = _cmdList; }

	/// <summary>
	/// �R�}���h���X�g�̃Z�b�g
	/// </summary>
	static void ReleaseCmdList() { ObjectBase::cmdList = nullptr; }

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw(const GraphicsPipelineManager::DrawSet& _drawSet);

protected:

	// �f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;

};