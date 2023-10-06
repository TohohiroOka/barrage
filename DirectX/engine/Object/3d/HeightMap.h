#pragma once
#include "Base3D.h"
#include "TerrainModel.h"

class Camera;
class LightGroup;

class HeightMap : public Base3D
{
private:// �T�u�N���X

	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct CONST_BUFFER_DATA_B0
	{
		XMFLOAT4 baseColor;//�x�[�X�J���[
		XMMATRIX viewproj; // �r���[�v���W�F�N�V�����s��
		XMMATRIX world; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
		unsigned int isBloom;//�u���[���̗L��
		unsigned int isToon;//�g�D�[���̗L��
		unsigned int isOutline;//�A�E�g���C���̗L��
		unsigned int isLight;//���C�e�B���O�̗L��
		float pad;
		XMFLOAT3 outlineColor;
		float pad1;
	};

	struct CONST_BUFFER_DATA_B1
	{
		XMFLOAT3 m_baseColor;//�x�[�X�J���[
		XMFLOAT3 m_ambient; //�A���r�G���g�W��
		XMFLOAT3 m_diffuse; //�f�B�t���[�Y�W��
		float m_metalness; //�����x
		float m_specular; //�X�y�L�����[�W��
		float m_roughness; //�e��
		float m_alpha; //�A���t�@
	};

public://�����o�֐�

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="_model">���f��</param>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<HeightMap> Create(TerrainModel* _model);

private://�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

public:

	HeightMap();
	~HeightMap();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const DrawMode _drawMode = DrawMode::alpha);

	/// <summary>
	/// ���f���̕ϊ�
	/// </summary>
	/// <param name="_beforeModel">�ύX�O</param>
	/// <param name="afterModel">�ύX��</param>
	/// <param name="_ratio">�ϊ�����</param>
	void SetChangeDrawModel(TerrainModel* _beforeModel, TerrainModel* afterModel, const float _ratio = 1.0f);

	/// <summary>
	/// ���f���̕ϊ�
	/// </summary>
	/// <param name="_beforeModel">�ύX�O</param>
	/// <param name="afterModel">�ύX��</param>
	/// <param name="_ratio">�ϊ�����</param>
	void SetChangeHitModel(TerrainModel* _beforeModel, TerrainModel* afterModel, const float _ratio = 1.0f);

private:

	//�p�C�v���C�����
	static std::vector<GraphicsPipelineManager::DrawSet> pipeline;

	// ���f��
	TerrainModel* model = nullptr;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffB1;
	//�x�[�X�J���[
	XMFLOAT4 baseColor = { 1,1,1,1 };

public:

	static void SetPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { HeightMap::pipeline = _pipeline; }
	void SetModel(TerrainModel* _model) { model = _model; }
	Model* GetModel() { return model->GetModel(); }
	std::vector<Mesh::VERTEX>& GetHitVertices() { return model->GetHitVertices(); }
	std::vector<unsigned long>& GetHitIndices() { return model->GetHitIndices(); }
};