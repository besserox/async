#pragma once

#include <functional>
#include <future>


template<typename> class CallerSync;
template<typename> class CallerAsync;

//synchronous calls
template<typename Callback, typename... Args>
class CallerSync<Callback(Args...)> {
public:
	std::future<Callback> operator() (const std::function<Callback(Args...)> &callback, Args... args) {
		std::packaged_task<Callback(Args...)> task(callback);
		auto f = task.get_future();
		task(args...);
		return f;
	}
};
template<typename... Args>
class CallerSync<void(Args...)> {
public:
	std::future<int> operator() (const std::function<void(Args...)> &callback, Args... args) {
		auto closure = [&](Args... args)->int { callback(args...); return 0; };
		std::packaged_task<int(Args...)> task(closure);
		auto f = task.get_future();
		task(args...);
		return f;
	}
};

//asynchronous calls with std::launch::async (spawns a thread)
template<typename Callback, typename... Args>
class CallerAsync<Callback(Args...)> {
public:
	std::future<Callback> operator() (const std::function<Callback(Args...)> &callback, Args... args) {
		return std::async(std::launch::async, callback, args...);
	}
};
template<typename... Args>
class CallerAsync<void(Args...)> {
public:
	std::future<int> operator() (const std::function<void(Args...)> &callback, Args... args) {
		auto closure = [&](Args... args)->int { callback(args...); return 0; };
		return std::async(std::launch::async, closure, args...);
	}
};

