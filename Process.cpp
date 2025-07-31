#include "Process.h"

Process::Process(int pid, String name) {
	this->pid = pid;
	this->name = name;
	this->memorySize = 0;
	this->commandCounter = 0;
	this->cpuCoreID = -1; // default value
	this->symbolTable = std::make_shared<Symbol_Table>();
	this->generateRandomCommands();
	this->currentState = Process::ProcessState::READY;
}

Process::Process(int pid, String name, int memorySize) {
	this->pid = pid;
	this->name = name;
	this->memorySize = memorySize;
	this->commandCounter = 0;
	this->cpuCoreID = -1; // default value
	this->symbolTable = std::make_shared<Symbol_Table>();
	this->generateRandomCommands();
	this->currentState = Process::ProcessState::READY;
}

// called by CPU core worker
void Process::executeCurrentCommand(int coreId)
{
	if (this->commandCounter < commandList.size()) {

		commandList[commandCounter]->execute();  // run instruction
		this->logInstruction(coreId, commandList[commandCounter]->getOutput());
		this->commandCounter++; // count this single command;
	}
}

bool Process::isFinished() const {
	return this->commandCounter >= this->commandList.size();
}

int Process::incrementCommandCounter()
{
	return this->commandCounter++;
}

int Process::getRemainingTime() const
{
	return this->commandCounter; //commandCounter for now...
}

int Process::getCommandCounter() const
{
	return this->commandCounter;
}

std::size_t Process::getLinesOfCode() const
{
	return this->commandList.size();
}

int Process::getPID() const {
	return this->pid;
}

int Process::getCPUCoreID() const
{
	return this->cpuCoreID;
}

void Process::setState(Process::ProcessState currentState)
{
	this->currentState = currentState;
	if (currentState == Process::RUNNING)
	{
		this->runningTimestamp = getCurrentTimestamp(); // get timestamp when started running
	}
	else if (currentState == Process::FINISHED)
	{
		this->finishedTimestamp = getCurrentTimestamp(); // get timestamp when finished running
	}
}

Process::ProcessState Process::getState() const
{
	return this->currentState;
}

String Process::getName() const
{
	return this->name;
}

int Process::getMemSize() const
{
	return this->memorySize;
}

void Process::addCommand(std::shared_ptr<ICommand> command)
{
	/*if (command == nullptr)
	{
		std::cerr << "Failed to create command: No command declared [NULL detected]." << std::endl;
		return;
	}*/
	this->commandList.push_back(command);
}

void Process::generateRandomCommands()
{
	srand(time(nullptr)); // Seed the random number generator

	// Get min and max number of commands from config
	int minIns = ConfigReader::getInstance()->getMinIns();
	int maxIns = ConfigReader::getInstance()->getMaxIns();

	int noCommands = minIns + rand() % (maxIns - minIns + 1);

	for (int i = 0; i < noCommands; i++) {
		int type = rand()% 6; // 0 to 5 (PRINT, DECLARE, ADD, SUBTRACT, SLEEP, FOR)

		switch (type) {
			case 0: { // PRINT COMMAND
				auto newCommand = std::make_shared<PrintCommand>(this->pid, this->name, symbolTable);
				this->addCommand(newCommand);
				break;
			}

			case 1: { // DECLARE COMMAND
				String varName = "";
				auto newCommand = std::make_shared<DeclareCommand>(varName, 0, symbolTable);
				this->addCommand(newCommand);
				break;
			}

			case 2: { // ADD COMMAND
				auto newCommand = std::make_shared<AddCommand>(symbolTable);
				this->addCommand(newCommand);
				break;
			}

			case 3: { // SUBTRACT COMMAND
				auto newCommand = std::make_shared<SubtractCommand>(symbolTable);
				this->addCommand(newCommand);
				break;
			}

			case 4: { // SLEEP COMMAND
				uint8_t sleepTicks = static_cast<uint8_t>(rand() % 255); // clamp range of uint16_t from 0 to 255
				auto newCommand = std::make_shared<SleepCommand>(this->pid, sleepTicks);
				this->addCommand(newCommand);
				break;
			}
			case 5: { 
				const int MAX_DEPTH = 1 + rand() % 3; // Randomly choose max depth between 1 and 3
				const int repeats = 1 + rand() % 4; 
				generateNestedForCommand(1, MAX_DEPTH, repeats); // creates instructions then directly adds to commandList
				break;
			}
		}
	}
}

void Process::generateNestedForCommand(int currentDepth, int maxDepth, int repeats) 
{
	int repeatsNested = 1 + rand() % 2;
	int noCommands = 1 + rand() % 3;
	std::vector<std::shared_ptr<ICommand>> instructions;

	for (int i = 0; i < noCommands; i++) {
		int instructionType = rand() % 5; // 0 to 5 (PRINT, DECLARE, ADD, SUBTRACT, SLEEP, FOR)

		switch (instructionType) {
			case 0: {
				auto cmd = std::make_shared<PrintCommand>(this->pid, this->name, symbolTable);
				instructions.push_back(cmd->clone());
				break;
			}
			case 1: {
				auto cmd = std::make_shared<DeclareCommand>("", 0, symbolTable);
				instructions.push_back(cmd->clone());
				break;
			}
			case 2: {
				auto cmd = std::make_shared<AddCommand>(symbolTable);
				instructions.push_back(cmd->clone());
				break;
			}
			case 3: {
				auto cmd = std::make_shared<SubtractCommand>(symbolTable);
				instructions.push_back(cmd->clone());
				break;
			}
			case 4: {
				uint8_t ticks = static_cast<uint8_t>(rand() % 255);
				auto cmd = std::make_shared<SleepCommand>(this->pid, ticks);
				instructions.push_back(cmd->clone());
				break;
			}
			case 5: {
				if (currentDepth < maxDepth) {
					generateNestedForCommand(currentDepth + 1, maxDepth, repeatsNested);
				}
				break;
			}
		}
	}

	for (int r = 0; r < repeats; ++r) {
		for (const auto& cmd : instructions) {
			this->addCommand(cmd);
		}
	}
}


void Process::printCommands() const
{
	for (const auto& command : this->commandList) {
		std::cout << "Command Type: ";
		switch (command->getCommandType()) {
		case ICommand::PRINT:    std::cout << "PRINT"; break;
		case ICommand::DECLARE:  std::cout << "DECLARE"; break;
		case ICommand::ADD:      std::cout << "ADD"; break;
		case ICommand::SUBTRACT: std::cout << "SUBTRACT"; break;
		case ICommand::SLEEP:    std::cout << "SLEEP"; break;
		case ICommand::FOR:      std::cout << "FOR"; break;
		default:                 std::cout << "UNKNOWN"; break;
		}
		std::cout << std::endl;
	}
}


void Process::logInstruction(int core_id, String message)
{
	std::string log_entry = std::format("({}) Core {}: \"{}\"", getCurrentTimestamp(), core_id, message);
	this->print_logs.push_back(log_entry);
}

CommandList Process::getCommandList()
{
	return this->commandList;
}

String Process::getRunningTimestamp()
{
	return this->runningTimestamp;
}

String Process::getFinishedTimestamp()
{
	return this->runningTimestamp;
}

std::vector<String> Process::getLogs()
{
	return this->print_logs;
}

String Process::stateToString(Process::ProcessState state) {
	switch (state) {
	case Process::READY: return "READY";
	case Process::RUNNING: return "RUNNING";
	case Process::WAITING: return "WAITING";
	case Process::FINISHED: return "FINISHED";
	default: return "UNKNOWN";
	}
}


String Process::getCurrentTimestamp()
{
	auto now = std::chrono::system_clock::now();
	auto time_t = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
	localtime_s(&tm, &time_t);

	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
	String timestamp(buffer);

	return timestamp;
}
