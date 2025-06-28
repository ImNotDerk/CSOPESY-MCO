#include "Process.h"

Process::Process(int pid, String name) {
	this->pid = pid;
	this->name = name;
	this->commandCounter = 0;
	this->generateRandomCommands();
	this->currentState = Process::ProcessState::READY;
}

// called by CPU core worker
void Process::executeCurrentCommand(int coreId)
{
	if (this->commandCounter < commandList.size()) {

		//ICommand* cmd = commandList[commandCounter].get();

		//if (auto* forCmd = dynamic_cast<ForCommand*>(cmd)) {
		//	forCmd->performWithLogging(this, coreId, 1); // FOR will increment inside
		//}
		//else {
		//	cmd->execute();
		//	this->logInstruction(coreId, cmd->getOutput());
		//}
		commandList[commandCounter]->execute();  // run instruction
		this->logInstruction(coreId, commandList[commandCounter]->getOutput());
		this->commandCounter++; // count this single command

		// Still move to next top-level command

		//this->commandCounterIndex++; // use a separate index if needed
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
	// Get min and max number of commands from config
	int min = ConfigReader::getInstance()->getMinIns();
	int max = ConfigReader::getInstance()->getMaxIns();

	int noCommands = min + rand() % (max - min + 1);

	for (int i = 0; i < noCommands; i++) {
		int type = rand() % 3; // 0 to 5 (PRINT, DECLARE, ADD, SUBTRACT, SLEEP, FOR)

		switch (type) {
		case 0: { // PRINT COMMAND
			String msg;
			if (!symbolTable.empty())
			{
				bool msgOrNone = rand() % 2; // 0 for no message, 1 for message

				if (msgOrNone) {
					int randomIndex = rand() % symbolTable.size();
					auto it = symbolTable.begin();
					std::advance(it, randomIndex);

					uint16_t varValue = it->second;
					msg = "Print value from: " + std::to_string(varValue);
				}
				else {
					msg = "Hello world from " + this->name;
				}
			}
			else 
			{
				msg = "Hello world from " + this->name;
			}

			const std::shared_ptr<ICommand> command = std::make_shared<PrintCommand>(this->pid, msg);
			this->addCommand(command);
			break;
		}

		case 1: { // DECLARE COMMAND
			String varName = getUniqueVariableName();
			std::shared_ptr<DeclareCommand> declareCmd = std::make_shared<DeclareCommand>(varName, 0); // 0 placeholder since uint16_t declaration must be in DeclareCommand's execute()
			declareCmd->execute();
			symbolTable[declareCmd->getVariableName()] = declareCmd->getValue();
			this->addCommand(declareCmd);
			break;
		}

		case 2: { // ADD COMMAND
			uint16_t operand1 = 0, operand2 = 0;

			if (symbolTable.size() >= 2) {
				int operand1Index;
				int operand2Index;

				operand1Index = rand() % symbolTable.size();
				auto iterate = symbolTable.begin();
				std::advance(iterate, operand1Index);
				operand1 = iterate->second;

				operand2Index = rand() % symbolTable.size();
				auto iterate2 = symbolTable.begin();
				std::advance(iterate2, operand2Index);
				operand2 = iterate2->second;
			}
			else if (symbolTable.size() == 1) {
				int operand1Index;
				operand1Index = rand() % symbolTable.size();
				auto iterate = symbolTable.begin();
				std::advance(iterate, operand1Index);
				operand1 = iterate->second;
				operand2 = 0;
			}
			else {
				operand1 = 0;
				operand2 = 0;
			}
			std::shared_ptr<AddCommand> addCmd = std::make_shared<AddCommand>(operand1, operand2);
			addCmd->execute();
			String varName = getUniqueVariableName();
			symbolTable[varName] = addCmd->getResult();
			this->addCommand(addCmd);
			break;
		}

		case 3: { // SUBTRACT COMMAND
			// create subtract command instance here
			// then use the addCommand function here
			break;
		}

		case 4: { // SLEEP COMMAND
			// create sleep command instance here
			// then use the addCommand function here
			break;
		}
		case 5: { 
			/*int repeats = 1 + rand() % 4;
			std::vector<ICommand*> forInstructions;

			String msg = "Inside loop";

			forInstructions.push_back(new PrintCommand(this->pid, msg));

			const std::shared_ptr<ICommand> command = std::make_shared<ForCommand>(this->pid, forInstructions, repeats);
			this->addCommand(command);
			break;*/

			/*const int MAX_DEPTH = 3;
			ICommand* forCmd = generateNestedForCommand(1, MAX_DEPTH);
			this->addCommand(std::shared_ptr<ICommand>(forCmd));*/
			break;
		}
		}
	}
}

ICommand* Process::generateNestedForCommand(int currentDepth, int maxDepth) 
{
	int noCommands = 1 + rand() % 2;

	String msg = "[FOR] Create FOR depth: " + std::to_string(currentDepth);

	if (currentDepth >= maxDepth) {
		String msg = "[FOR] Max FOR depth reached";
		return new PrintCommand(this->pid, msg);
	}

	int repeats = 1 + rand() % 2; // Randomly choose number of repeats
	std::vector<ICommand*> innerCommands;

	innerCommands.push_back(new PrintCommand(this->pid, msg));

	// Generate Random Commands
	for (int i = 0; i < noCommands; ++i) {
		int type = rand() % 2;
		if (type == 0) {
			String msg = "[PRINT] Print from FOR depth " + std::to_string(currentDepth);
			innerCommands.push_back(new PrintCommand(this->pid, msg));
		}
		else if (type == 1) {
			// Nested FOR — increase depth
			ICommand* nestedFor = generateNestedForCommand(currentDepth + 1, maxDepth);
			innerCommands.push_back(nestedFor);
		}
	}

	//String msge = "Debug:: Exit FOR depth " + std::to_string(currentDepth);

	//innerCommands.push_back(new PrintCommand(this->pid, msge));

	return new ForCommand(this->pid, innerCommands, repeats);
}

void Process::printCommands() const
{
	for (const auto& command : this->commandList) {
		command->execute(); // Assuming ICommand has a print or execute method to display the command
		std::cout << std::endl;
	}
}

String Process::getUniqueVariableName() {
	int varCounter = 0;
	String varName = "var";
	String newKey;

	do {
		newKey = varName + std::to_string(varCounter);
		varCounter++;
	} while (symbolTable.find(newKey) != symbolTable.end());

	return newKey;
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
