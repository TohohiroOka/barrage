#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "GraphicsPipelineManager.h"

class Camera;

class DrawLine3D
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�\���̐錾
	//�萔�o�b�t�@�̍\����
	struct CONST_BUFFER_DATA {
		XMFLOAT4 color;//�F
		XMMATRIX matWorld;//3D�ϊ��s��
		XMMATRIX viewproj;//3D�ϊ��s��
	};

private://�ÓI�����o�֐�

	/// <summary>
	/// �p�x�����߂�
	/// </summary>
	/// <param name="_startPoint">�n�_</param>
	/// <param name="_endPoint">�I�_</param>
	/// <returns>��_�Ԃ̊p�x</returns>
	float GetAngle(const XMFLOAT3& _startPoint, const XMFLOAT3& _endPoint);

public://�ÓI�����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <returns></returns>
	DrawLine3D() {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DrawLine3D();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// Object�̐���
	/// </summary>
	/// <param name="_lineNum">���̖{��</param>
	static std::unique_ptr<DrawLine3D> Create(int _lineNum);

	/// <summary>
	/// �J�����̃Z�b�g
	/// </summary>
	/// <param name="_camera">�J����</param>
	static void SetCamera(Camera* _camera) {
		DrawLine3D::camera = _camera;
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
	/// �p�C�v���C���̐ݒ�
	/// </summary>
	/// <param name="_pipeline"></param>
	static void SetPipeline(const GraphicsPipelineManager::GRAPHICS_PIPELINE& _pipeline) { DrawLine3D::pipeline = _pipeline; }

public://�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int _lineNum);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// ���̏��Z�b�g
	/// </summary>
	/// <param name="_startPoint">�n�_</param>
	/// <param name="_endPoint">�I�_</param>
	/// <param name="_width">���̕�</param>
	void SetLine(XMFLOAT3 _startPoint[], XMFLOAT3 _endPoint[], float _width);

	/// <summary>
	/// �F�̕ύX
	/// </summary>
	void SetColor(const XMFLOAT4& _color) { this->color = _color; }

	/// <summary>
	/// //�`��
	/// </summary>
	void Draw();

private://�ÓI�����o�ϐ�

	// ��{���̒��_��
	static const int vertNum = 4;
	// ��{���̃C���f�b�N�X��
	static const int indexNum = 6;
	// ��{���̃C���f�b�NBasics
	unsigned short BASE_INDICES[indexNum] = {
	0,1,2,1,2,3,
	};
	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�J����
	static Camera* camera;
	//�p�C�v���C��
	static GraphicsPipelineManager::GRAPHICS_PIPELINE pipeline;

protected://�����o�ϐ�

	//���_�z��
	std::vector<XMFLOAT3> vertices;
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//���_�f�[�^�̗v�f��
	UINT vertexArrayNum = 0;
	//�C���f�b�N�X�f�[�^�̗v�f��
	UINT indexArrayNum = 0;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// �F
	XMFLOAT4 color = {};
};