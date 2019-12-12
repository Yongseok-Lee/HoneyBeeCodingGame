#include "Interpolator.h"
#include <algorithm>

Interpolator::Interpolator()
{
	/// t : current time
	/// b : start value
	/// c : end value
	/// d : duration
	_easeFunctions = {
		{ Linear, [](float t, float b, float c, float d) -> float
			{
				return c * t / d + b;
			}
		},
		{ QuadIn, [](float t, float b, float c, float d) -> float
			{
				t /= d;
				return c * t*t + b;
			}
		},
		{ QuadOut, [](float t, float b, float c, float d) -> float
			{
				t /= d;
				return -c * t*(t - 2) + b;
			}
		},
		{ QuadInOut, [](float t, float b, float c, float d) -> float
			{
				t /= d / 2.0f;
				if (t < 1) return c / 2 * t*t + b;
				t--;
				return -c / 2 * (t*(t - 2) - 1) + b;
			}
		},
		{ CubicIn, [](float t, float b, float c, float d) -> float
			{
				t /= d;
				return c * t*t*t + b;
			}
		},
		{ CubicOut,	[](float t, float b, float c, float d) -> float
			{
				t /= d;
				t--;
				return c * (t*t*t + 1) + b;
			}
		},
		{ CubicInOut, [](float t, float b, float c, float d) -> float
			{
				t /= d / 2;
				if (t < 1) return c / 2 * t*t*t + b;
				t -= 2;
				return c / 2 * (t*t*t + 2) + b;
			}
		},
		{ ExpoIn, [](float t, float b, float c, float d) -> float
			{
				return c * std::pow(2, 10 * (t / d - 1)) + b;
			}
		},
		{ ExpoOut, [](float t, float b, float c, float d) -> float
			{
				return c * (-std::pow(2, -10 * t / d) + 1) + b;
			}
		},
		{ ExpoInOut, [](float t, float b, float c, float d) -> float
			{
				t /= d / 2;
				if (t < 1) return c / 2 * std::pow(2, 10 * (t - 1)) + b;
				t--;
				return c / 2 * (-std::pow(2, -10 * t) + 2) + b;
			}
		},
		{ CircIn, [](float t, float b, float c, float d) -> float
			{
				t /= d;
				return -c * (std::sqrt(1 - t * t) - 1) + b;
			}
		},
		{ CircOut, [](float t, float b, float c, float d) -> float
			{
				t /= d;
				t--;
				return c * std::sqrt(1.0f - t * t) + b;
			}
		},
		{ CircInOut, [](float t, float b, float c, float d) -> float
			{
				t /= d / 2;
				if (t < 1) return -c / 2 * (std::sqrt(1 - t * t) - 1) + b;
				t -= 2;
				return c / 2 * (std::sqrt(1 - t * t) + 1) + b;
			}
		}
	};
}

void Interpolator::Init()
{
}

void Interpolator::Update()
{
	_tweenList.erase(std::remove_if(_tweenList.begin(), _tweenList.end(), [](Tween& t)
	{
		return t._isDone;
	}), _tweenList.end());

	std::for_each(_tweenList.begin(), _tweenList.end(), [&](Tween& t)
	{
		clock_t currentTime = clock();
		*t._value = _easeFunctions[t._ease]((currentTime - t._startTime) / 1000.0f, t._start, t._end, t._duration);
		if ((currentTime - t._startTime) / 1000.0f >= t._duration)
		{
			t._isDone = true;
			*t._value = t._end;
		}
	});

	std::for_each(_tweenList.begin(), _tweenList.end(), [](Tween& t)
	{
		if (t._isDone)
			t._doneCallback(t);
	});
}

void Interpolator::Release()
{
	_tweenList.clear();
}

Tween* Interpolator::AddTween(Tween tween)
{
	tween._isDone = false;
	tween._startTime = clock();

	_tweenList.push_back(tween);
	return &_tweenList.back();
}

void Interpolator::DeleteTween(Tween* tween)
{
}
