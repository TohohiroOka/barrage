#pragma once
#include "Model.h"
#include <DirectXMath.h>

class TerrainModel
{
private: // �G�C���A�X

	// DirectX::���ȗ�
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:

	struct HEIGHT_MAP_INFO {
		int terrainWidth;//��
		int terrainHeight;//����
		std::vector<XMFLOAT3> heightMap;//�n�`���
	};

	enum TEXTURE {
		HEIGHT_MAP_TEX,
		GRAPHIC_TEX_1,
		GRAPHIC_TEX_2,
		SIZE,
	};

public:

	enum class FACE_DIRECTION {
		X_PLUS,
		X_MINUS,
		Y_PLUS,//�m�[�}��
		Y_MINUS,//������
		Z_PLUS,
		Z_MINUS,
		SIZE,
	};

public:

	TerrainModel() {};
	~TerrainModel() {};

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="_heightmapFilename">heightmap��</param>
	/// <param name="_heightRate">��������</param>
	/// <param name="_pos">���[���h���W</param>
	/// <param name="_rota">��]</param>
	/// <param name="_scale">�X�P�[��</param>
	/// <param name="_filename">�e�N�X�`����1</param>
	/// <param name="_filename2">�e�N�X�`����2</param>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<TerrainModel> Create(const std::string& _heightmapFilename, const float _heightRate,
		const XMFLOAT3& _pos, const XMFLOAT3& _rota, const float _scale,
		const std::string& _filename1 = "null", const std::string& _filename2 = "null");

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="_cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* _cmdList);

private:

	/// <summary>
	/// �n�C�g�}�b�v
	/// </summary>
	/// <param name="_direction">����</param>
	/// <param name="_heightRate">��������</param>
	/// <param name="_pos">���[���h���W</param>
	/// <param name="_scale">�X�P�[��</param>
	/// <param name="_filename">�t�@�C����</param>
	/// <returns>������</returns>
	bool HeightMapLoad(const float _heightRate, const XMFLOAT3& _pos, const XMFLOAT3& _rota,
		const float _scale, const std::string& _filename);

	/// <summary
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="_filename1">�t�@�C����1</param>
	/// <param name="_filename2">�t�@�C����2</param>
	void LoadTexture(const std::string& _filename1, const std::string& _filename2);

	/// <summary>
	/// ���f���̐���
	/// </summary>
	/// <param name="_direction">����</param>
	void CreaetModel();

private:

	//��b�t�@�C��
	static const std::string baseDirectory;

private:

	//�n�C�g�}�b�v�̏��
	HEIGHT_MAP_INFO hmInfo;
	//�e�N�X�`�����
	std::array<std::unique_ptr<Texture>, TEXTURE::SIZE> texture;
	//���W�ړ����̋���
	XMFLOAT3 transformPos;
	// ���f��
	std::unique_ptr<Model> model = nullptr;
	//�C���f�b�N�X�̑傫��
	int indexNum = 0;
	//���_��
	int vertNum = 0;
	//�����蔻��p���_���
	std::vector<Mesh::VERTEX> hitVertices;
	std::vector<unsigned long> hitIndices;



	//���f���̌���
	FACE_DIRECTION direction;

public:

	Model* GetModel() { return model.get(); }
	std::vector<Mesh::VERTEX>& GetHitVertices() { return hitVertices; }
	std::vector<unsigned long>& GetHitIndices() { return hitIndices; }
	void SetMesh(Mesh* _mesh) { model->ResetASetMeshes(_mesh); }
	void SetHitVertices(const Mesh::VERTEX& inVertex, const int arrayNum) {
		hitVertices[arrayNum] = inVertex;
	}
};

