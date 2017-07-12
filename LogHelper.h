/*
 * LogInitor.h
 *
 *  Created on: 2017年2月9日
 *      Author: wong
 */

#ifndef LOGHELPER_H_
#define LOGHELPER_H_

#include <string>

#include "base/AsyncLogging.h"

class LogHelper
{
public:
    LogHelper(int nLevel, const std::string& strPath, int filesize = 500 * 1000 * 1000);
    virtual ~LogHelper();

private:
    static void OutputFunc(const char* msg, int len);

private:
    static muduo::AsyncLogging* m_asyncLog;
};

#endif /* LOGHELPER_H_ */
