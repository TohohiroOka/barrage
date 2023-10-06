#pragma once
#include "Singleton.h"
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <memory>
#include <random>
#include <cstdlib>

class GameHelper : public Singleton<GameHelper>
{
	friend class Singleton<GameHelper>;

public:
	GameHelper(){};
	~GameHelper(){};

	void Update();

	/// <summary>
	/// �����擾
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	unsigned GetDigit(unsigned num);

	/// <summary>
	/// ��_�Ԃ̊p�x�擾
	/// </summary>
	/// <param name="_startPoint">���W1</param>
	/// <param name="_endPoint">���W2</param>
	/// <returns></returns>
	float GetAngle(const DirectX::XMFLOAT2& _startPoint, const DirectX::XMFLOAT2& _endPoint);

	/// <summary>
	/// ��������
	/// 0����͈͂܂ł̗������o��
	/// </summary>
	/// <param name="range">�͈�</param>
	/// <returns>����</returns>
	float Randomfloat(int _range);

	/// <summary>
	/// ��������
	/// 1��-1���o��
	/// </summary>
	/// <returns>1or-1</returns>
	int RandomSign();

	/// <summary>
	/// �~��ɗ����𐶐��B�ψꉻ�L��
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT3 GetRandInUniformInCircle();

	/// <summary>
	/// �~��ɗ����𐶐��B�ψꉻ�L��
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT3 GetHalfRandInUniformInCircle();

	/// <summary>
	/// value��m���ڂ���n���ڂ��擾����
	/// </summary>
	/// <param name="value">�擾��ƂȂ镂�������_��</param>
	/// <param name="m">�擾���J�n���錅</param>
	/// <param name="n">�擾���I�����錅</param>
	/// <returns></returns>
	int GetDigits(int value, int m, int n);

	//�Q�[�����x��ύX����
	void SetGameSpeed(float _gameSpeed) { gameSpeed = _gameSpeed; }

	/// <summary>
	/// �X���E
	/// </summary>
	/// <param name="_gameSpeed">�Q�[�����x</param>
	/// <param name="_timer">��������</param>
	void SetSlow(float _gameSpeed, float _timer) {
		gameSpeed = _gameSpeed;
		slowTimer = _timer;
	}

	const float mapSize = 255.0f * 10;
	//�Q�[�����x
	float gameSpeed = 1;
	//�X���E���������t���[���s����
	float slowTimer;

};