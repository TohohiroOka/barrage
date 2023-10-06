#pragma once
#include "Object/2d/Sprite.h"

class PostEffect : public Sprite
{
public:

	enum class TexType {
		normal,
		bloom,
		outline,
		size,
	};

private:

	/// <summary>
	/// �萔�o�b�t�@�p�f�[�^�\����
	/// </summary>
	struct CONST_BUFFER_DATA
	{
		float outline;
	};

public://�����o�֐�

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	static std::unique_ptr<PostEffect> Create();

	/// <summary>
	/// �������
	/// </summary>
	void Finalize();

public://�����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PostEffect();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �[�x�o�b�t�@����
	/// </summary>
	void CreateDepthBuffer();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const std::vector<Texture*> _tex);

	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDrawScene();
	
	/// <summary>
	/// �`��㏈��
	/// </summary>
	void PostDrawScene();

	Texture* GetTex(TexType _type) {
		return texture[int(_type)].get();
	}

private://�ÓI�����o�ϐ�

	//��ʃN���A�J���[
	static const float clearColor[4];
	//�p�C�v���C�����
	static std::vector<GraphicsPipelineManager::DrawSet> pipeline;

private://�����o�ϐ�

	//�e�N�X�`�����
	std::array<std::unique_ptr<Texture>, int(TexType::size)> texture;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;


public:
	static void SetPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { PostEffect::pipeline = _pipeline; }

};