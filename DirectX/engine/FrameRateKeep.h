#pragma once
#include <chrono>
#include <d3dx12.h>

class WindowApp;

class FrameRateKeep
{
private:

public:

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<FrameRateKeep> Create();

	/// <summary>
	/// Fps�l�̌Œ�
	/// </summary>
	void FixedFps();

public:

	//�X�^�[�g����
	float deltaTime = 0.0f;
	float frameRate = 0.0f;
	float commandWaitTime = 0.0f;
	int count = 0;
	std::chrono::steady_clock::time_point lastUpdate;

};

