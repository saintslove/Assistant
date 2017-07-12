/*
 * LogInitor.cpp
 *
 *  Created on: 2017年2月9日
 *      Author: wong
 */

#include "LogHelper.h"

#include <stdio.h>

#include "base/Logging.h"
#include "base/AsyncLogging.h"

#include "Common.h"

muduo::AsyncLogging* LogHelper::m_asyncLog = NULL;

LogHelper::LogHelper(int nLevel, const std::string& strPath, int filesize/* = 500 * 1000 * 1000*/)
{
    switch (nLevel)
    {
    case 0:
        muduo::Logger::setLogLevel(muduo::Logger::TRACE);
        break;
    case 1:
        muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
        break;
    case 2:
        muduo::Logger::setLogLevel(muduo::Logger::INFO);
        break;
    case 3:
        muduo::Logger::setLogLevel(muduo::Logger::WARN);
        break;
    case 4:
        muduo::Logger::setLogLevel(muduo::Logger::ERROR);
        break;
    case 5:
        muduo::Logger::setLogLevel(muduo::Logger::FATAL);
        break;
    default:
        muduo::Logger::setLogLevel(muduo::Logger::INFO);
        break;
    }

    m_asyncLog = new muduo::AsyncLogging(strPath.c_str(), filesize);
    m_asyncLog->start();
    muduo::Logger::setOutput(OutputFunc);
}

LogHelper::~LogHelper()
{
    m_asyncLog->stop();
    SAFE_DELETE(m_asyncLog);
}

void LogHelper::OutputFunc(const char* msg, int len)
{
    if (m_asyncLog != NULL)
    {
        m_asyncLog->append(msg, len);
    }
    fwrite(msg, 1, len, stdout);
    fflush(stdout);
}

