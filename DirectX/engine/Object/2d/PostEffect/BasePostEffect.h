#pragma once
#include "Object/2d/Sprite.h"

class BasePostEffect :public Sprite
{
public:

	enum class EffectTyep {
		bloom,
		outline,
		fog,
		size,
	};

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	BasePostEffect();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~BasePostEffect();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const EffectTyep _type, const std::string& _texName);

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	void Draw();

	static void SetPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { BasePostEffect::pipeline = _pipeline; }

private://�ÓI�����o�ϐ�

	//�p�C�v���C�����
	static std::vector<GraphicsPipelineManager::DrawSet> pipeline;

private://�����o�ϐ�
	
	//�G�t�F�N�g���
	EffectTyep type;

};