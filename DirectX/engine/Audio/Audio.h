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

private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//チャンクヘッダ
	struct CHUNK_HEADER
	{
		char id[4];//チャンク毎のID
		int	size;  // チャンクサイズ
	};

	//RIFFヘッダチャンク
	struct RIFF_HEADER
	{
		CHUNK_HEADER chunk;//"RIFF"
		char type[4];//"WAVE"
	};

	//FMTチャンク
	struct FORMAT_CHUNK
	{
		CHUNK_HEADER chunk;//"fmt"
		WAVEFORMATEX fmt;//波形フォーマット
	};

	//音声データ
	struct SOUND_DATA
	{
		WAVEFORMATEX wfex;//波形フォーマット
		BYTE* pBuffer;//バッファの先頭アドレス
		unsigned int bufferSize;//バッファのサイズ
		IXAudio2SourceVoice* pSourceVoice;//dataの設定
	};

public:

	Audio();
	~Audio();

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="_number">サウンドデータの格納番号</param>
	/// <param name="_roop">ループの有無</param>
	/// <returns>データの設定を行ったか</returns>
	bool SoundPlayWava(const Sound::SoundName _number, const bool _roop = false, const float _volume = 1.0f);

	/// <summary>
	/// サウンドの停止
	/// </summary>
	/// <param name="_number">サウンドデータの格納番号</param>
	void StopSound(const Sound::SoundName _number);

	/// <summary>
	/// 完全解放
	/// </summary>
	void Delete();

private:

	/// <summary>
	/// 音声データ読み込み
	/// </summary>
	/// <param name="_fileName">ファイル名</param>
	/// <returns>格納番号</returns>
	int SoundLoadWave(const std::string& _fileName);

	/// <summary>
	/// データの生成
	/// </summary>
	/// <param name="_soundData">生成するサウンドデータ情報</param>
	void CreateSoundData(SOUND_DATA& _soundData);

private:
	//サウンドデータ配列
	std::vector<SOUND_DATA*> soundData;

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
};