#pragma once
#include "Object/ObjectBase.h"

class Sprite : public ObjectBase
{
public: // �T�u�N���X

	/// <summary>
	/// ���_�f�[�^�\����
	/// </summary>
	struct VERTEX
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT2 uv;  // uv���W
	};

private:

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
	static std::unique_ptr<Sprite> Create(const std::string& _name, const XMFLOAT2& _position, const XMFLOAT2& _anchorpoint = { 0, 0 }, const XMFLOAT4& _color = { 1,1,1,1 }, bool _isFlipX = false, bool _isFlipY = false);

public: // �����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Sprite() {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Sprite();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_name">�e�N�X�`���ۑ���</param>
	/// <param name="_anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="_isFlipX">���E���]���邩</param>
	/// <param name="_isFlipY">�㉺���]���邩</param>
	void Initialize(const std::string& _name, const XMFLOAT2& _position, const XMFLOAT2& _anchorpoint = { 0, 0 }, const XMFLOAT4& _color = { 1,1,1,1 }, bool _isFlipX = false, bool _isFlipY = false);

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

	// ���_��
	static const int vertNum = 4;
	// �ˉe�s��
	static XMMATRIX matProjection;

	//�e�N�X�`����
	std::unique_ptr<TextureManager> texture;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// Z�����̉�]�p
	float rotation = 0.0f;
	// ���W
	XMFLOAT2 position{ 0, 0 };
	// �X�v���C�g���A����
	XMFLOAT2 size = { 100.0f, 100.0f };
	// �A���J�[�|�C���g
	XMFLOAT2 anchorpoint = { 0, 0 };
	// ���[���h�s��
	XMMATRIX matWorld{};
	// �F
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// ���E���]
	bool isFlipX = false;
	// �㉺���]
	bool isFlipY = false;
	// �e�N�X�`���n�_
	XMFLOAT2 texLeftTop = { 0, 0 };
	// �e�N�X�`�����A����
	XMFLOAT2 texSize = { 500.0f, 500.0f };

protected: // �����o�֐�

	/// <summary>
	/// ���_�f�[�^�]��
	/// </summary>
	void TransferVertices();

	/// <summary>
	/// ���_�f�[�^�]��
	/// </summary>
	void TransferVerticesNoTex();

public:

	static void SetPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { Sprite::pipeline = _pipeline; }
	const XMFLOAT2& GetPosition() { return position; }
	const float GetRotation() { return rotation; }
	const XMFLOAT2& GetSize() { return size; }
	const XMFLOAT2& GetTexSize() { return texSize; }
	const XMFLOAT2& GetTexLeftTop() { return texLeftTop; }
	const XMFLOAT4& GetColor() { return color; }
	const XMFLOAT2& GetAnchorpoint() { return anchorpoint; }
	bool GetIsFlipX() { return isFlipX; }
	bool GetIsFlipY() { return isFlipY; }
	TextureManager* GetTexture() { return texture.get(); }
	void SetTexture(const std::string& _name, bool isChangeSize = true) {
		texture->SetTexture(_name);
		if (!isChangeSize) { return; }
		XMFLOAT2 _size = texture->GetTexSize();
		size = { _size.x, _size.y };
		texSize = { _size.x,_size.y };
	};
	void DeleteTexture() { texture->SetTexture(""); }
	void SetPosition(const XMFLOAT2& _position) { this->position = _position; }
	void SetRotation(float _rotation) { this->rotation = _rotation; }
	void SetSize(const XMFLOAT2& _size) { this->size = _size; }
	void SetTexSize(const XMFLOAT2& _texSize) { this->texSize = _texSize; }
	void SetTexLeftTop(const XMFLOAT2& _texLeftTop) { this->texLeftTop = _texLeftTop; }
	void SetColor(const XMFLOAT4& _color) { this->color = _color; }
	void SetAnchorpoint(const XMFLOAT2& _anchorpoint) { this->anchorpoint = _anchorpoint; }
	void SetIsFlipX(bool _isFlipX) { this->isFlipX = _isFlipX; };
	void SetIsFlipY(bool _isFlipY) { this->isFlipY = _isFlipY; };
};