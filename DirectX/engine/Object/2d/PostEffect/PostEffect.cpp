#include "PostEffect.h"
#include "WindowApp.h"

using namespace DirectX;

const float PostEffect::clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
std::vector<GraphicsPipelineManager::DrawSet> PostEffect::pipeline;

PostEffect::PostEffect()
	:Sprite()
{
}

PostEffect::~PostEffect()
{
}

void PostEffect::Finalize()
{
	descHeapRTV.Reset();
	descHeapDSV.Reset();
}

void PostEffect::Initialize()
{
	HRESULT result;

	//���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VERTEX) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) { assert(0); }

	// ���_�f�[�^
	VERTEX vertices
		[vertNum] = {
		{{ -1,-1,0.0f }, {0.0f,1.0f}}, // ����
		{{ -1,+1,0.0f }, {0.0f,0.0f}}, // ����
		{{ +1,-1,0.0f }, {1.0f,1.0f}}, // �E��
		{{ +1,+1,0.0f }, {1.0f,0.0f}}, // �E��
	};

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VERTEX) * 4;
	vbView.StrideInBytes = sizeof(VERTEX);

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }

	std::array<UINT, 2> texSize = texSize = { WindowApp::GetWindowWidth(),WindowApp::GetWindowHeight() };

	//�e�N�X�`���o�b�t�@�����p�ϐ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		texSize[0], texSize[1],
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	const int renderNum = int(TexType::size);

	// �e�N�X�`���p�o�b�t�@�̐���
	for (int i = 0; i < renderNum; i++)
	{
		texture[i] = std::make_unique<Texture>();

		result = device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, // �e�N�X�`���p�w��
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor),
			IID_PPV_ARGS(&texture[i]->texBuffer));
		if (FAILED(result)) { assert(0); }

		//�e�N�X�`����ԃN���A
		{
			//��f��
			const UINT pixel_count = texSize[0] * texSize[1];
			//��f��s���̃f�[�^�T�C�Y
			const UINT row_pitch = sizeof(UINT) * texSize[0];
			//��f�S�̂̃f�[�^�T�C�Y
			const UINT depth_pitch = row_pitch * texSize[1];
			//��f�C���[�W
			UINT* img = new UINT[pixel_count];
			for (UINT j = 0; j < pixel_count; j++) { img[j] = 0xff0000ff; }

			// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
			result = texture[i]->texBuffer->WriteToSubresource(0, nullptr,
				img, row_pitch, depth_pitch);
			if (FAILED(result)) { assert(0); }
			delete[] img;
		}
	}
	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < renderNum; i++)
	{
		//�f�X�N���v�^�q�[�v��SRV����
		texture[i]->descriptor = std::make_unique<DescriptorHeapManager>();
		texture[i]->descriptor->CreateSRV(texture[i]->texBuffer, srvDesc);
	}

	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHescDesc{};
	rtvDescHescDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHescDesc.NumDescriptors = renderNum;

	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&rtvDescHescDesc, IID_PPV_ARGS(&descHeapRTV));
	if (FAILED(result)) { assert(0); }

	//�f�X�N���v�^�q�[�v��RTV����
	for (int i = 0; i < renderNum; i++)
	{
		device->CreateRenderTargetView(texture[i]->texBuffer.Get(), nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}
}

void PostEffect::CreateDepthBuffer()
{
	HRESULT result;

	//�[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WindowApp::GetWindowWidth(),
			(UINT)WindowApp::GetWindowHeight(),
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//�[�x�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg�p
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuff));
	if (FAILED(result)) { assert(0); }

	//DSV�p�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[��1��

	//�f�X�N���v�^�q�[�v��DSV����
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	if (FAILED(result)) { assert(0); }

	//�[�x�o�b�t�@���\�[�X�ݒ�
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff.Get(), &dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

std::unique_ptr<PostEffect> PostEffect::Create()
{
	// Sprite�̃C���X�^���X�𐶐�
	PostEffect* instance = new PostEffect();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize();
	//�[�x�o�b�t�@����
	instance->CreateDepthBuffer();

	//���j�[�N�|�C���^��Ԃ�
	return std::unique_ptr<PostEffect>(instance);
}

void PostEffect::Draw(const std::vector<Texture*> _tex)
{
	// �萔�o�b�t�@�փf�[�^�]��
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->outline = 1.0f;
		constBuff->Unmap(0, nullptr);
	}

	ObjectBase::Draw(pipeline[0]);

	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[
	for (int i = 0; i < pipeline[0].texNum; i++) {
		cmdList->SetGraphicsRootDescriptorTable(i + 1, _tex[i]->descriptor->gpu);
	}
	// �`��R�}���h
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::PreDrawScene()
{
	const int renderNum = int(TexType::size);

	//���\�[�X�o���A��ύX
	for (int i = 0; i < renderNum; i++) {
		cmdList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(texture[i]->texBuffer.Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET));
	}

	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h���擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[renderNum];
	for (int i = 0; i < renderNum; i++)
	{
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h���擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(renderNum, rtvHs, false, &dsvH);

	//�r���[�|�[�g�ݒ�
	CD3DX12_VIEWPORT viewports[renderNum];
	CD3DX12_RECT scissorRects[renderNum];
	for (int i = 0; i < renderNum; i++)
	{
		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, FLOAT(WindowApp::GetWindowWidth()), FLOAT(WindowApp::GetWindowHeight()));
		scissorRects[i] = CD3DX12_RECT(0, 0, LONG(WindowApp::GetWindowWidth()), LONG(WindowApp::GetWindowHeight()));
	}

	cmdList->RSSetViewports(renderNum, viewports);
	//�V�U�����O��`�ݒ�
	cmdList->RSSetScissorRects(renderNum, scissorRects);

	//�S��ʃN���A
	for (int i = 0; i < renderNum; i++)
	{
		cmdList->ClearRenderTargetView(rtvHs[i], clearColor, 0, nullptr);
	}
	//�[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene()
{
	const int renderNum = int(TexType::size);

	//���\�[�X�o���A��ύX
	for (int i = 0; i < renderNum; i++)
	{
		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture[i]->texBuffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}
}