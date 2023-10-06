#include "WindowApp.h"
#include "MainEngine.h"
#include "SafeDelete.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �E�B���h�E�T�C�Y
	const int window_width = 1500;// ����
	const int window_height = 830;// �c��

	//�Q�[����
	const wchar_t* gameName = L"DirectX";

	//�E�B���h�E������
	WindowApp* winApp = new WindowApp();
	winApp->Initialize(window_width, window_height, gameName);

	//�S�̂̏�����
	MainEngine* engine = new MainEngine();
	engine->Initialize();

	SYSTEMTIME StartTime;
	SYSTEMTIME NowTime;
	__int64 m_ProgressTime;

	while (true)
	{
		if (engine->Update()|| winApp->Update()) { break; }
		engine->Draw();

		//�t���[�����[�g�Ǘ�
		engine->FrameControl();

		while (1) {
			// ���ݎ���
			GetLocalTime(&NowTime);
			// �ϊ�
			FILETIME ftime1;
			FILETIME ftime2;
			SystemTimeToFileTime(&StartTime, &ftime1);
			SystemTimeToFileTime(&NowTime, &ftime2);
			// int64�ɃL���X�g
			__int64* nTime1 = (__int64*)&ftime1;
			__int64* nTime2 = (__int64*)&ftime2;
			// �o�ߕb
			m_ProgressTime = (*nTime2 - *nTime1);
			if (m_ProgressTime >= __int64(100000)) {
				break;
			}
		}

	}

	//�o�^����
	safe_delete(engine);
	winApp->Release();

	return 0;
}