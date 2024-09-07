#include "TimedCall.h"

TimedCall::TimedCall(std::function<void()> fire, uint32_t time) 
	: fire_(fire),time_(time){
}

void TimedCall::Update() {

	if (isFinished) {
		return;
	}

	time_--;
	if (time_ <= 0) {
		isFinished = true;
		fire_();
	}
}