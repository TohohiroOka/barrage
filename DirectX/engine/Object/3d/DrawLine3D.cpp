#include "DrawLine3D.h"
#include "Camera.h"

#include <string>
#include <vector>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

ID3D12Device* DrawLine3D::device = nullptr;
ID3D12GraphicsCommandList* DrawLine3D::cmdList = nullptr;
Camera* DrawLine3D::camera = nullptr;
GraphicsPipelineManager::GRAPHICS_PIPELINE DrawLine3D::pipeline;

const float PI = 3.141592f;

DrawLine3D::~DrawLine3D()
{
	//�o�b�t�@�����
	vertBuff.Reset();
	indexBuff.Reset();
	constBuff.Reset();
}

std::unique_ptr<DrawLine3D> DrawLine3D::Create(int _lineNum)
{
	DrawLine3D* instance = new DrawLine3D();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(_lineNum)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	instance->Update();

	return std::unique_ptr<DrawLine3D>(instance);
}

void DrawLine3D::StaticInitialize(ID3D12Device* _device)
{
	// �������`�F�b�N
	assert(!DrawLine3D::device);

	// nullptr�`�F�b�N
	assert(_device);

	DrawLine3D::device = _device;
}

bool DrawLine3D::Initialize(int _lineNum)
{
	HRESULT result = S_FALSE;

	//���_�f�[�^�̗v�f��
	vertexArrayNum = vertNum * _lineNum;
	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	const UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * vertexArrayNum);
	//���_�f�[�^�̗v�f���ύX
	vertices.resize(vertexArrayNum);

	//���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[�̐���
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(XMFLOAT3);

	//�C���f�b�N�z��̗v�f��
	indexArrayNum = indexNum * _lineNum;
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	const UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indexArrayNum);

	//�g���C���f�b�N�X�̍쐬
	std::vector<unsigned short> indices;
	for (int i = 0; i < int(indexArrayNum); i++)
	{
		//���C���f�b�N�X
		unsigned short addIndex;

		//�x�[�X + ��{�Ŏg���C���f�b�N�X�ԍ��̍ő�l * (���݂̔z�� / ��{���̔z��̍ő吔)
		addIndex = BASE_INDICES[i % indexNum] + 4 * int(i / indexNum);

		//�z��̈�Ԍ��ɓ����
		indices.push_back(addIndex);
	}

	//�C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB), // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	std::copy(indices.begin(), indices.end(), indexMap);
	indexBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;
}

float DrawLine3D::GetAngle(const XMFLOAT3& _startPoint, const XMFLOAT3& _endPoint)
{
	float angle = atan2f(_endPoint.y - _startPoint.y, _endPoint.x - _startPoint.x) * (180.0f / PI);
	return angle;
}

void DrawLine3D::SetLine(XMFLOAT3 _startPoint[], XMFLOAT3 _endPoint[], float _width)
{
	HRESULT result;

	//��
	XMFLOAT2 lineWidth1 = {};
	XMFLOAT2 lineWidth2 = {};

	//���̖{������
	const int LineNum = vertexArrayNum / vertNum;
	for (int i = 0; i < LineNum; i++)
	{
		//�p�x
		const float angle = GetAngle(_startPoint[i], _endPoint[i]);

		//�������p�l
		float LEFT = (angle + 90.0f) * (PI / 180.0f);
		float RIGHT = (angle - 90.0f) * (PI / 180.0f);

		lineWidth1.x = _width * cosf(LEFT);
		lineWidth1.y = _width * sinf(LEFT);
		lineWidth2.x = _width * cosf(RIGHT);
		lineWidth2.y = _width * sinf(RIGHT);

		// ���_�f�[�^
		int arrayNum = i * 4;
		vertices[arrayNum] = { _endPoint[i].x + lineWidth2.x, _endPoint[i].y + lineWidth2.y, _endPoint[i].z }; // ����
		arrayNum++;
		vertices[arrayNum] = { _startPoint[i].x + lineWidth2.x, _startPoint[i].y + lineWidth2.y, _startPoint[i].z }; // ����
		arrayNum++;
		vertices[arrayNum] = { _endPoint[i].x + lineWidth1.x, _endPoint[i].y + lineWidth1.y, _endPoint[i].z }; // �E��
		arrayNum++;
		vertices[arrayNum] = { _startPoint[i].x + lineWidth1.x, _startPoint[i].y + lineWidth1.y, _startPoint[i].z }; // �E��
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}
}

void DrawLine3D::PreDraw(ID3D12GraphicsCommandList* _cmdList)
{
	DrawLine3D::cmdList = _cmdList;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipeline.pipelineState.Get());

	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(pipeline.rootSignature.Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void DrawLine3D::PostDraw()
{
	// �R�}���h���X�g������
	DrawLine3D::cmdList = nullptr;
}

void DrawLine3D::Update()
{
	//���[���h�s��ϊ�
	XMMATRIX matWorld = XMMatrixIdentity();
	XMMATRIX matScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	matWorld *= matScale;

	XMMATRIX matRot;//�p�x
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
	matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));
	matRot *= XMMatrixRotationY(XMConvertToRadians(0.0f));
	matWorld *= matRot;

	XMMATRIX matTrans;//���s����
	matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	matWorld *= matTrans;

	//�萔�o�b�t�@�Ƀf�[�^��]��
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->matWorld = matWorld;
		if (camera != nullptr)
		{
			const XMMATRIX& matViewProjection = camera->GetView() * camera->GetProjection();
			constMap->viewproj = matViewProjection;
		} else {
			constMap->viewproj = XMMatrixIdentity();
		}
	}
	constBuff->Unmap(0, nullptr);
}

void DrawLine3D::Draw()
{
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//�`��R�}���h
	cmdList->DrawIndexedInstanced(indexArrayNum, 1, 0, 0, 0);
}