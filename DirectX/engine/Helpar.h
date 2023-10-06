#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

//���O����������
static LPCWSTR GetName(std::string _className, std::string _setName)
{
	//���O�̌���
	std::string name = _className + _setName;
	//�傫���擾
	size_t size = name.size();
	//���O�̃T�C�Y+1�̔z��ɍ�蒼��
	LPWSTR returnName = new wchar_t[size + 1];
	std::copy(name.begin(), name.end(), returnName);
	//�z��̖���0����
	returnName[size] = 0;

	return returnName;
}