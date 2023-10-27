#pragma once
#include <iostream>
#include <string>

class Logging
{
public:
	enum LogLevel
	{
		info,
		warning,
		error
	};

	Logging();
	~Logging();

	void debugLog(LogLevel level, std::string message);
};

