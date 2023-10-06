#pragma once
#include "Base3D.h"

class BaseCollider;
class Camera;
class LightGroup;

class PrimitiveObject3D : public Base3D
{
private://�\���̐錾

	//�萔�o�b�t�@�̍\����
	struct CONST_BUFFER_DATA {
		XMFLOAT4 color;//�F
		XMMATRIX matWorld;//3D�ϊ��s��
		XMMATRIX viewproj;//3D�ϊ��s��
	};

public:

	/// <summary>
	/// ����
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<PrimitiveObject3D> Create();

public://�����o�֐�


	PrimitiveObject3D() {};
	~PrimitiveObject3D();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;


	/// <summary>
	/// //�`��
	/// </summary>
	void Draw(const DrawMode _drawMode = DrawMode::alpha);

	/// <summary>
	/// ���_���̍Đݒ�
	/// </summary>
	void VertexInit();

	/// <summary>
	/// 
	/// </summary>
	void SetWorld(const XMMATRIX& _world) {
		matWorld =_world;
	}

protected://�����o�ϐ�

	//�p�C�v���C�����
	static std::vector<GraphicsPipelineManager::DrawSet> pipeline;

	// ���_�z��
	std::vector<XMFLOAT3> vertices;
	//�C���f�b�N�X�z��
	std::vector<unsigned long> indices;
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0;
	//�x�[�X�J���[
	XMFLOAT4 baseColor = { 1,1,1,1 };

public:

	/// <summary>
	/// ���_�̃Z�b�g
	/// </summary>
	/// <param name="_vertex">���_</param>
	void SetVertex(const XMFLOAT3& _vertex) { this->vertices.push_back(_vertex); }

	/// <summary>
	/// ���_�̃��Z�b�g
	/// </summary>
	void ResetVertex() { 
		this->vertices.clear();
		this->vertices.resize(0);
	}

	/// <summary>
	/// ���_�̐�
	/// </summary>
	int VertexNum() { return int(vertices.size()); }

	static void SetPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { PrimitiveObject3D::pipeline = _pipeline; }
	void SetColor(const XMFLOAT4& _color) { baseColor = _color; }
};