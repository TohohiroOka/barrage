#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

class ComputeShaderManager
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private://�ÓI�����o�ϐ�

	// �f�o�C�X
	static ID3D12Device* device;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�R���s���[�g�V�F�[�_�[�I�u�W�F�N�g
	static ComPtr<ID3DBlob> computeShader;
	// �v�Z�p���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootSignature;
	// �v�Z�p�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelineState;
	//�f�X�N���v�^�q�[�v�̐���
	static ComPtr<ID3D12DescriptorHeap> Heap;
	//�f�X�N���v�^�q�[�v�̐���
	//static ComPtr<ID3D12DescriptorHeap> srvUavHeap;
	//�q�[�v��
	static const UINT FrameCount = 2;
	//���Ƃ���
	static const UINT ThreadCount = 1;
	//�萔�o�b�t�@�̍ő�
	static const int IN_OUT_MAX = 300;

public:

	// �萔�o�b�t�@�p�\����
	struct InputData
	{
		XMFLOAT3 startPosition = {};//�������W
		XMFLOAT3 endPosition = {};//�I�����W
		XMFLOAT3 nowPosition = {};//���ݍ��W
		float time = {};//����
	};

	// �萔�o�b�t�@�p�\����
	struct OutputData
	{
		XMFLOAT3 nowPosition[IN_OUT_MAX] = {};
	};

private:

	/// <summary>
	/// �V�F�[�_�[�̓ǂݍ���
	/// </summary>
	static void LoadShader();

	/// <summary>
	/// �R���s���[�g�V�F�[�_�[�p�p�C�v���C���̐���
	/// </summary>
	static void CreatePipeline();

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̐���
	/// </summary>
	static void CreateDescriptor();

public://�ÓI�����o�֐�

	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<ComputeShaderManager> Create();

	/// <summary>
	///	���
	/// </summary>
	static void Finalize();

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ComputeShaderManager() {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ComputeShaderManager();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="max">�z��</param>
	/// <param name="startPosition">�X�^�[�g���W�z��</param>
	/// <param name="endPosition">�G���h���W�z��</param>
	/// <param name="nowPosition">���ݍ��W�z��</param>
	/// <param name="time">�C�[�W���O����</param>
	void ShaderUpdate(UINT max, XMFLOAT3* startPosition, XMFLOAT3* endPosition,
		XMFLOAT3* nowPosition, float* time);

	/// <summary>
	/// �v�Z���ʂ̎擾(cmdList���s��Ɏ擾�\)
	/// </summary>
	/// <returns></returns>
	XMFLOAT3* GetConstBufferNum();

	/// <summary>
	/// �X�V�O����
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void PreUpdate(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �X�V�㏈��
	/// </summary>
	void PostUpdate();

private://�����o�ϐ�

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> inputBuffer;
	//���o�̓f�[�^�̒��p�l
	InputData* data;
	//���݂̃f�[�^�T�C�Y
	int size = 0;
};