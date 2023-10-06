#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>

class Csv
{
protected: // �G�C���A�X
// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:

	/// <summary>
	/// �}�b�v�̓ǂݍ���
	/// </summary>
	/// <param name="_fileName">�t�@�C����</param>
	/// <param name="_map"></param>
	static void LoadFile(const std::string& _fileName, std::vector<std::vector<int>> _map);

	/// <summary>
	/// �I�u�W�F�N�g���W�̔z��ԍ��̒l�̎擾
	/// </summary>
	/// <param name="_map">�񎟌��z��</param>
	/// <param name="_chipSize">����̑傫��</param>
	/// <param name="_position">�I�u�W�F�N�g���W</param>
	/// <returns></returns>
	static int GetChipNum(std::vector<std::vector<int>> _map, float _chipSize, const XMFLOAT2& _position);
};