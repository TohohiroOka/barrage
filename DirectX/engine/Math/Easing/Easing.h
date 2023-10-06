#pragma once
#include <DirectXMath.h>

class Easing
{
private: // �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:
	//���������^��
	static float Lerp(const float _start, const float _end, const float _time);
	static XMFLOAT3 Lerp(const XMFLOAT3& _start, const XMFLOAT3& _end, const float _time);

	//sin
	static float InSine(float _start, float _end, float _time);
	static float OutSine(float _start, float _end, float _time);
	static float InOutSine(float _start, float _end, float _time);

	//2��
	static float InQuad(float _start, float _end, float _time);
	static float OutQuad(float _start, float _end, float _time);
	static float InOutQuad(float _start, float _end, float _time);

	//3��
	static float InCubic(float _start, float _end, float _time);
	static float OutCubic(float _start, float _end, float _time);
	static float InOutCubic(float _start, float _end, float _time);

	//4��
	static float InQuart(float _start, float _end, float _time);
	static float OutQuart(float _start, float _end, float _time);
	static float InOutQuart(float _start, float _end, float _time);

	//5��
	static float InQuint(float _start, float _end, float _time);
	static float OutQuint(float _start, float _end, float _time);
	static float InOutQuint(float _start, float _end, float _time);

	//Expo
	static float InExpo(float _start, float _end, float _time);
	static float OutExpo(float _start, float _end, float _time);
	static float InOutExpo(float _start, float _end, float _time);

	//Circ
	static float InCirc(float _start, float _end, float _time);
	static float OutCirc(float _start, float _end, float _time);
	static float InOutCirc(float _start, float _end, float _time);

	//Back
	static float InBack(float _start, float _end, float _time);
	static float OutBack(float _start, float _end, float _time);
	static float InOutBack(float _start, float _end, float _time);

	//Elastic
	static float InElastic(float _start, float _end, float _time);
	static float OutElastic(float _start, float _end, float _time);
	static float InOutElastic(float _start, float _end, float _time);

	//Bounce
	static float InBounce(float _start, float _end, float _time);
	static float OutBounce(float _start, float _end, float _time);
	static float InOutBounce(float _start, float _end, float _time);

private:
	/// <summary>
	/// Bounce�v�Z�p
	/// </summary>
	/// <param name="time">�C�[�W���O����(0�`1)</param>
	/// <returns>�C�[�W���O�v�Z����</returns>
	static float OutBounceCalculate(float& _time);
};