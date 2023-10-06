#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "ShaderManager.h"
#include <unordered_map>

class GraphicsPipelineManager
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://�����oenum

	struct GRAPHICS_PIPELINE
	{
		// ���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	};

	struct PEPELINE {
		//shader��
		std::string name;
		//�g��shader�̎��
		std::vector<std::string> shaderType;
		//layout�ݒ�
		std::vector<std::string> inputLayoutType;
		//�����_�[�^�[�Q�b�g��
		int rtvNum;
		//�u�����h���[�h
		std::string blendMode;
		//3D/2D/Particle
		std::string drawMode;
		//�`����@
		std::string drawType;
		//�摜��
		int textureNum;
		//�萔�o�b�t�@��
		int rootparams;
	};

	enum class DrawType {
		POINT,
		LINE_LIST,
		LINE_STRIP,
		TRIANGLE_LIST,
		TRIANGLE_STRIP,
	};

	struct DrawSet {
		std::string pipeName;
		DrawType drawType;
		int constBuffNum;
		int texNum;
	};

private://�����o�֐�

	/// <summary>
	/// �u�����h���[�h�̐ݒ�
	/// </summary>
	/// <param name="_mode">�u�����h�̎��</param>
	/// <returns>�u�����h�ݒ�</returns>
	D3D12_RENDER_TARGET_BLEND_DESC CreateBlendDesc(const std::string& _mode);

	/// <summary>
	/// �p�C�v���C���f�X�N�̐���
	/// </summary>
	/// <param name="_pepeline">�p�C�v���C���ݒ�</param>
	/// <returns>�p�C�v���C���f�X�N</returns>
	D3D12_GRAPHICS_PIPELINE_STATE_DESC CreatepelineDesc(const PEPELINE& _pepeline);

	/// <summary>
	/// ���[�g�V�O�l�`���̐���
	/// </summary>
	/// <param name="_pepelineName">�p�C�v���C����</param>
	/// <param name="_pepeline">�p�C�v���C���ݒ�</param>
	void CreateRootSignature(const std::string& _pepelineName, const PEPELINE& _pepeline);

public://�����o�֐�

	GraphicsPipelineManager();
	~GraphicsPipelineManager();

	/// <summary>
	/// �f�o�C�X�̃Z�b�g
	/// </summary>
	/// <param name="_device">�f�o�C�X</param>
	static void SetDevice(ID3D12Device* _device) { GraphicsPipelineManager::device = _device; }

	/// <summary>
	/// �p�C�v���C���̐���
	/// enum OBJECT_KINDS�̒������������Ƃ���
	/// </summary>
	/// <param name="_name">�p�C�v���C����</param>
	/// <param name="_pepeline">�p�C�v���C���ݒ�</param>
	/// <returns>�p�C�v���C�����</returns>
	void CreatePipeline(const std::string& _name, const PEPELINE& _pepeline, GraphicsPipelineManager::DrawSet& _drawSet);

	/// <summary>
	/// �`��O����
	/// </summary>
	static void PreDraw(ID3D12GraphicsCommandList* _cmdList, const DrawSet& _drawSet);

	/// <summary>
	/// �t���[��������
	/// </summary>
	static void ResetDrawSet();

private://�ÓI�����o�ϐ�

	// �f�o�C�X
	static ID3D12Device* device;
	//�p�C�v���C���ۑ��z��
	static std::unordered_map<std::string, GRAPHICS_PIPELINE> graphicsPipeline;
	//�ЂƂO�̃p�C�v���C���ݒ�
	static DrawSet oldDraw;

private://�����o�ϐ�

	//�V�F�[�_�[
	std::unique_ptr<ShaderManager> shaderManager;
};