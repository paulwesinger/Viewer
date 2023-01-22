#include "error.h"


void ErrorLogs::Warning(bool isExpressionTrue,std::string message) {
    if ( ! isExpressionTrue) {
        logwarn(message);
    }
}

void ErrorLogs::Failed(bool isExpressionTrue,const std::string message, bool & errorflag) {
    if (! isExpressionTrue) {
        errorflag = true;
        logwarn(message);
        return;
    }
    errorflag = false;
}

