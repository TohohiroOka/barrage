#include "ComputeShaderManager.h"

#include <d3dcompiler.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

ID3D12Device* ComputeShaderManager::device = nullptr;
ID3D12GraphicsCommandList* ComputeShaderManager::cmdList = nullptr;
ComPtr<ID3DBlob> ComputeShaderManager::computeShader = nullptr;
ComPtr<ID3D12RootSignature> ComputeShaderManager::rootSignature = nullptr;
ComPtr<ID3D12PipelineState> ComputeShaderManager::pipelineState = nullptr;
ComPtr<ID3D12DescriptorHeap> ComputeShaderManager::Heap = nullptr;

const int MAX = 10;
ComputeShaderManager::InputData reset = {};
std::vector<ComputeShaderManager::InputData>test(MAX, reset);

ComputeShaderManager::~ComputeShaderManager()
{
	inputBuffer.Reset();
}

void ComputeShaderManager::StaticInitialize(ID3D12Device* device)
{
	ComputeShaderManager::device = device;
	//�V�F�[�_�[�̓ǂݍ���
	LoadShader();
	//�p�C�v���C���̐���
	CreatePipeline();
	//�f�X�N���v�^�q�[�v�̐���
	CreateDescriptor();
}

std::unique_ptr<ComputeShaderManager> ComputeShaderManager::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	ComputeShaderManager* instance = new ComputeShaderManager();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize();

	return std::unique_ptr<ComputeShaderManager>(instance);
}

void ComputeShaderManager::Initialize()
{
	HRESULT result = S_FALSE;

	//�萔�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	prop.Type = D3D12_HEAP_TYPE_DEFAULT;
	prop.VisibleNodeMask = 1;
	//�T�C�Y�͒萔�o�b�t�@�Ɠ����悤�Ɏw��
	D3D12_RESOURCE_DESC desc{};
	desc.Alignment = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Height = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels = 1;
	desc.SampleDesc = { 1, 0 };
	desc.Width = (sizeof(XMFLOAT3) * test.size() + 0xff) & ~0xff;

	//�ݒ�̔��f
	result = device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
		IID_PPV_ARGS(&inputBuffer));
	if (FAILED(result)) {
		assert(0);
	}

	//�����GPU����󂯎��̂�char�^�ɂ��Ă��܂�
	D3D12_UNORDERED_ACCESS_VIEW_DESC outdesc{};
	outdesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	outdesc.Format = DXGI_FORMAT_UNKNOWN;
	outdesc.Buffer.NumElements = (UINT)test.size();
	outdesc.Buffer.StructureByteStride = sizeof(XMFLOAT3);

	device->CreateUnorderedAccessView(inputBuffer.Get(), nullptr, &outdesc, Heap->GetCPUDescriptorHandleForHeapStart());
}

void ComputeShaderManager::LoadShader()
{
	HRESULT result = S_FALSE;

	//�V�F�[�_�[�R���p�C��
	auto hr = D3DCompileFromFile(L"Resources/Shaders/BasicCS.hlsl", nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "cs_5_0", D3DCOMPILE_DEBUG |
		D3DCOMPILE_SKIP_OPTIMIZATION, 0, &computeShader, nullptr);
}

void ComputeShaderManager::CreatePipeline()
{
	HRESULT result = S_FALSE;

	//�V�F�[�_�[���烋�[�g�V�O�l�`�������擾
	ID3DBlob* sig = nullptr;
	result = D3DGetBlobPart(computeShader->GetBufferPointer(), computeShader->GetBufferSize(),
		D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
	if (FAILED(result)) {
		assert(0);
	}

	//���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	if (FAILED(result)) {
		assert(0);
	}

	//�p�C�v���C���̐ݒ�
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = computeShader->GetBufferPointer();
	desc.CS.BytecodeLength = computeShader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignature.Get();

	//�ݒ�̔��f
	result = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineState));
	if (FAILED(result)) {
		assert(0);
	}

	rootSignature->SetName(L"cs_PEroot");
	pipelineState->SetName(L"cs_PEpipe");
}

void ComputeShaderManager::CreateDescriptor()
{
	HRESULT result = S_FALSE;

	//���\�[�X��1�Ƃ��č쐬
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&Heap));
	if (FAILED(result)) {
		assert(0);
	}
}

void ComputeShaderManager::ShaderUpdate(UINT max, XMFLOAT3* startPosition, XMFLOAT3* endPosition,
	XMFLOAT3* nowPosition, float* time)
{
	HRESULT result;
	size = max;
	InputData* inData = new InputData[max];
	for (int i = 0; i < size; i++)
	{
		inData[i].startPosition = startPosition[i];
		inData[i].endPosition = endPosition[i];
		inData[i].nowPosition = nowPosition[i];
		inData[i].time = time[i];
	}
	////�萔�o�b�t�@�Ƀf�[�^��]��
	D3D12_RANGE range{ 0, 1 };
	result = inputBuffer->Map(0, &range, (void**)&data);
	data = inData;
	inputBuffer->Unmap(0, &range);

	//�p�C�v���C���̃Z�b�g
	cmdList->SetPipelineState(pipelineState.Get());

	//���[�g�V�O�l�`���̃Z�b�g
	cmdList->SetComputeRootSignature(rootSignature.Get());

	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { Heap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	auto handle = Heap->GetGPUDescriptorHandleForHeapStart();
	cmdList->SetComputeRootDescriptorTable(0, handle);

	//�R���s���[�g�V�F�[�_�[�̎��s(�����256�̃X���b�h�O���[�v���w��)
	cmdList->Dispatch((UINT)test.size(), 1, 1);
}

XMFLOAT3* ComputeShaderManager::GetConstBufferNum()
{
	HRESULT result;

	D3D12_RANGE range{ 0, 1 };
	result = inputBuffer->Map(0, &range, (void**)&data);
	inputBuffer->Unmap(0, &range);

	//GPU����f�[�^�����炤
	test.assign(data, data + test.size());

	//�o�͒l�̕ۑ��ϐ�
	XMFLOAT3* outputNum = new XMFLOAT3[size];
	for (int i = 0; i < size; i++)
	{
		outputNum[i] = test[i].nowPosition;
	}

	return outputNum;
}

void ComputeShaderManager::PreUpdate(ID3D12GraphicsCommandList* cmdList)
{
	ComputeShaderManager::cmdList = cmdList;
}

void ComputeShaderManager::PostUpdate()
{
	ComputeShaderManager::cmdList = nullptr;
}

void ComputeShaderManager::Finalize()
{
	computeShader.Reset();
	rootSignature.Reset();
	pipelineState.Reset();
	Heap.Reset();
}