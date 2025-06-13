#pragma once
#include <iostream>
#include <thread>

class IETThread {
public:
	IETThread() = default;
	~IETThread() = default;

	void startThread();
	static void sleepThread(int ms);

private:
	virtual void run() = 0;
};

