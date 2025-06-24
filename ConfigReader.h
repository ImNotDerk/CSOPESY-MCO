#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
typedef std::string String;

class ConfigReader
{
public:
	std::vector<String> readFileToVector(const String& filename);
	void setParams(const String& filename);
	void assignParams(int index);
	void testPrint(); // debug test to check

	static ConfigReader* getInstance();
	static void initialize();
	static void destroy();

	int getNumCPU();
	String getSchedulerToUse();
	int getQuantum();
	int getBatchProcessFreq();
	int getMinIns();
	int getMaxIns();
	int getDelays();

private:
	ConfigReader();
	~ConfigReader() = default;
	static ConfigReader* sharedInstance;
	std::vector<String> paramsList;
	int num_cpu;
	String scheduler_to_use;
	int quantum_cycles;
	int batch_process_freq;
	int min_ins;
	int max_ins;
	int delays;
};

