#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <d3dx12.h>

class Audio
{
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
		IXAudio2SourceVoice* pSourceVoice;
	};

public:

	Audio();
	~Audio();

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

	/// <summary>
	/// �����Đ�
	/// </summary>
	/// <param name="_number">�T�E���h�f�[�^�̊i�[�ԍ�</param>
	/// <param name="_roop">���[�v�̗L��</param>
	/// <returns>�f�[�^�̐ݒ���s������</returns>
	bool SoundPlayWava(int _number, bool _roop);

	/// <summary>
	/// �T�E���h�̒�~
	/// </summary>
	/// <param name="_number">�T�E���h�f�[�^�̊i�[�ԍ�</param>
	void StopSound(int _number);

private:
	//�T�E���h�f�[�^�z��
	static std::vector<SOUND_DATA*> soundData;

	//�T�E���h�̃o�b�t�@
	XAUDIO2_BUFFER buf{};

	static ComPtr<IXAudio2> xAudio2;
	static IXAudio2MasteringVoice* masterVoice;
};