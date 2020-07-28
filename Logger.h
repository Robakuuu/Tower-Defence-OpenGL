#pragma once
#include <string>
#include <iostream>
#include <time.h>

using namespace std;

class Logger
{
private:
	Logger();
	static void getLog(string fromWhere, string type, string message);
	static std::string currentDateTime();

public:
	static void info(string fromWhere, string message);
	static void warning(string fromWhere, string message);
	static void error(string fromWhere, string message);
};

