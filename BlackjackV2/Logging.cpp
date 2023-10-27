#include "Logging.h"


Logging::Logging()
{
	std::cout << "logger initilzied" << std::endl;
}

Logging::~Logging()
{
	std::cout << "logger destroyed" << std::endl;
}

void Logging::debugLog(LogLevel level, std::string message)
{
	switch (level)
	{
	case Logging::info:
		std::cout << "\u001b[32mInfo Message: ";
		break;
	case Logging::warning:
		std::cout << "\u001b[33mWarning Message: ";
		break;
	case Logging::error:
		std::cout << "\u001b[31mError Message: ";
		break;
	default:
		std::cout << "Unknown Log Level";
		break;
	}
	std::cout << message << "\u001b[0m" << std::endl;

}
