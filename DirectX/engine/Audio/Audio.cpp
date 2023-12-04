#include "Audio.h"
#include <fstream>
#include <cassert>
#pragma comment(lib,"xaudio2.lib")

Audio::Audio()
{
	XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);//XAudio�G���W���̃C���X�^���X����
	xAudio2->CreateMasteringVoice(&masterVoice);//�}�X�^�[�{�C�X����

	for (auto& i : Sound::SoundNameStr) {
		SoundLoadWave("Resources/sound/" + i + ".wav");
	}
}

Audio::~Audio()
{
}

int Audio::SoundLoadWave(const std::string& fileName)
{
	//���݂̔z��T�C�Y
	int size = (UINT)soundData.size();

	SOUND_DATA* addSoundData = nullptr;
	addSoundData = new SOUND_DATA();

	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;

	//wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(fileName, std::ios_base::binary);

	//�t�@�C���I�[�v�����s�����m
	assert(file.is_open());

	///wav�f�[�^�ǂݍ���
	//RIFF�w�b�_�[�̓ǂݍ���
	RIFF_HEADER riff;
	file.read((char*)&riff, sizeof(riff));

	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	//Format�`�����N�̓ǂݍ���
	FORMAT_CHUNK format = {};

	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(CHUNK_HEADER));

	//�`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Data�`�����N�̓ǂݍ���
	CHUNK_HEADER data;
	file.read((char*)&data, sizeof(data));

	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK ", 4) == 0 ||
		strncmp(data.id, "LIST", 4) == 0)
	{
		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);

		//�ēx�ǂݍ���
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}

	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	///�t�@�C���N���[�Y
	//Wave�t�@�C�������
	file.close();

	///�ǂݍ��񂾉����f�[�^��Ԃ�
	//return���邽�߂̉����f�[�^
	addSoundData->wfex = format.fmt;
	addSoundData->pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	addSoundData->bufferSize = data.size;

	CreateSoundData(*addSoundData);
	float SourceVoiceChannelVolumes[1] = { 0.0 };
	addSoundData->pSourceVoice->SetChannelVolumes(1, SourceVoiceChannelVolumes);

	soundData.push_back(addSoundData);

	return size;
}

void Audio::CreateSoundData(SOUND_DATA& soundData) {
	HRESULT result;

	//�g�`�t�H�[�}�b�g������SourceVoice�̐���
	result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));
}

bool Audio::SoundPlayWava(const Sound::SoundName _number, const bool _roop, const float _volume)
{
	HRESULT result;

	int number = int(_number);

	XAUDIO2_VOICE_STATE xa3state;
	soundData[number]->pSourceVoice->GetState(&xa3state);

	//0�łȂ��Ȃ特�y���Đ����ɂȂ邽�߃X���[����
	if (xa3state.BuffersQueued != 0)
	{
		return false;
	}

	//�f�[�^����
	CreateSoundData(*soundData[number]);

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData[number]->pBuffer;
	buf.AudioBytes = soundData[number]->bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (_roop == true)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	soundData[number]->pSourceVoice->SetVolume(_volume);

	//�g�`�f�[�^�̍Đ�
	result = soundData[number]->pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));
	result = soundData[number]->pSourceVoice->Start();
	assert(SUCCEEDED(result));

	buf = { NULL };

	return true;
}

void Audio::StopSound(const Sound::SoundName _number)
{
	HRESULT result;

	int number = int(_number);

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData[number]->pBuffer;
	buf.AudioBytes = soundData[number]->bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	result = soundData[number]->pSourceVoice->Stop(0);
	assert(SUCCEEDED(result));
	result = soundData[number]->pSourceVoice->FlushSourceBuffers();
	assert(SUCCEEDED(result));
	result = soundData[number]->pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));
}

void Audio::Delete()
{
	//xAudio2���
	xAudio2.Reset();

	const int size = (UINT)soundData.size();

	//�o�b�t�@�̃����������
	for (int i = 0; i < size; i++)
	{
		delete[] soundData[i]->pBuffer;

		soundData[i]->pBuffer = 0;
		soundData[i]->bufferSize = 0;
		soundData[i]->wfex = {};
	}

	soundData.clear();
}
