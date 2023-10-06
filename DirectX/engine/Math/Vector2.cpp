#include "Vector2.h"
#include <cmath>

//�R���X�g���N�^
Vector2::Vector2()
	:x(0), y(0)
{
}


// �l��p���Ă̏�����
Vector2::Vector2(float x, float y)
	:x(x), y(y)
{
}

// �x�N�g���̑傫��
float Vector2::length() const {
	return float(sqrt(x * x + y * y));
}

// �x�N�g���̐��K��
Vector2& Vector2::normalize() {
	float len = length();
	if (len != 0) {
		*this /= len;
	}
	return *this;
}

// �x�N�g���̓���
float Vector2::dot(const Vector2& v) const {
	return x * v.x + y * v.y;
}

//�x�N�g���̊O��
float Vector2::cross(const Vector2& v) const {
	return x * v.x - y * v.y;
}

//�P�����Z�q+�̒ǉ�
Vector2 Vector2::operator+() const {
	return *this;
}

//�P�����Z�q+�̒ǉ�
Vector2 Vector2::operator-() const {
	return Vector2(-x, -y);
}

//+=�̒ǉ�
Vector2& Vector2:: operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
	return *this;
}

//-=�̒ǉ�
Vector2& Vector2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

//*=�̒ǉ�
Vector2& Vector2::operator*=(float s) {
	x *= s;
	y *= s;
	return *this;
}

// /=�̒ǉ�
Vector2& Vector2::operator/=(float s) {
	x /= s;
	y /= s;
	return *this;
}

// �x�N�g���̉��Z
const Vector2 operator+(const Vector2& v1, const Vector2& v2) {
	Vector2 temp(v1);
	return temp += v2;
}

// �x�N�g���̌��Z
const Vector2 operator-(const Vector2& v1, const Vector2& v2) {
	Vector2 temp(v1);
	return temp -= v2;
}

const Vector2 operator*(const Vector2& v, float s) {
	Vector2 temp(v);
	return temp *= s;
}

//�x�N�g���̏�Z
const Vector2 operator*(float s, const Vector2& v) {
	return v * s;
}

// �x�N�g���̏��Z
const Vector2 operator/(const Vector2& v, float s) {
	Vector2 temp(v);
	return temp /= s;
}