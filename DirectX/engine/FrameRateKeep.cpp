#include "FrameRateKeep.h"
#include "WindowApp.h"

std::unique_ptr<FrameRateKeep> FrameRateKeep::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	FrameRateKeep* instance = new FrameRateKeep();
	if (instance == nullptr) {
		return nullptr;
	}

	return std::unique_ptr<FrameRateKeep>(instance);
}

void FrameRateKeep::FixedFps()
{
	// �o�ߎ��Ԍv��
	auto now = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.0f;
	frameRate = 1.0f / deltaTime;
	lastUpdate = now;

	// FPS,CPU�g�p���\��
	{
		//const float fps_basis = 60.0f;
		//// ��b�Ɉ�x�X�V
		//if (++count > fps_basis) {
		//	count = 0;
		//	float cputime = deltaTime - commandWaitTime;
		//	char str[50];
		//	sprintf_s(str, 50, "fps=%03.0f cpu usage=%06.2f%%", frameRate, cputime * fps_basis * 100.0f);
		//	SetWindowTextA(WindowApp::GetHwnd(), str);
		//}
	}
}