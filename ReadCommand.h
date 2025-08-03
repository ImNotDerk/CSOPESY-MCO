#include "ICommand.h"
class ReadCommand : public ICommand
{
public:
	ReadCommand(const String& varName, const String& address);
	ReadCommand(const String& varName);
	std::shared_ptr<ICommand> clone() const override;
	void execute() override;

	uint16_t getValue() const;
	String getOutput() const override;

private:
	String varName; 
	uint16_t value; 
};
