#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "Pipeline/GraphicsPipelineManager.h"
#include "Texture.h"
#include "EngineUseTexture.h"

class TextureManager
{
private:

	struct INFORMATION
	{
		bool isDelete = false; //�V�[���J�ڂō폜���s����
		std::unique_ptr<Texture> instance = nullptr;
	};

public:

	TextureManager(const std::string& _name);
	~TextureManager();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="_keepName">�ۑ���</param>
	/// <param name="_filename">�摜�t�@�C����</param>
	/// <param name="_isDelete">�V�[���J�ڂō폜���s����</param>
	static void LoadTexture(const std::string& _keepName, const std::string& _filename, bool _isDelete = true);

	/// <summary>
	/// �����_�[�^�[�Q�b�g�p�摜����
	/// </summary>
	/// <param name="_fileName"></param>
	static void CreateRenderTexture(const std::string& _fileName);

	/// <summary>
	/// �[�x�l�p�摜�쐬
	/// </summary>
	/// <param name="_fileName"></param>
	/// <param name="_texSize">�摜�T�C�Y</param>
	static void CreateDepthTexture(const std::string& _fileName, const std::array<UINT, 2>& _texSize = {});

	/// <summary>
	/// �V�[�����Ƃ̉������
	/// </summary>
	static void SceneFinalize();

	/// <summary>
	/// �������
	/// </summary>
	static void Finalize();

	/// <summary>
	/// ���̖��O�̉摜�����݂��邩
	/// </summary>
	/// <param name="_name">�摜��</param>
	/// <returns></returns>
	static bool GetIsTextureName(const std::string& _name) {
		return texture.count(_name);
	}

	std::array<UINT, 2>& GetSize() {
		return texture[name].instance->texSize;
	}

	ID3D12Resource* GetTexBuffer() {
		return texture[name].instance->texBuffer.Get();
	}

	DescriptorHeapManager* GetDescriptor() {
		return texture[name].instance->descriptor.get();
	}

	/// <summary>
	/// �摜�̑傫�����o��
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT2 GetTexSize() {
		//�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
		if (texture[name].instance->texBuffer.Get()) {
			return { float(texture[name].instance->texSize[0]),float(texture[name].instance->texSize[1]) };
		} else {
			return { 0.0f,0.0f };
		}
	}

	void SetTexture(const std::string& _name) { name = _name; }
	std::string& GetTexture() { return name; }

private:
	//�e�N�X�`�����
	static std::unordered_map<std::string, INFORMATION> texture;

	std::string name;
};

