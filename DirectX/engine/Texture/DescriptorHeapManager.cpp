#include "DescriptorHeapManager.h"

ID3D12Device* DescriptorHeapManager::device = nullptr;
std::array<DescriptorHeapManager::Descriptors, int(DescriptorHeapManager::DescriptorsType::size)> DescriptorHeapManager::descHeap;

void DescriptorHeapManager::StaticInitialize(ID3D12Device* _device)
{
	HRESULT result = S_FALSE;

	// nullptr�`�F�b�N
	assert(!DescriptorHeapManager::device);
	assert(_device);
	DescriptorHeapManager::device = _device;

	//�f�X�N���v�^�q�[�v�̐���
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = DescriptorsSize;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap[int(DescriptorsType::srv)].descHeap));
	if (FAILED(result)) { assert(0); }

	//�f�X�N���v�^�q�[�v�̐���rtv
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHescDesc{};
	rtvDescHescDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHescDesc.NumDescriptors = DescriptorsSize;
	result = device->CreateDescriptorHeap(&rtvDescHescDesc, IID_PPV_ARGS(&descHeap[int(DescriptorsType::rtv)].descHeap));
	if (FAILED(result)) { assert(0); }

	//�f�X�N���v�^�q�[�v�̐���dsv
	D3D12_DESCRIPTOR_HEAP_DESC dsvDescHescDesc{};
	dsvDescHescDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvDescHescDesc.NumDescriptors = DescriptorsSize;
	result = device->CreateDescriptorHeap(&dsvDescHescDesc, IID_PPV_ARGS(&descHeap[int(DescriptorsType::dsv)].descHeap));
	if (FAILED(result)) { assert(0); }

}

void DescriptorHeapManager::PreDraw(ID3D12GraphicsCommandList* _cmdList)
{
	assert(descHeap[int(DescriptorsType::srv)].descHeap);
	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap[int(DescriptorsType::srv)].descHeap.Get() };
	_cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void DescriptorHeapManager::RtvPreDraw(ID3D12GraphicsCommandList* _cmdList)
{
	assert(descHeap[int(DescriptorsType::rtv)].descHeap);
	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap[int(DescriptorsType::rtv)].descHeap.Get() };
	_cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void DescriptorHeapManager::Finalize()
{
	descHeap[int(DescriptorsType::srv)].descHeap.Reset();
	descHeap[int(DescriptorsType::rtv)].descHeap.Reset();
	descHeap[int(DescriptorsType::dsv)].descHeap.Reset();
}

DescriptorHeapManager::~DescriptorHeapManager()
{
	descHeap[int(DescriptorsType::srv)].TableManager[heapNumberSRV] = false;
	if (heapNumberRTV != -1) {
		descHeap[int(DescriptorsType::rtv)].TableManager[heapNumberRTV] = false;
	}
	if (heapNumberDSV != -1) {
		descHeap[int(DescriptorsType::dsv)].TableManager[heapNumberDSV] = false;
	}
}

void DescriptorHeapManager::CreateSRV(
	Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer, D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc)
{
	heapNumberSRV++;
	for (; heapNumberSRV < DescriptorsSize; heapNumberSRV++)
	{
		if (!descHeap[int(DescriptorsType::srv)].TableManager[heapNumberSRV])
		{
			descHeap[int(DescriptorsType::srv)].TableManager[heapNumberSRV] = true;
			break;
		}
	}

	cpu = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap[int(DescriptorsType::srv)].descHeap->GetCPUDescriptorHandleForHeapStart(), heapNumberSRV,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	gpu = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap[int(DescriptorsType::srv)].descHeap->GetGPUDescriptorHandleForHeapStart(), heapNumberSRV,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	//�q�[�v��texnumber�ԖڂɃV�F�[�_�[���\�[�X�r���[���쐬
	device->CreateShaderResourceView(
		_texBuffer.Get(),
		&_srvDesc,
		cpu);
}


void DescriptorHeapManager::CreateRTV(Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer)
{
	heapNumberRTV++;
	for (; heapNumberRTV < DescriptorsSize; heapNumberRTV++)
	{
		if (!descHeap[int(DescriptorsType::rtv)].TableManager[heapNumberRTV])
		{
			descHeap[int(DescriptorsType::rtv)].TableManager[heapNumberRTV] = true;
			break;
		}
	}

	//�q�[�v��texnumber�ԖڂɃV�F�[�_�[���\�[�X�r���[���쐬
	device->CreateRenderTargetView(
		_texBuffer.Get(),
		nullptr,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap[int(DescriptorsType::rtv)].descHeap->GetCPUDescriptorHandleForHeapStart(), heapNumberRTV,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
}

void DescriptorHeapManager::CreateDSV(Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer)
{
	heapNumberDSV++;
	for (; heapNumberDSV < DescriptorsSize; heapNumberDSV++)
	{
		if (!descHeap[int(DescriptorsType::dsv)].TableManager[heapNumberDSV])
		{
			descHeap[int(DescriptorsType::dsv)].TableManager[heapNumberDSV] = true;
			break;
		}
	}

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	//�q�[�v��n�ԖڂɃf�v�X�X�e���V���r���[���쐬
	device->CreateDepthStencilView(
		_texBuffer.Get(),
		&dsvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap[int(DescriptorsType::dsv)].descHeap->GetCPUDescriptorHandleForHeapStart(), heapNumberDSV,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV)));
}
