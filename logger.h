#pragma once

#include "loggerqueue.h"

enum LogLevel
{
    OK, //  访问成功
    ERROR,  //  访问失败-禁止访问
};

//  日志类
class Logger
{
private:
    int m_loglevel;
    LockQueue<std::string> m_lockQue;

    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;

public:
    //  获取日志单例
    static Logger& GetInstance();

    //  设置日志级别
    void SetLogLevel(LogLevel level);

    //  写日志
    void Log(std::string msg);
};


//  定义宏  LOG_OK("xxx %d %s", 20, "xxx")
#define LOG_OK(logmsgformat, ...) \
    do { \
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(OK); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(c); \
    } while (0)

//  定义   LOG_ERROR("xxx %d %s", 20, "xxx");
#define LOG_ERROR(logmsgformat, ...) \
    do { \
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(ERROR); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(c); \
    } while (0)