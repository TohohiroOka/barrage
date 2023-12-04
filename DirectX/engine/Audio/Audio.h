#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <d3dx12.h>
#include "Singleton.h"
#include "game//system/Sound.h"

class Audio : public Singleton<Audio>
{
	friend class Singleton<Audio>;

private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//�`�����N�w�b�_
	struct CHUNK_HEADER
	{
		char id[4];//�`�����N����ID
		int	size;  // �`�����N�T�C�Y
	};

	//RIFF�w�b�_�`�����N
	struct RIFF_HEADER
	{
		CHUNK_HEADER chunk;//"RIFF"
		char type[4];//"WAVE"
	};

	//FMT�`�����N
	struct FORMAT_CHUNK
	{
		CHUNK_HEADER chunk;//"fmt"
		WAVEFORMATEX fmt;//�g�`�t�H�[�}�b�g
	};

	//�����f�[�^
	struct SOUND_DATA
	{
		WAVEFORMATEX wfex;//�g�`�t�H�[�}�b�g
		BYTE* pBuffer;//�o�b�t�@�̐擪�A�h���X
		unsigned int bufferSize;//�o�b�t�@�̃T�C�Y
		IXAudio2SourceVoice* pSourceVoice;//data�̐ݒ�
	};

public:

	Audio();
	~Audio();

	/// <summary>
	/// �����Đ�
	/// </summary>
	/// <param name="_number">�T�E���h�f�[�^�̊i�[�ԍ�</param>
	/// <param name="_roop">���[�v�̗L��</param>
	/// <returns>�f�[�^�̐ݒ���s������</returns>
	bool SoundPlayWava(const Sound::SoundName _number, const bool _roop = false, const float _volume = 1.0f);

	/// <summary>
	/// �T�E���h�̒�~
	/// </summary>
	/// <param name="_number">�T�E���h�f�[�^�̊i�[�ԍ�</param>
	void StopSound(const Sound::SoundName _number);

	/// <summary>
	/// ���S���
	/// </summary>
	void Delete();

private:

	/// <summary>
	/// �����f�[�^�ǂݍ���
	/// </summary>
	/// <param name="_fileName">�t�@�C����</param>
	/// <returns>�i�[�ԍ�</returns>
	int SoundLoadWave(const std::string& _fileName);

	/// <summary>
	/// �f�[�^�̐���
	/// </summary>
	/// <param name="_soundData">��������T�E���h�f�[�^���</param>
	void CreateSoundData(SOUND_DATA& _soundData);

private:
	//�T�E���h�f�[�^�z��
	std::vector<SOUND_DATA*> soundData;

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
};