#pragma once

#include <functional>
#include <future>


template<typename, typename> class CallerSync;
template<typename, typename> class CallerAsync;

template<typename Result, typename Callback, typename... Args>
class CallerSync<Result, Callback(Args...)> {
public:
	static std::future<Callback> call(Result &result, const std::function<Callback(Args...)> &callback, Args... args) {
		std::packaged_task<Callback(Args...)> task(callback);
		auto f = task.get_future();
		task(args...);
		return f;
	}
};

template<typename Result, typename Callback, typename... Args>
class CallerAsync<Result, Callback(Args...)> {
public:
	static std::future<Callback> call(Result &result, const std::function<Callback(Args...)> &callback, Args... args) {
		return std::async(std::launch::async, callback, args...);
	}
};
