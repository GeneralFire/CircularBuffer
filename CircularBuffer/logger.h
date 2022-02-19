#pragma once

#include <ostream>
#include <iostream>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

class Logger
{
public:
    Logger& operator=(Logger const&) = delete;
	enum class logLevel
	{
		DBG = 0,
		LOG,
		WRN,
		ERR,
	};

    static void log(int lg, char const* const str)
    {
        log((logLevel)lg, str);
    }

	static void log(logLevel lg, char const* const str)
	{
        const char* end = RESET;
        const char* start = NULL;
        switch (lg)
        {
        case logLevel::DBG:
            start = GREEN;
            break;
        case logLevel::LOG:
            start = WHITE;
            break;
        case logLevel::WRN:
            start = YELLOW;
            break;
        case logLevel::ERR:
            start = RED;
            break;
        }
        if ((int)currentLogLevel <= (int)lg)
        {
            std::cout << start << str << end << std::endl;
        }
	}
    
    static void setLogLevel(logLevel lg)
    {
        currentLogLevel = lg;
    }

    static logLevel currentLogLevel;
};

