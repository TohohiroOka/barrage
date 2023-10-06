#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "GraphicsPipelineManager.h"
#include "Texture.h"

class Camera;

class CubeMap
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//���_�f�[�^3D
	struct VERTEX {
		XMFLOAT3 pos;
		XMFLOAT3 uv;
	};

	//�萔�o�b�t�@�̍\����
	struct CONST_BUFFER_DATA {
		XMMATRIX viewproj; // �r���[�v���W�F�N�V�����s��
		XMMATRIX matWorld; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
	};

public://�����o�֐�

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="_device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_cmdList">�R�}���h���X�g</param>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<CubeMap> Create(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// �J�����̃Z�b�g
	/// </summary>
	/// <param name="_camera">�J����</param>
	static void SetCamera(Camera* _camera) {
		CubeMap::camera = _camera;
	}

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="_cmdList">�`��R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// �������
	/// </summary>
	static void Finalize();

private://�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

public:

	CubeMap() {};
	~CubeMap();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �L���[�u�}�b�v�̓]��
	/// </summary>
	/// <param name="_cmdList">�`��R�}���h���X�g</param>
	/// <param name="_rootParameterIndex"></param>
	void TransferTextureBubber(ID3D12GraphicsCommandList* _cmdList, const UINT& _rootParameterIndex);

	/// <summary>
	/// �L���[�u�}�b�v�̃Z�b�g
	/// </summary>
	/// <returns></returns>
	Texture* SetTexture() { return texture.get(); }

	/// <summary>
	/// �p�C�v���C���̃Z�b�g
	/// </summary>
	/// <param name="_pipeline">�p�C�v���C��</param>
	static void SetPipeline(const GraphicsPipelineManager::GRAPHICS_PIPELINE& _pipeline) { pipeline = _pipeline; }

private:

	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�p�C�v���C��
	static GraphicsPipelineManager::GRAPHICS_PIPELINE pipeline;
	//�J����
	static Camera* camera;
	//�C���f�b�N�X��
	static const int indexNum = 36;

private:

	//�e�N�X�`�����
	std::unique_ptr<Texture> texture = nullptr;
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//�L���[�u�}�b�v�o�b�t�@
	ComPtr<ID3D12Resource> texConstBuffer;
	//���\�[�X�z��
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���W
	XMFLOAT3 position = { 0,400,0 };
	//�傫��
	XMFLOAT3 scale = { 500,500,500 };
	//��]
	XMFLOAT3 rotation = {};
};