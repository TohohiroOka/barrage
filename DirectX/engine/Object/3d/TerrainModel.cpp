#include "TerrainModel.h"
#include <DirectXTex.h>

using namespace DirectX;

const std::string TerrainModel::baseDirectory = "Resources/HeightMap/";

std::unique_ptr<TerrainModel> TerrainModel::Create(const std::string& _heightmapFilename, const float _heightRate,
	const XMFLOAT3& _pos, const XMFLOAT3& _rota, const float _scale, const std::string& _filename1, const std::string& _filename2)
{
	//�C���X�^���X�𐶐�
	TerrainModel* instance = new TerrainModel();
	if (instance == nullptr) {
		return nullptr;
	}

	//�n�`�p�e�N�X�`���ǂݍ���
	instance->HeightMapLoad(_heightRate, _pos, _rota, _scale, _heightmapFilename);

	//�e�N�X�`���ǂݍ���
	instance->LoadTexture(_filename1, _filename2);

	//���f������
	instance->CreaetModel();

	return std::unique_ptr<TerrainModel>(instance);
}

void TerrainModel::Draw(ID3D12GraphicsCommandList* _cmdList)
{
	model->VIDraw(_cmdList);

	//�e�N�X�`���]��
	_cmdList->SetGraphicsRootDescriptorTable(3, texture[TEXTURE::HEIGHT_MAP_TEX]->descriptor->gpu);
	_cmdList->SetGraphicsRootDescriptorTable(4, texture[TEXTURE::GRAPHIC_TEX_1]->descriptor->gpu);
	_cmdList->SetGraphicsRootDescriptorTable(5, texture[TEXTURE::GRAPHIC_TEX_2]->descriptor->gpu);

	//�`��R�}���h
	_cmdList->DrawIndexedInstanced(indexNum, 1, 0, 0, 0);
}

bool TerrainModel::HeightMapLoad(const float _heightRate,
	const XMFLOAT3& _pos, const XMFLOAT3& _rota, const float _scale, const std::string& _filename)
{
	//���O����
	std::string fname = baseDirectory + _filename;

	texture[TEXTURE::HEIGHT_MAP_TEX] = Texture::Create(fname);

	//height map���J��
	FILE* filePtr;
	int error = fopen_s(&filePtr, fname.c_str(), "rb");
	if (error != 0)
	{
		return false;
	}

	//�t�@�C���w�b�_�[�̓ǂݍ���
	BITMAPFILEHEADER bitmapFileHeader;
	size_t count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	//�r�b�g�}�b�v���w�b�_�[�̓ǂݍ���
	BITMAPINFOHEADER bitmapInfoHeader;
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	//�n�`�̏c������ۑ�
	hmInfo.terrainWidth = bitmapInfoHeader.biWidth;
	hmInfo.terrainHeight = bitmapInfoHeader.biHeight;

	//�r�b�g�}�b�v�摜�f�[�^�̃T�C�Y���v�Z
	int imageSize = hmInfo.terrainWidth * hmInfo.terrainHeight * 3;

	//�r�b�g�}�b�v�摜�f�[�^�Ƀ����������蓖��
	unsigned char* bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	//�r�b�g�}�b�v�f�[�^�̐擪�Ɉړ�
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	//�r�b�g�}�b�v�摜�f�[�^�̓ǂݍ���
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	//�t�@�C�������
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	//�ۑ��p�z����}�b�v�̑傫���ɕύX
	size_t resize = hmInfo.terrainWidth * hmInfo.terrainHeight;
	hmInfo.heightMap.resize(resize);

	//�ԗv�f�̂ݎg�p���邽�߂̕ϐ�
	int k = 0;

	//�n�`�����傤�ǂ悭���邽�߂Ɋ���l
	float heightFactor = 10.0f;

	//���[���h�s��ϊ�
	XMMATRIX matWorld = XMMatrixIdentity();
	XMMATRIX matScale = XMMatrixScaling(_scale, _scale, _scale);
	matWorld *= matScale;

	XMMATRIX matRot;//�p�x
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(_rota.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(_rota.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(_rota.y));
	matWorld *= matRot;

	XMMATRIX matTrans;//���s����
	matTrans = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
	matWorld *= matTrans;

	// �摜�f�[�^�̓ǂݍ���
	for (int z = 0; z < hmInfo.terrainHeight; z++)
	{
		for (int x = 0; x < hmInfo.terrainWidth; x++)
		{
			float height = static_cast<float>(bitmapImage[k]);
			if (height < 15.0f) { height = 0.0f; }
			height = height / _heightRate;
			//if(height<)
			int index = (hmInfo.terrainHeight * z) + x;

			XMVECTOR pos = XMVector3Transform({ float(x),float(height) ,float(z) }, matWorld);

			hmInfo.heightMap[index].x += pos.m128_f32[0];
			hmInfo.heightMap[index].y += pos.m128_f32[1];
			hmInfo.heightMap[index].z += pos.m128_f32[2];


			k += 3;
		}
	}

	//�r�b�g�}�b�v�摜�f�[�^�̉��
	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}

void TerrainModel::LoadTexture(const std::string& _filename1, const std::string& _filename2)
{
	// �e�N�X�`������
	std::string filepath;
	if (_filename1 == "null") {
		filepath = "Resources/SubTexture/white1x1.png";
	}
	//�e�N�X�`���L��
	else
	{
		filepath = baseDirectory + _filename1;
	}

	texture[TEXTURE::GRAPHIC_TEX_1] = Texture::Create(filepath);

	// �e�N�X�`������
	if (_filename2 == "null") {
		filepath = "Resources/SubTexture/white1x1.png";
	}
	//�e�N�X�`���L��
	else
	{
		filepath = baseDirectory + _filename2;
	}

	texture[TEXTURE::GRAPHIC_TEX_2] = Texture::Create(filepath);
}

void TerrainModel::CreaetModel()
{
	int surfaceNum = hmInfo.terrainWidth * hmInfo.terrainHeight;
	vertNum = surfaceNum;
	indexNum = surfaceNum * 6;

	std::vector<Mesh::VERTEX> vertices;
	vertices.resize(vertNum);
	std::vector<unsigned long> indices;
	indices.resize(indexNum);

	unsigned long basicsIndices[6] = { 0 ,256 ,1 ,256 ,256 + 1 ,1 };

	//�}���C���f�b�N�X�ԍ�
	unsigned long index = 0;

	//�`��p���_���ۑ�
	{
		//�}���C���f�b�N�X�ԍ�
		unsigned long vertNum = 0;
		unsigned long indexNum = 0;

		//���_�ۑ�
		for (int y = 0; y < hmInfo.terrainHeight; ++y)
		{
			for (int x = 0; x < hmInfo.terrainWidth; ++x)
			{
				//���_
				vertices[vertNum].pos = hmInfo.heightMap[x + y * 256];
				vertices[vertNum].uv = XMFLOAT2(float(x) / 255.0f, float(y) / 255.0f);
				vertNum++;

				//�C���f�b�N�X
				if (y == 255 || x == 255) { continue; }
				const int dist = 256 * y;
				indices[index] = basicsIndices[0] + x + dist;
				index++;
				indices[index] = basicsIndices[1] + x + dist;
				index++;
				indices[index] = basicsIndices[2] + x + dist;
				index++;
				indices[index] = basicsIndices[3] + x + dist;
				index++;
				indices[index] = basicsIndices[4] + x + dist;
				index++;
				indices[index] = basicsIndices[5] + x + dist;
				index++;
			}
		}

		int normalNum = static_cast<int>(indices.size() / 3);
		for (int i = 0; i < normalNum; i++)
		{
			index = i * 3;
			unsigned long index1 = indices[index];
			index++;
			unsigned long index2 = indices[index];
			index++;
			unsigned long index3 = indices[index];

			XMVECTOR p0 = XMLoadFloat3(&vertices[index1].pos);
			XMVECTOR p1 = XMLoadFloat3(&vertices[index2].pos);
			XMVECTOR p2 = XMLoadFloat3(&vertices[index3].pos);

			XMVECTOR v1 = XMVectorSubtract(p0, p1);
			XMVECTOR v2 = XMVectorSubtract(p0, p2);

			XMVECTOR normal = XMVector3Cross(v1, v2);
			normal = XMVector3Normalize(normal);

			XMStoreFloat3(&vertices[index1].normal, normal);
			XMStoreFloat3(&vertices[index2].normal, normal);
			XMStoreFloat3(&vertices[index3].normal, normal);
		}
	}

	{
		////�����蔻��p���_���ۑ�
		//{
		//	int hitSurfaceNum = windthSize / 2 * heightSize / 2;
		//	int hitVertNum = hitSurfaceNum * 4;
		//	int hitIndexNum = hitSurfaceNum * 6;

		//	hitVertices.resize(hitVertNum);
		//	hitIndices.resize(hitIndexNum);
		//	index = 0;
		//	//���_�ۑ�
		//	for (int j = 0; j < heightSize - 1; ++j)
		//	{
		//		if (j % 2 == 1) { continue; }
		//		for (int i = 0; i < windthSize - 1; ++i)
		//		{
		//			if (i % 2 == 1) { continue; }
		//			int index1 = (hmInfo.terrainHeight * j) + i;// ����
		//			int index2 = (hmInfo.terrainHeight * j) + (i + 2);// �E��
		//			int index3 = (hmInfo.terrainHeight * (j + 2)) + i;// ����
		//			int index4 = (hmInfo.terrainHeight * (j + 2)) + (i + 2);// �E��

		//			int vertNum1 = index;
		//			index++;
		//			int vertNum2 = index;
		//			index++;
		//			int vertNum3 = index;
		//			index++;
		//			int vertNum4 = index;
		//			index++;

		//			// ����
		//			hitVertices[vertNum2].pos = hmInfo.heightMap[index3];
		//			hitVertices[vertNum2].uv = XMFLOAT2(1.0f, 0.0f);

		//			// �E��
		//			hitVertices[vertNum4].pos = hmInfo.heightMap[index4];
		//			hitVertices[vertNum4].uv = XMFLOAT2(0.0f, 1.0f);

		//			// ����
		//			hitVertices[vertNum1].pos = hmInfo.heightMap[index1];
		//			hitVertices[vertNum1].uv = XMFLOAT2(0.0f, 0.0f);

		//			// �E��
		//			hitVertices[vertNum3].pos = hmInfo.heightMap[index2];
		//			hitVertices[vertNum3].uv = XMFLOAT2(1.0f, 1.0f);
		//		}
		//	}

		//	//�C���f�b�N�X�ۑ�
		//	for (int i = 0; i < hitSurfaceNum; i++)
		//	{
		//		int vertexNum = i * 4;
		//		index = i * 6;
		//		hitIndices[index] = basicsIndices[0] + vertexNum;
		//		index++;
		//		hitIndices[index] = basicsIndices[1] + vertexNum;
		//		index++;
		//		hitIndices[index] = basicsIndices[2] + vertexNum;
		//		index++;
		//		hitIndices[index] = basicsIndices[3] + vertexNum;
		//		index++;
		//		hitIndices[index] = basicsIndices[4] + vertexNum;
		//		index++;
		//		hitIndices[index] = basicsIndices[5] + vertexNum;
		//	}

		//	//�@���ǉ�
		//	int normalNum = static_cast<int>(hitIndices.size() / 3);
		//	for (int i = 0; i < normalNum; i++)
		//	{
		//		index = i * 3;
		//		unsigned long index1 = hitIndices[index];
		//		index++;
		//		unsigned long index2 = hitIndices[index];
		//		index++;
		//		unsigned long index3 = hitIndices[index];

		//		XMVECTOR p0 = XMLoadFloat3(&hitVertices[index1].pos);
		//		XMVECTOR p1 = XMLoadFloat3(&hitVertices[index2].pos);
		//		XMVECTOR p2 = XMLoadFloat3(&hitVertices[index3].pos);

		//		XMVECTOR v1 = XMVectorSubtract(p0, p1);
		//		XMVECTOR v2 = XMVectorSubtract(p0, p2);

		//		XMVECTOR normal = XMVector3Cross(v1, v2);
		//		normal = XMVector3Normalize(normal);

		//		XMStoreFloat3(&hitVertices[index1].normal, normal);
		//		XMStoreFloat3(&hitVertices[index2].normal, normal);
		//		XMStoreFloat3(&hitVertices[index3].normal, normal);
		//	}
		//}
	}

	Mesh* mesh = new Mesh;

	//���b�V���֕ۑ�
	for (int i = 0; i < vertices.size(); i++)
	{
		mesh->AddVertex(vertices[i]);
	}

	//���b�V���֕ۑ�
	for (int i = 0; i < indexNum; i++)
	{
		mesh->AddIndex(indices[i]);
	}

	mesh->CreateBuffers();

	model = std::make_unique<Model>();
	model->SetMeshes(mesh);
}
