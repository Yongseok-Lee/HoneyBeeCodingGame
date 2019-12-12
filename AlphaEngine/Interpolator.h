#pragma once
#include "Singleton.h"
#include "System.h"
#include <unordered_map>

// http://www.gizma.com/easing/#l

enum ease_function
{
	Linear,
	QuadIn,
	QuadOut,
	QuadInOut,
	CubicIn,
	CubicOut,
	CubicInOut,
	ExpoIn,
	ExpoOut,
	ExpoInOut,
	CircIn,
	CircOut,
	CircInOut,
	EaseNum
};

using EaseFunction = float(float, float, float, float);

struct Tween
{
	ease_function _ease;
	float* _value;
	float _duration;
	float _start;
	float _end;
	std::function<void(Tween)> _doneCallback;
	bool _isDone = false;
	long _startTime = 0;
};

class Interpolator : public System, public Singleton<Interpolator>
{
	std::unordered_map<ease_function, std::function<EaseFunction>> _easeFunctions;
	std::vector<Tween> _tweenList;

public:
	Interpolator();
	virtual ~Interpolator() = default;
	void Init() override;
	void Update() override;
	void Release() override;

	Tween* AddTween(Tween tween);
	void DeleteTween(Tween* tween);
};

