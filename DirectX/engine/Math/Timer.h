#pragma once
#include "GameHelper.h"

namespace Engine
{
	class Timer {
		public:
			Timer() { Reset(); }
			~Timer() {};

			void Update() {
				if (!isMove) { return; }
				float gameSpeed = GameHelper::Instance()->GetGameSpeed();
				if (gameSpeed != 0) {
					time += 1.0f / gameSpeed;
				} else { return; }
			}

			void Reset() {
				isMove = true;
				time = 0.0f;
			}

			void SetIsMove(bool _isMove) {
				isMove = _isMove;
			}

			float GetTime() { return time; }

			bool isMove = true;
			float time = 0.0f;

			void operator=(const int _num) { time = float(_num); }
			void operator=(const float _num) { time = _num; }
			void operator+=(const int _num) { time += float(_num); }
			void operator+=(const float _num) { time += _num; }
			void operator-=(const int _num) { time -= float(_num); }
			void operator-=(const float _num) { time -= _num; }
			int operator+(const int _num) { return int(time) + _num; }
			float operator+(const float _num) { return time + _num; }
			int operator-(const int _num) { return int(time) - int(_num); }
			float operator-(const float _num) { return time - _num; }
			int operator%(const int _num) { return int(time) % _num; }
			float operator%(const float _num) { return float(int(time) % int(_num)); }
			int operator/(const int _num) { return int(time) / _num; }
			float operator/(const float _num) { return time / _num; }
			bool operator>(const int _num) { return time > _num; }
			bool operator>(const float _num) { return time > _num; }
			bool operator<(const int _num) { return time < _num; }
			bool operator<(const float _num) { return time < _num; }
			bool operator>=(const int _num) { return time >= _num; }
			bool operator>=(const float _num) { return time >= _num; }
			bool operator<=(const int _num) { return time <= _num; }
			bool operator<=(const float _num) { return time <= _num; }
			bool operator!=(const int _num) { return time != _num; }
			bool operator!=(const float _num) { return time != _num; }
			bool operator==(const int _num) { return time == _num; }
			bool operator==(const float _num) { return time == _num; }
	};
}