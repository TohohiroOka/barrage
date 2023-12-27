#include "../Texture/TextureManager.h"
#include "WindowApp.h"

std::unordered_map<std::string, TextureManager::INFORMATION> TextureManager::texture;

TextureManager::TextureManager(const std::string& _name)
{
	name = _name;
}

TextureManager::~TextureManager()
{
}

void TextureManager::LoadTexture(const std::string& _keepName, const std::string& _filename, bool _isDelete)
{
	//�����L�[������΂��̂܂ܕԂ�
	if (texture.count(_keepName)) { return; }

	//�e�N�X�`���ǂݍ���
	texture[_keepName].instance = Texture::Create(_filename);
	texture[_keepName].isDelete = _isDelete;
}

void TextureManager::CreateRenderTexture(const std::string& _fileName)
{
	//�����L�[������΂��̂܂ܕԂ�
	if (texture.count(_fileName)) { return; }

	//�e�N�X�`���ǂݍ���
	texture[_fileName].instance = Texture::CreateSRV_RTV(_fileName);
	texture[_fileName].isDelete = false;
}

void TextureManager::CreateDepthTexture(const std::string& _fileName, const std::array<UINT, 2>& _texSize)
{
	//�����L�[������΂��̂܂ܕԂ�
	if (texture.count(_fileName)) { return; }

	std::array<UINT, 2> texSize = {};
	if (_texSize[0] <= 0) {
		texSize = { WindowApp::GetWindowWidth(),WindowApp::GetWindowHeight() };
	} else {
		texSize = _texSize;
	}

	//�e�N�X�`���ǂݍ���
	texture[_fileName].instance = Texture::CreateSRV_DSV(_fileName, texSize);
	texture[_fileName].isDelete = false;
}

void TextureManager::SceneFinalize()
{
	for (auto itr = texture.begin(); itr != texture.end();) {
		if (!(*itr).second.isDelete) {
			++itr;
			continue;
		}
		itr = texture.erase(itr);
	}
}

void TextureManager::Finalize()
{
	for (auto itr = texture.begin(); itr != texture.end(); ++itr)
	{
		(*itr).second.instance.reset();
	}
	texture.clear();
}
