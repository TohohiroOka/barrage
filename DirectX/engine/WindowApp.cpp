#include "WindowApp.h"
#include <imgui_impl_win32.h>

UINT WindowApp::windowWidth;
UINT WindowApp::windowHeight;
WNDCLASSEX WindowApp::winClass{};
HWND WindowApp::hwnd = nullptr;
bool WindowApp::isWindowResize;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT WindowApp::WindowProcdure(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam)
{
	//if (_msg == WM_SIZE) {
	//	//�E�B���h�E�T�C�Y�擾
	//	RECT window_rect;
	//	GetWindowRect(_hwnd, &window_rect);

	//	////�N���C�A���g�̈�擾
	//	//RECT client_rect;
	//	//GetClientRect(_hwnd, &client_rect);

	//	// �t���[���T�C�Y�Z�o
	//	int frame_size_x = (window_rect.right - window_rect.left);// - (client_rect.right - client_rect.left);
	//	int frame_size_y = (window_rect.bottom - window_rect.top);// - (client_rect.bottom - client_rect.top);

	//	window_rect = { 0,0,frame_size_x ,frame_size_y };

	//	//// ���T�C�Y�p�T�C�Y�̎Z�o
	//	//int resize_width = frame_size_x + windowWidth;
	//	//int resize_height = frame_size_y + windowHeight;

	//	windowWidth = frame_size_x;
	//	windowHeight = frame_size_y;

	//	AdjustWindowRect(&window_rect, WS_VSCROLL, false);// �����ŃT�C�Y�␳

	//	//SetWindowPos(
	//	//	// �E�B���h�E�n���h��
	//	//	_hwnd,
	//	//	// �z�u�����̃n���h��(NULL�ł悵)
	//	//	NULL,
	//	//	// �\�����WX
	//	//	CW_USEDEFAULT,
	//	//	// �\�����WY
	//	//	CW_USEDEFAULT,
	//	//	// ���T�C�Y����
	//	//	frame_size_x,
	//	//	// ���T�C�Y�c��
	//	//	frame_size_y,
	//	//	// SWP_NOMOVE => �ʒu�ύX�Ȃ�
	//	//	SWP_NOMOVE);

	//	// �E�B���h�E�\��
	//	//ShowWindow(hwnd, SW_SHOW);

	//	isWindowResize = true;
	//}

	if (_msg == WM_DESTROY)
	{
		PostQuitMessage((0));
		return 0;
	}
	ImGui_ImplWin32_WndProcHandler(_hwnd, _msg, _wparam, _lparam);

	return DefWindowProc(_hwnd, _msg, _wparam, _lparam);
}

void WindowApp::Initialize(int _windowWidth, int _windowHeight,const wchar_t* _gameName)
{
	windowWidth = _windowWidth;
	windowHeight = _windowHeight;

	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.lpfnWndProc = (WNDPROC)WindowProcdure;// �E�B���h�E�v���V�[�W����ݒ�
	winClass.lpszClassName = _gameName;// �E�B���h�E�N���X��
	winClass.hInstance = GetModuleHandle(nullptr);// �E�B���h�E�n���h��
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);// �J�[�\���w��

	// �E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&winClass);
	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = { 0, 0, (LONG)windowWidth, (LONG)windowHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);// �����ŃT�C�Y�␳

	// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(winClass.lpszClassName,// �N���X��
		_gameName,// �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,// �W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,// �\��X���W�iOS�ɔC����j
		CW_USEDEFAULT,// �\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left,// �E�B���h�E����
		wrc.bottom - wrc.top,// �E�B���h�E�c��
		nullptr,// �e�E�B���h�E�n���h��
		nullptr,// ���j���[�n���h��

		winClass.hInstance,// �Ăяo���A�v���P�[�V�����n���h��
		nullptr);// �I�v�V����

	// �E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

bool WindowApp::Update()
{
	isWindowResize = false;
	//���b�Z�[�W����
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//�A�v���P�[�V�������I��鎞��message��WM_QUIT�ɂȂ�
	if (msg.message == WM_QUIT)
	{
		return true;
	}
	else { return false; }
}

void WindowApp::Release()
{
	UnregisterClass(winClass.lpszClassName, winClass.hInstance);
}