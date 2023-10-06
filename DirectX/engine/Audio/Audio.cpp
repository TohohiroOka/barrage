#include "Audio.h"
#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

std::vector<Audio::SOUND_DATA*> Audio::soundData;
Microsoft::WRL::ComPtr<IXAudio2>Audio::xAudio2;
IXAudio2MasteringVoice* Audio::masterVoice;

Audio::Audio()
{
	XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);//XAudio�G���W���̃C���X�^���X����
	xAudio2->CreateMasteringVoice(&masterVoice);//�}�X�^�[�{�C�X����
	masterVoice->SetVolume(0.1f);//�S�̂̉���}
}

Audio::~Audio()
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

bool Audio::SoundPlayWava(int number, bool roop)
{
	HRESULT result;

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
	buf.pAudioData = soundData[number]->pBuffer;
	buf.AudioBytes = soundData[number]->bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (roop == true)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//�g�`�f�[�^�̍Đ�
	result = soundData[number]->pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));
	result = soundData[number]->pSourceVoice->Start();
	assert(SUCCEEDED(result));

	buf = { NULL };

	return true;
}

void Audio::StopSound(int number)
{
	HRESULT result;

	result = soundData[number]->pSourceVoice->Stop(0);
	assert(SUCCEEDED(result));
	result = soundData[number]->pSourceVoice->FlushSourceBuffers();
	assert(SUCCEEDED(result));
	result = soundData[number]->pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));
}