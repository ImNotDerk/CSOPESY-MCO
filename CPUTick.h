#pragma once
class CPUTick
{
public:
	CPUTick();
	~CPUTick() = default;

    static CPUTick* getInstance();
	void resetTicks();
	void addTick(int ticks);
	int getTicks() const;

private:
    CPUTick& operator=(CPUTick const&) {};
    static CPUTick* sharedInstance;
	int nTicks;
};
