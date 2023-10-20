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
#include "../Math/Vector3.h"
#include "../Math/Vector2.h"
#include "../Math/DirectXMathHelper.h"

class GameHelper : public Singleton<GameHelper>
{
	friend class Singleton<GameHelper>;

public:
	GameHelper(){};
	~GameHelper(){};

	void Update();

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

/// <summary>
/// �����擾
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
static unsigned GetDigit(unsigned num) {
	return unsigned(std::to_string(num).length());
};

/// <summary>
/// ��_�Ԃ̊p�x�擾
/// </summary>
/// <param name="_startPoint">���W1</param>
/// <param name="_endPoint">���W2</param>
/// <returns></returns>
static float GetAngle(const DirectX::XMFLOAT2& _startPoint, const DirectX::XMFLOAT2& _endPoint) {
	float radian = atan2f(_endPoint.y - _startPoint.y, _endPoint.x - _startPoint.x);
	return DirectX::XMConvertToDegrees(radian);
}

/// <summary>
/// ��������
/// 0����͈͂܂ł̗������o��
/// </summary>
/// <param name="range">�͈�</param>
/// <returns>����</returns>
static int RandomInt(int _range) {
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, _range);

	return rand100(mt);
}

/// <summary>
/// ��������
/// 1����2�܂ł̗������o��
/// </summary>
/// <param name="range1">�͈�</param>
/// <param name="range2">�͈�</param>
/// <returns>����</returns>
static int RandomInt(int _range1, int _range2) {
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(_range1, _range2);

	return rand100(mt);
}

/// <summary>
/// ��������
/// 0����͈͂܂ł̗������o��
/// </summary>
/// <param name="range">�͈�</param>
/// <returns>����</returns>
static float RandomFloat(float _range) {
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, int(_range));

	return float(rand100(mt));
}

/// <summary>
/// ��������
/// 1����2�܂ł̗������o��
/// </summary>
/// <param name="range1">�͈�</param>
/// <param name="range2">�͈�</param>
/// <returns>����</returns>
static float RandomFloat(float _range1, float _range2) {
	std::random_device rnd;
	std::mt19937 mt(rnd());
	int range1 = int(_range1);
	int range2 = int(_range2);
	std::uniform_int_distribution<> rand100(range1, range2);

	return float(rand100(mt));
}

/// <summary>
/// ��������
/// 1��-1���o��
/// </summary>
/// <returns>1or-1</returns>
static int RandomSign() {
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, 1);

	int sign = rand100(mt);

	if (sign == 0)
	{
		return 1;
	} else
	{
		return -1;
	}
}

/// <summary>
/// �~��ɗ����𐶐��B�ψꉻ�L��
/// </summary>
/// <returns></returns>
static DirectX::XMFLOAT3 GetRandInUniformInCircle() {
	float Range = 1.0f;
	float Theta = float((RandomInt(314 * 2) - 314)) / 100.0f;

	DirectX::XMFLOAT3 result;
	result.x = Range * cosf(Theta);
	result.y = Range * sinf(Theta);
	result.z = 0;

	return result;
}

/// <summary>
/// �~��ɗ����𐶐��B�ψꉻ�L��
/// </summary>
/// <returns></returns>
static DirectX::XMFLOAT3 GetHalfRandInUniformInCircle() {
	DirectX::XMFLOAT3 result = { 0,0,0 };

	while (result.x > 0)
	{
		float Range = sqrtf(float(RandomInt(1)));
		float Theta = float((RandomInt(314 * 2) - 314)) / 100.0f;
		DirectX::XMFLOAT3 result;

		result.x = Range * cosf(Theta);
		result.y = Range * sinf(Theta);
		result.z = 0;
	}

	return result;
}

/// <summary>
/// value��m���ڂ���n���ڂ��擾����
/// </summary>
/// <param name="value">�擾��ƂȂ镂�������_��</param>
/// <param name="m">�擾���J�n���錅</param>
/// <param name="n">�擾���I�����錅</param>
/// <returns></returns>
static int GetDigits(int value, int m, int n) {
	double mod_value;
	int result;
	// ��Βl�����߂�
	value = int(fabs(value));
	// n���ڈȉ��̌����擾
	mod_value = fmod(value, pow(10, n + 1));
	// m���ڈȏ�̌����擾
	result = int(mod_value / pow(10, m));

	return result;
}

/// <summary>
/// ���`���float
/// </summary>
/// <param name="points">����_�̏W��</param>
/// <param name="startIndex">�ۊǂ����Ԃ̓Y��</param>
/// <param name="t">���Ԍo��</param>
/// <returns></returns>
static float SplinePosition(const std::vector<float>& points, int startIndex, float t) {
	std::array<int, 4> index;
	if (startIndex < 1) {
		index[0] = int(points.size()) - 1;
	} else {
		index[0] = startIndex - 1;
	}
	index[1] = startIndex;
	if (startIndex >= int(points.size()) - 1) {
		index[2] = 0;
		index[3] = 1;
	} else if (startIndex >= int(points.size()) - 2) {
		index[2] = startIndex + 1;
		index[3] = 0;
	} else {
		index[2] = startIndex + 1;
		index[3] = startIndex + 2;
	}

	//p0~p3�̐���_���擾���遦p1~p2���Ԃ���
	float p0 = points[index[0]];
	float p1 = points[index[1]];
	float p2 = points[index[2]];
	float p3 = points[index[3]];

	//Catmull-Rom�̎��ɂ����
	float position = (p1 * 2.0f + (-p0 + p2) * t
		+ (p0 * 2.0f - p1 * 5.0f + p2 * 4.0f - p3) * t * t
		+ (-p0 + p1 * 3.0f - p2 * 3.0f + p3) * t * t * t) * 0.5f;

	return position;
}

/// <summary>
/// ���`���Vector2
/// </summary>
/// <param name="points">����_�̏W��</param>
/// <param name="startIndex">�ۊǂ����Ԃ̓Y��</param>
/// <param name="t">���Ԍo��</param>
/// <returns></returns>
static Vector2 SplinePosition(const std::vector<Vector2>& points, int startIndex, float t) {
	std::array<int, 4> index;
	if (startIndex < 1) {
		index[0] = int(points.size()) - 1;
	} else {
		index[0] = startIndex - 1;
	}
	index[1] = startIndex;
	if (startIndex >= int(points.size()) - 1) {
		index[2] = 0;
		index[3] = 1;
	} else if (startIndex >= int(points.size()) - 2) {
		index[2] = startIndex + 1;
		index[3] = 0;
	} else {
		index[2] = startIndex + 1;
		index[3] = startIndex + 2;
	}

	//p0~p3�̐���_���擾���遦p1~p2���Ԃ���
	Vector2 p0 = points[index[0]];
	Vector2 p1 = points[index[1]];
	Vector2 p2 = points[index[2]];
	Vector2 p3 = points[index[3]];

	//Catmull-Rom�̎��ɂ����
	Vector2 position = (p1 * 2 + (-p0 + p2) * t
		+ (p0 * 2 - p1 * 5 + p2 * 4 - p3) * t * t
		+ (-p0 + p1 * 3 - p2 * 3 + p3) * t * t * t) * 0.5;

	return position;
}

/// <summary>
/// ���`���Vector3
/// </summary>
/// <param name="points">����_�̏W��</param>
/// <param name="startIndex">�ۊǂ����Ԃ̓Y��</param>
/// <param name="t">���Ԍo��</param>
/// <returns></returns>
static Vector3 SplinePosition(const std::vector<Vector3>& points, int startIndex, float t) {
	std::array<int, 4> index;
	if (startIndex < 1) {
		index[0] = int(points.size()) - 1;
	} else {
		index[0] = startIndex - 1;
	}
	index[1] = startIndex;
	if (startIndex >= int(points.size()) - 1) {
		index[2] = 0;
		index[3] = 1;
	} else if (startIndex >= int(points.size()) - 2) {
		index[2] = startIndex + 1;
		index[3] = 0;
	} else {
		index[2] = startIndex + 1;
		index[3] = startIndex + 2;
	}

	//p0~p3�̐���_���擾���遦p1~p2���Ԃ���
	Vector3 p0 = points[index[0]];
	Vector3 p1 = points[index[1]];
	Vector3 p2 = points[index[2]];
	Vector3 p3 = points[index[3]];

	//Catmull-Rom�̎��ɂ����
	Vector3 position = (p1 * 2 + (-p0 + p2) * t
		+ (p0 * 2 - p1 * 5 + p2 * 4 - p3) * t * t
		+ (-p0 + p1 * 3 - p2 * 3 + p3) * t * t * t) * 0.5;

	return position;
}

/// <summary>
/// �����̊p�x�擾
/// </summary>
/// <param name="vec">���x�x�N�g��</param>
/// <param name="isMinusYRotaFix">y���p�x�����̐��̏ꍇ���̐��ɏC�����邩</param>
/// <returns>�����̊p�x</returns>
static const Vector3 VelocityRotate(const Vector3& vec, const bool isMinusYRotaFix = false) {
	using namespace DirectX;
	Vector3 rota = {};
	rota.y = XMConvertToDegrees(std::atan2(vec.x, vec.z));
	//Y���p�x�����̐��Ȃ̂��C������ꍇ
	if (isMinusYRotaFix) {
		//���̐��Ȃ�Ίp�x�C��
		if (rota.y <= 0) {
			rota.y += 360;
		}
	}
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rota.y));
	Vector3 distanceVecZ = MatrixTransformDirection(vec, matRot);
	rota.x = XMConvertToDegrees(std::atan2(-distanceVecZ.y, distanceVecZ.z));

	return rota;
}