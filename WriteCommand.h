#include "ICommand.h"
class WriteCommand : public ICommand
{
public:
	WriteCommand(const String& address, uint16_t value);
	std::shared_ptr<ICommand> clone() const override;
	void execute() override;

	void reWrite();
	String getOutput() const override;

private:
	String address;
	uint16_t value;
};
