#include "ConfigReader.h"

ConfigReader* ConfigReader::getInstance()
{
    return sharedInstance;
}

void ConfigReader::initialize()
{
    if (sharedInstance == nullptr)
    {
        sharedInstance = new ConfigReader();
    }
}

void ConfigReader::destroy()
{
    if (sharedInstance != nullptr)
    {
        delete sharedInstance;
        sharedInstance = nullptr;
    }
}

std::vector<String> ConfigReader::readFileToVector(const String& filename) {
    std::vector<String> lineVector;  
    std::ifstream file(filename);  

	if (!file) {  // Check wheter file is opened successfully
        std::cerr << "Error: File " << filename << " can not be opened." << std::endl;
        return lineVector;
	}

    String line;  
    while (std::getline(file, line)) { 
        std::istringstream iss(line);
		String name, val;
        if (iss >> name >> val) {
            lineVector.push_back(val); // Store value only to the vector
        }
    }  

    file.close();  
    return lineVector;
}

void ConfigReader::assignParams(int index) {
    try {
        if (index == 0) {
            this->num_cpu = std::stoi(paramsList[0]); // stoi converts String type read from Config converted to int data type
        }
        else if(index == 1) {
            this->scheduler_to_use = paramsList[1];
        }
        else if (index == 2) {
            this->quantum_cycles = std::stoi(paramsList[2]);
        }
        else if (index == 3) {
            this->batch_process_freq = std::stoi(paramsList[3]);
        }
        else if (index == 4) {
            this->min_ins = std::stoi(paramsList[4]);
        }
        else if (index == 5) {
            this->max_ins = std::stoi(paramsList[5]);
        }
        else if (index == 6) {
            this->delays = std::stoi(paramsList[6]);
        }
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Out of range: " << paramsList[index] << " found in index " << index << std::endl;
    }
    catch (const std::invalid_argument& e){
        std::cerr << "Invalid argument/Type mismatched: " << paramsList[index] << " found in index " << index << std::endl;
    }
}

void ConfigReader::setParams(const String& filename) {
	this->paramsList = readFileToVector(filename); // Read the parameter values from a file and store it into each desginated parameter
    for (int i = 0; i < paramsList.size(); ++i) {
        assignParams(i);
	}
}

void ConfigReader::testPrint() { // debug test to check if working
    std::cout << "num_cpu = " << getNumCPU() << std::endl;
    std::cout << "scheduler_to_use = " << getSchedulerToUse() << std::endl;
    std::cout << "quantum_cycles = " << getQuantum() << std::endl;
    std::cout << "batch_process_freq = " << getBatchProcessFreq() << std::endl;
    std::cout << "min_ins = " << getMinIns() << std::endl;
    std::cout << "max_ins = " << getMaxIns() << std::endl;
    std::cout << "delays = " << getDelays() << std::endl;
}

int ConfigReader::getNumCPU() {
    return this->num_cpu;
}

String ConfigReader::getSchedulerToUse() {
    return this->scheduler_to_use;
}

int ConfigReader::getQuantum() {
    return this->quantum_cycles;
}

int ConfigReader::getBatchProcessFreq() {
    return this->batch_process_freq;
}

int ConfigReader::getMinIns() {
    return this->min_ins;
}

int ConfigReader::getMaxIns() {
    return this->max_ins;
}

int ConfigReader::getDelays() {
    return this->delays;
}