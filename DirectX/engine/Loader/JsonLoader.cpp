#include "Loader/JsonLoader.h"
#include <json.hpp>

using njson = nlohmann::json;
const std::string JsonLoader::base_directory = "Resources/";

void JsonLoader::LoadShaderName(const std::string& _fileName, std::vector<Shader>* _shaderName)
{
    const std::string fullpath = base_directory + "Shaders/" + _fileName + ".json";

    //�t�@�C���X�g���[��
    std::ifstream file;

    //�t�@�C�����J��
    file.open(fullpath);
    if (file.fail()) {
        assert(0);
    }

    //�𓀃f�[�^�ۑ���
    njson deserialized;

    //��
    file >> deserialized;

    //�f�[�^�����������̃`�F�b�N
    assert(deserialized.is_object());
    assert(deserialized.contains("name"));
    assert(deserialized["name"].is_string());

    //"name"�𕶎���Ƃ��Ď擾
    std::string name = deserialized["name"].get<std::string>();
    //���������x���f�[�^�t�@�C�����`�F�b�N
    assert(name.compare("scene") == 0);

    for (njson& itr : deserialized["shaders"])
    {
        njson& data = itr["shader"];

        Shader add{};
        add.name = data["Name"];
        for (auto& i : data["Type"]) {
            add.type.emplace_back(i);
        }
        _shaderName->emplace_back(add);
    }
}

void JsonLoader::LoadPipeline(const std::string& _fileName, std::vector<Pipeline>* _pipeline)
{
    const std::string fullpath = base_directory + "Pipeline/" + _fileName + ".json";

    //�t�@�C���X�g���[��
    std::ifstream file;

    //�t�@�C�����J��
    file.open(fullpath);
    if (file.fail()) {
        assert(0);
    }

    //�𓀃f�[�^�ۑ���
    njson deserialized;

    //��
    file >> deserialized;

    //�f�[�^�����������̃`�F�b�N
    assert(deserialized.is_object());
    assert(deserialized.contains("name"));
    assert(deserialized["name"].is_string());

    //"name"�𕶎���Ƃ��Ď擾
    std::string name = deserialized["name"].get<std::string>();
    //���������x���f�[�^�t�@�C�����`�F�b�N
    assert(name.compare("scene") == 0);

    for (njson& itr : deserialized["pipelines"])
    {
        Pipeline addPipe{};
        //�g�����X�t�H�[�����
        njson& data = itr["pipeline"];
        addPipe.name = data["Name"];
        for (auto& i : data["ShaderType"]) {
            addPipe.shaderType.emplace_back(i);
        }
        for (auto& i : data["InputLayoutType"]) {
            addPipe.inputLayoutType.emplace_back(i);
        }
        addPipe.rtvNum = data["RtvNum"];
        addPipe.blendMode = data["BlendMode"];
        addPipe.drawMode = data["DrawMode"];
        addPipe.drawType = data["DrawType"];
        addPipe.textureNum = data["TextureNum"];
        addPipe.rootparams = data["Rootparams"];

        _pipeline->emplace_back(addPipe);
    }
}

void JsonLoader::SerializeJson(const std::string& _fileName, const std::array<float, 3> _cameraDist,
    const std::array<int, 3> _mapSize, std::vector<std::vector<std::vector<int>>> _map)
{
    Json x;
    x.name = _fileName;
    x.cameraDist = _cameraDist;
    x.mapSize = _mapSize;
	x.map = _map;
    x.installationSurface = 0;

    // �t�@�C���o��
    std::ofstream os(_fileName, std::ios::out);
    cereal::JSONOutputArchive archiveFile(os);
    x.serialize(archiveFile);
}

bool JsonLoader::DeserializeJson(const std::string _fileName,
    std::array<float, 3>* _cameraDist, std::vector<std::vector<std::vector<int>>>* _map)
{
    Json x;

    std::ifstream os(_fileName, std::ios::in);
    if (!os.is_open()) {
        return false;
    }
    cereal::JSONInputArchive archive(os);
    x.serialize(archive);

    *_cameraDist = x.cameraDist;
    *_map = x.map;

    return true;
}

void JsonLoader::SerializeBinary(const std::string& _fileName,std::vector<std::vector<std::vector<int>>> _map)
{
    Binary x;
    x.map = _map;

    // �t�@�C���o��
    std::ofstream os("binary/" + _fileName + ".binary", std::ios::out | std::ios::binary);
    cereal::BinaryOutputArchive archive(os);
    x.serialize(archive);
}

bool JsonLoader::DeserializeBinary(const std::string _fileName, std::vector<std::vector<std::vector<int>>>* _map)
{
    Binary x;

    std::ifstream os("binary/" + _fileName +".binary", std::ios::in | std::ios::binary);
    if (!os.is_open()) {
        return false;
    }
    cereal::BinaryInputArchive archive(os);
    x.serialize(archive);

    *_map = x.map;

    return true;
}
