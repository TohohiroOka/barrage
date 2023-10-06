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

float GameHelper::Randomfloat(int _range)
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, _range);

	return float(rand100(mt));
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
	float Theta = float((Randomfloat(314 * 2) - 314)) / 100.0f;

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
		float Range = sqrtf(float(Randomfloat(1)));
		float Theta = float((Randomfloat(314 * 2) - 314)) / 100.0f;
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
	// â‘Î’l‚ğ‹‚ß‚é
	value = int(fabs(value));
	// nŒ…–ÚˆÈ‰º‚ÌŒ…‚ğæ“¾
	mod_value = fmod(value, pow(10, n + 1));
	// mŒ…–ÚˆÈã‚ÌŒ…‚ğæ“¾
	result = int(mod_value / pow(10, m));

	return result;
}