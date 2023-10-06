#pragma once
#include "Object/2d/Sprite.h"

class BasePostEffect :public Sprite
{
private:

	struct CONST_BUFFER_DATA_OUTLINE {
		float outlineWidth;//�A�E�g���C���̑���
		XMFLOAT4 pad;
	};

public:

	enum class EffectTyep {
		bloom,
		outline,
		fog,
		size,
	};

protected:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const EffectTyep _type);

	/// <summary>
	/// �[�x�o�b�t�@����
	/// </summary>
	void CreateDepthBuffer();

public://�����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	BasePostEffect();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~BasePostEffect();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	void Draw(const Texture* _tex);

	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDrawScene();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	void PostDrawScene();

	Texture* GetTex() { return texture.get(); }

private://�ÓI�����o�ϐ�

	//��ʃN���A�J���[
	static const float clearColor[4];
	//�p�C�v���C�����
	static std::vector<GraphicsPipelineManager::DrawSet> pipeline;

private://�����o�ϐ�

	//�e�N�X�`�����
	std::unique_ptr<Texture> texture;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuffer;
	//�G�t�F�N�g���
	EffectTyep type;

public:
	static void SetPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { BasePostEffect::pipeline = _pipeline; }

};