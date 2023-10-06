#include "ShaderManager.h"
#include "Helpar.h"
#include "Loader/JsonLoader.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

const std::wstring pass = L"Resources/Shaders/";

//�G���[�̏o��
void OutputErrstr(ID3DBlob* _errorBlob)
{
	// errorBlob����G���[���e��string�^�ɃR�s�[
	std::string errstr;
	errstr.resize(_errorBlob->GetBufferSize());

	std::copy_n((char*)_errorBlob->GetBufferPointer(),
		_errorBlob->GetBufferSize(),
		errstr.begin());
	errstr += "\n";
	// �G���[���e���o�̓E�B���h�E�ɕ\��
	OutputDebugStringA(errstr.c_str());
	exit(1);
}

//�p�X�̎擾
std::wstring GetFullPath(LPCWSTR _assetName)
{
	return pass + _assetName;
}

//�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
ID3DBlob* CompileShader(const LPCWSTR _fileName, LPCSTR _target)
{
	HRESULT result = S_FALSE;
	//�o�͗p�I�u�W�F�N�g
	ID3DBlob* blob = nullptr;
	//�C���N���[�h�ݒ�
	ID3DInclude* includeFrag = D3D_COMPILE_STANDARD_FILE_INCLUDE;
	// �f�o�b�O�p�ݒ�
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	//�G���[�I�u�W�F�N�g
	ID3DBlob* error;

	result = D3DCompileFromFile(GetFullPath(_fileName).c_str(), nullptr, includeFrag, "main", _target,
		compileFlags, 0, &blob, &error);
	if (FAILED(result))
	{
		OutputErrstr(error);
	}

	return blob;
}

std::unique_ptr<ShaderManager> ShaderManager::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	ShaderManager* instance = new ShaderManager();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->LoadShader();

	return std::unique_ptr<ShaderManager>(instance);
}

void ShaderManager::LoadShader()
{
	HRESULT result = S_FALSE;

	//���_�V�F�[�_�[���f��
	LPCSTR vsModel = "vs_5_0";
	//�s�N�Z���V�F�[�_�[���f��
	LPCSTR psModel = "ps_5_0";
	//�W�I���g���V�F�[�_�[���f��
	LPCSTR gsModel = "gs_5_0";

	std::vector<JsonLoader::Shader> shader{};
	JsonLoader::LoadShaderName("Shader", &shader);
	for (auto& i : shader) {
		for (auto& j : i.type) {
			std::string inName;
			if (j == "v") {
				inName = i.name + "VS";
				shaderObject[inName] = CompileShader(GetName(inName, ".hlsl"), vsModel);
			} else if (j == "p") {
				inName = i.name + "PS";
				shaderObject[inName] = CompileShader(GetName(inName, ".hlsl"), psModel);
			} else if (j == "g") {
				inName = i.name + "GS";
				shaderObject[inName] = CompileShader(GetName(inName, ".hlsl"), gsModel);
			}
		}
	}
}

void ShaderManager::Finalize()
{
	for (auto shader : shaderObject)
	{
		shader.second.Reset();
	}
	shaderObject.clear();

	for (auto shader : shaderObjectCS)
	{
		shader.second.Reset();
	}
	shaderObjectCS.clear();
}