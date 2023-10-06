#pragma once
#include <Windows.h>

class WindowApp
{
public:
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	WindowApp() {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~WindowApp() {};

	/// <summary>
	/// �E�B���h�E�v���\�W���̐���
	/// </summary>
	/// <param name="_hwnd"></param>
	/// <param name="_msg"></param>
	/// <param name="_wparam"></param>
	/// <param name="_lparam"></param>
	/// <returns>�E�B���h�E�v���\�W��</returns>
	static LRESULT WindowProcdure(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_windowWidth">��ʕ�</param>
	/// <param name="_windowHeight">��ʍ���</param>
	/// <param name="_gameName">�^�C�g���o�[��</param>
	void Initialize(int _windowWidth, int _windowHeight, const wchar_t* _gameName);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <returns>�A�v���P�[�V�������I�����邩�ۂ�</returns>
	bool Update();

	/// <summary>
	/// �o�^����
	/// </summary>
	void Release();

	/// <summary>
	/// �E�B���h�E�I�u�W�F�N�g�̎擾
	/// </summary>
	/// <returns>hwnd</returns>
	static HWND GetHwnd() { return hwnd; }

	/// <summary>
	/// �E�B���h�E�N���X�ݒ�̎擾
	/// </summary>
	/// <returns>hInstance</returns>
	static HINSTANCE GetWinInstance() { return winClass.hInstance; }

	/// <summary>
	/// �����̎擾
	/// </summary>
	/// <returns>windowWidth</returns>
	static UINT GetWindowWidth() { return windowWidth; }

	/// <summary>
	/// �c���̎擾
	/// </summary>
	/// <returns>windowHeight</returns>
	static UINT GetWindowHeight() { return windowHeight; }

	/// <summary>
	/// �c���̎擾
	/// </summary>
	/// <returns>windowHeight</returns>
	static bool GetIsWindowResize() { return isWindowResize; }
private:

	//���b�Z�[�W
	MSG msg = {};
	//����
	static UINT windowWidth;
	//����
	static UINT windowHeight;
	//�E�B���h�E�N���X�̐ݒ�
	static WNDCLASSEX winClass;
	//�E�B���h�E�I�u�W�F�N�g
	static HWND hwnd;
	//�E�B���h�E�T�C�Y�ύX�t���O
	static bool isWindowResize;
};

