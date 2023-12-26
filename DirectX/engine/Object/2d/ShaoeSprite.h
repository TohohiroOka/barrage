#pragma once
#include "../ObjectBase.h"
#include "../Object/3d/Model.h"

class ShaoeSprite : public ObjectBase
{
private:

	/// <summary>
	/// ���_�f�[�^�\����
	/// </summary>
	struct SHAOE_VERTEX
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT2 uv;  // uv���W
	};

	/// <summary>
	/// �萔�o�b�t�@�p�f�[�^�\����
	/// </summary>
	struct CONST_BUFFER_DATA
	{
		XMFLOAT4 color;	// �F (RGBA)
		XMMATRIX mat;	// �R�c�ϊ��s��
	};

public: // �ÓI�����o�֐�

	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// �X�v���C�g����
	/// </summary>
	/// <param name="_name">�e�N�X�`���ۑ���</param>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<ShaoeSprite> Create(const std::string& _name, const XMFLOAT2& _position,const XMFLOAT4& _color = { 1,1,1,1 });

public: // �����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ShaoeSprite() {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ShaoeSprite();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_name">�e�N�X�`���ۑ���</param>
	void Initialize(const std::string& _name, const XMFLOAT2& _position, const XMFLOAT4& _color = { 1,1,1,1 });

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const DrawMode _drawMode = DrawMode::alpha);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const Texture* _tex, const DrawMode _drawMode = DrawMode::alpha);

private:

	//�p�C�v���C�����
	static std::vector<GraphicsPipelineManager::DrawSet> pipeline;

protected: // �����o�ϐ�

	// �ˉe�s��
	static XMMATRIX matProjection;

	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//�C���f�b�N�X��
	int indexNum;
	//�e�N�X�`����
	std::unique_ptr<TextureManager> texture;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// Z�����̉�]�p
	float rotation = 0.0f;
	// ���W
	XMFLOAT2 position = { 0, 0 };
	//�傫��
	XMFLOAT2 scale = { 1, 1 };
	// ���[���h�s��
	XMMATRIX matWorld{};
	// �F
	XMFLOAT4 color = { 1, 1, 1, 1 };

public:

	static void SetPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { ShaoeSprite::pipeline = _pipeline; }
	const XMFLOAT2& GetPosition() { return position; }
	const float GetRotation() { return rotation; }
	const XMFLOAT4& GetColor() { return color; }
	TextureManager* GetTexture() { return texture.get(); }
	void DeleteTexture() { texture->SetTexture(""); }
	void SetPosition(const XMFLOAT2& _position) { this->position = _position; }
	void SetRotation(float _rotation) { this->rotation = _rotation; }
	void SetScale(const XMFLOAT2& _scale) { this->scale = _scale; }
	void SetColor(const XMFLOAT4& _color) { this->color = _color; }
	void SetModel(Model* _model);
};

