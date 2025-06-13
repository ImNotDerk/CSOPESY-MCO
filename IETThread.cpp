#include "IETThread.h"

void IETThread::startThread()
{
	//std::cout << "Thread running." << std::endl;
	std::thread(&IETThread::run, this).detach();
}

void IETThread::sleepThread(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}