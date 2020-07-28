#include "Logger.h"

void Logger::getLog(string fromWhere, string type, string message)
{
	cout << type << ": [" << currentDateTime() << "] [" << fromWhere << "] " << message << endl;
}

void Logger::info(string fromWhere, string message)
{
	getLog(fromWhere, "INFO", message);
}

void Logger::warning(string fromWhere, string message)
{
	getLog(fromWhere, "WARNING", message);
}

void Logger::error(string fromWhere, string message)
{
	getLog(fromWhere, "ERROR", message);
}
string Logger::currentDateTime() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
#pragma warning(suppress : 4996)
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X", &tstruct);

	return buf;
}