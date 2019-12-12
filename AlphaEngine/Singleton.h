#pragma once
#include <mutex>
#include <memory>

template <typename T>
class Singleton
{
	static std::unique_ptr<T> _instance;
	static std::once_flag _flag;

protected:
	Singleton() = default;

public:
	~Singleton() = default;
	Singleton(const Singleton &) = delete;
	Singleton(Singleton &&) = delete;
	Singleton &operator=(const Singleton &) = delete;
	Singleton &operator=(Singleton &&) = delete;
	static T* GetInstance();

};

template <typename T>
std::unique_ptr<T> Singleton<T>::_instance = nullptr;

template <typename T>
std::once_flag Singleton<T>::_flag;

template <typename T>
T* Singleton<T>::GetInstance()
{
	std::call_once(_flag, []() {
		_instance.reset(new T);
	});

	return _instance.get();
}