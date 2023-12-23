#include "Texture.h"
#include <DirectXTex.h>
#include <string>
#include "WindowApp.h"

ID3D12Device* Texture::device = nullptr;

void Texture::StaticInitialize(ID3D12Device* _device)
{
	// nullptr�`�F�b�N
	assert(!Texture::device);
	assert(_device);
	Texture::device = _device;

	DescriptorHeapManager::StaticInitialize(_device);
}

std::unique_ptr<Texture> Texture::Create(const std::string& _fileName, ID3D12GraphicsCommandList* _cmdList)
{
	//�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Texture* instance = new Texture();

	//�摜�t�@�C���̓ǂݍ���
	if (_cmdList == nullptr)
	{
		instance->LoadTexture(_fileName);
	} else
	{
		instance->LoadTextureFromDDSFile(_fileName, _cmdList);
	}

	return std::unique_ptr<Texture>(instance);
}

std::unique_ptr<Texture> Texture::CreateSRV_RTV(const std::string& _fileName)
{
	//�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Texture* instance = new Texture();

	//render�p�摜����
	instance->CreateRenderTexture(_fileName);

	return std::unique_ptr<Texture>(instance);
}

std::unique_ptr<Texture> Texture::CreateSRV_DSV(const std::string& _fileName, const std::array<UINT, 2>& _texSize)
{
	//�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Texture* instance = new Texture();

	//render�p�摜����
	instance->CreateDepthTexture(_fileName, _texSize);

	return std::unique_ptr<Texture>(instance);
}

Texture::~Texture()
{
	texBuffer.Reset();
	texConstBuffer.Reset();
}

void Texture::LoadTexture(const std::string& _fileName)
{
	HRESULT result;

	//�e�N�X�`���o�b�t�@�ԍ�
	static int texBuffNum = 0;

	////WIC�e�N�X�`���̃��[�h
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImage{};

	//���j�R�[�h�ɕϊ�
	wchar_t wfilePath[128];
	int bufferSize = MultiByteToWideChar(CP_ACP, 0,
		_fileName.c_str(), -1, wfilePath, _countof(wfilePath));

	//�f�[�^�`��
	//��؂蕶��'.'���o�Ă����ԍŌ�̕���������
	int pos1 = int(_fileName.rfind('.'));
	//��؂蕶���̌���t�@�C���g���q�Ƃ��ĕۑ�
	std::string fileExt_ = _fileName.substr(pos1 + 1, _fileName.size() - pos1 - 1);


	if (fileExt_ == "dds")
	{
		result = LoadFromDDSFile(
			wfilePath,
			DirectX::DDS_FLAGS_NONE,
			&metadata, scratchImage);
		assert(SUCCEEDED(result));
	} else {
		result = LoadFromWICFile(
			wfilePath,
			DirectX::WIC_FLAGS_NONE,
			&metadata, scratchImage);
		assert(SUCCEEDED(result));
	}

	metadata.format = DirectX::MakeSRGB(metadata.format);

	//�e�N�X�`���o�b�t�@�̐���
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//�e�N�X�`���o�b�t�@����
	result = device->CreateCommittedResource(//GPU���\�[�X�̐���
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texBuffer));
	assert(SUCCEEDED(result));

	//�~�b�v�}�b�v
	const int mipSize = int(metadata.mipLevels);
	for (int i = 0; i < mipSize; i++) {
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const DirectX::Image* img = scratchImage.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuffer->WriteToSubresource(
			(UINT)i,
			nullptr,//�S�̈�փR�s�[
			img->pixels,//���f�[�^�A�h���X
			(UINT)img->rowPitch,//�P���C���T�C�Y
			(UINT)img->slicePitch//1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	descriptor = std::make_unique<DescriptorHeapManager>();
	descriptor->CreateSRV(texBuffer, srvDesc);

	//�e�N�X�`�����擾
	D3D12_RESOURCE_DESC resDesc = texBuffer.Get()->GetDesc();
	//�X�v���C�g�̑傫�����摜�̉𑜓x�ɍ��킹��
	texSize = { UINT(resDesc.Width), UINT(resDesc.Height) };
}

void Texture::CreateRenderTexture(const std::string& _fileName)
{
	texSize = { WindowApp::GetWindowWidth(),WindowApp::GetWindowHeight() };

	//�e�N�X�`���o�b�t�@�����p�ϐ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		texSize[0], texSize[1],
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	const float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
	HRESULT result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, // �e�N�X�`���p�w��
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor),
		IID_PPV_ARGS(&texBuffer));
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
		result = texBuffer->WriteToSubresource(0, nullptr,
			img, row_pitch, depth_pitch);
		if (FAILED(result)) { assert(0); }
		delete[] img;
	}

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV����
	descriptor = std::make_unique<DescriptorHeapManager>();
	descriptor->CreateSRV(texBuffer, srvDesc);
	descriptor->CreateRTV(texBuffer);
}

void Texture::CreateDepthTexture(const std::string& _fileName, const std::array<UINT, 2>& _texSize)
{
	if (_texSize[0] <= 0) {
		texSize = { WindowApp::GetWindowWidth(),WindowApp::GetWindowHeight() };
	} else {
		texSize = _texSize;
	}

	//�[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			texSize[0],
			texSize[1],
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);
	//�[�x�o�b�t�@�̐���
	HRESULT result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&texBuffer)
	);
	if (FAILED(result)) { assert(0); }

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	descriptor = std::make_unique<DescriptorHeapManager>();
	descriptor->CreateSRV(texBuffer, srvDesc);
	descriptor->CreateDSV(texBuffer);
}

void Texture::LoadTextureFromDDSFile(const std::string& _fileName, ID3D12GraphicsCommandList* _cmdList)
{
	HRESULT result;

	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;

	//���j�R�[�h�ɕϊ�
	wchar_t wfilePath[128];
	int bufferSize = MultiByteToWideChar(CP_ACP, 0,
		_fileName.c_str(), -1, wfilePath, _countof(wfilePath));

	result = DirectX::LoadFromDDSFile(wfilePath, DirectX::DDS_FLAGS_NONE, &metadata, image);
	assert(SUCCEEDED(result));

	result = CreateTexture(device, metadata, &texBuffer);
	assert(SUCCEEDED(result));

	//���\�[�X�z��
	result = PrepareUpload(device, image.GetImages(), image.GetImageCount(), metadata, subresources);
	assert(SUCCEEDED(result));

	//�T�C�Y
	UINT totalBytes = (UINT)GetRequiredIntermediateSize(texBuffer.Get(), 0, UINT(subresources.size()));

	//�e�N�X�`���]���p�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(totalBytes),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texConstBuffer));
	assert(SUCCEEDED(result));

	UpdateSubresources(
		_cmdList,
		texBuffer.Get(), texConstBuffer.Get(),
		0, 0, (UINT)subresources.size(), subresources.data());

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		texBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	_cmdList->ResourceBarrier(1, &barrier);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = (UINT)metadata.mipLevels;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	descriptor = std::make_unique<DescriptorHeapManager>();
	descriptor->CreateSRV(texBuffer, srvDesc);
}
