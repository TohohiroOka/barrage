#include "GameHelper.h"

void GameHelper::Update()
{
	if (slowTimer == 0) { return; }
	slowTimer--;
	if (slowTimer != 0) { return; }
	gameSpeed = 1;
}

unsigned GameHelper::GetDigit(unsigned num) {
	return unsigned(std::to_string(num).length());
}

float GameHelper::GetAngle(const DirectX::XMFLOAT2& _startPoint, const DirectX::XMFLOAT2& _endPoint)
{
	float radian = atan2f(_endPoint.y - _startPoint.y, _endPoint.x - _startPoint.x);
	return DirectX::XMConvertToDegrees(radian);
}

int GameHelper::RandomInt(int _range)
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, _range);

	return rand100(mt);
}

int GameHelper::RandomSign()
{
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

DirectX::XMFLOAT3 GameHelper::GetRandInUniformInCircle()
{
	float Range = 1.0f;
	float Theta = float((RandomInt(314 * 2) - 314)) / 100.0f;

	DirectX::XMFLOAT3 result;
	result.x = Range * cosf(Theta);
	result.y = Range * sinf(Theta);
	result.z = 0;

	return result;
}

DirectX::XMFLOAT3 GameHelper::GetHalfRandInUniformInCircle()
{
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

int GameHelper::GetDigits(int value, int m, int n) {
	double mod_value;
	int result;
	// 絶対値を求める
	value = int(fabs(value));
	// n桁目以下の桁を取得
	mod_value = fmod(value, pow(10, n + 1));
	// m桁目以上の桁を取得
	result = int(mod_value / pow(10, m));

	return result;
}

float GameHelper::SplinePosition(const std::vector<float>& points, int startIndex, float t)
{
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

	//p0~p3の制御点を取得する※p1~p2を補間する
	float p0 = points[index[0]];
	float p1 = points[index[1]];
	float p2 = points[index[2]];
	float p3 = points[index[3]];

	//Catmull-Romの式による補間
	float position = (p1 * 2.0f + (-p0 + p2) * t
		+ (p0 * 2.0f - p1 * 5.0f + p2 * 4.0f - p3) * t * t
		+ (-p0 + p1 * 3.0f - p2 * 3.0f + p3) * t * t * t) * 0.5f;

	return position;
}

Vector2 GameHelper::SplinePosition(const std::vector<Vector2>& points, int startIndex, float t)
{
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

	//p0~p3の制御点を取得する※p1~p2を補間する
	Vector2 p0 = points[index[0]];
	Vector2 p1 = points[index[1]];
	Vector2 p2 = points[index[2]];
	Vector2 p3 = points[index[3]];

	//Catmull-Romの式による補間
	Vector2 position = (p1 * 2 + (-p0 + p2) * t
		+ (p0 * 2 - p1 * 5 + p2 * 4 - p3) * t * t
		+ (-p0 + p1 * 3 - p2 * 3 + p3) * t * t * t) * 0.5;

	return position;
}

Vector3 GameHelper::SplinePosition(const std::vector<Vector3>& points, int startIndex, float t)
{
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

	//p0~p3の制御点を取得する※p1~p2を補間する
	Vector3 p0 = points[index[0]];
	Vector3 p1 = points[index[1]];
	Vector3 p2 = points[index[2]];
	Vector3 p3 = points[index[3]];

	//Catmull-Romの式による補間
	Vector3 position = (p1 * 2 + (-p0 + p2) * t
		+ (p0 * 2 - p1 * 5 + p2 * 4 - p3) * t * t
		+ (-p0 + p1 * 3 - p2 * 3 + p3) * t * t * t) * 0.5;

	return position;
}