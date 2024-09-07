#pragma once
#include <functional>

class TimedCall {

public:

	TimedCall(std::function<void()> fire, uint32_t time);

	void Update();

	bool IsFinished() { return this->isFinished; }

private:

	std::function<void()> fire_;

	uint32_t time_;

	bool isFinished = false;

};
