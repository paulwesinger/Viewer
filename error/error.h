#ifndef ERROR_H
#define ERROR_H

#include "../logs/logs.h"

class ErrorLogs
{
public:
    static void Warning(bool isExpressionTrue,std::string message);
    static void Failed(bool isExpressionTrue,const std::string message, bool & errorflag );

private:

    ErrorLogs() {}
};

#endif // ERROR_H
