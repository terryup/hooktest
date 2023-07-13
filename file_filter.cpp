#include "config.h"
#include "logger.h"

#include <stdlib.h>
#include <string.h> //  strlen()
#include <vector>
#include <string>
#include <fstream> //   ifstream

#include <iostream>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>  //  stat
#include <dlfcn.h>  //  dlopen()

#define STRMAXLEN 301

//  配置文件的加载路径
const char config[] = "/home/zixuanhuang/hooktest/test.conf";

typedef int (*new_open)(const char *pathname, int flags);

Config m_config;

//  判断是否为保护文件
std::vector<std::string> is_protection_file(char *pathbuf)
{
    //  解析配置文件
    m_config.LoadConfigFile(config);
    std::vector<std::string> test = m_config.Load(pathbuf);
    return m_config.Load(pathbuf);
}

/*
//  查看当前的进程名
int getSelfProcessName(char *name, int len)
{
    
        std::string proc_path = "/proc/" + std::to_string(getpid()) + "/cmdline";
    std::ifstream cmdline(proc_path);
    std::string process_name;
    if (cmdline.is_open()) 
    {
        std::getline(cmdline, process_name, '\0');
    }
    std::cout << "path : " << process_name << std::endl;
    


    
    
    
    char *pName = strrchr(path, '/');
    std::cout << pName << "pName" << std::endl;
    std::cout << path + strlen(path) << ">" << pName + 2 << std::endl;
    if (pName != nullptr && path + strlen(path) > pName + 2)
    {
        if (strlen(pName + 1) > len)
        {
            std::cout << "strlen失败了" << std::endl;
            return 0;
        }
        else 
        {
            strcpy(name, pName + 1);
            return strlen(name);
        }
    }
    else
    {
        std::cout << "pName != nullptr && path + strlen(path) > pName + 2失败了" << std::endl;
        return 0;
    }

    

   //   char *pName = strrchr(process_name, '/');


}
*/

//  获取执行文件的绝对路径，例如：/home/zixuanhuang/hooktest/main
int getSelfProcessName(char *name, int len)
{
    char current_absolute_path[1024] = {0};
    int cnt = readlink("/proc/self/exe", current_absolute_path, 1024);
    if (cnt < 0 || cnt >= 1024)
    {
        LOG_ERROR("解析运行文件的绝对路径出错！");
        return 0;
    }

    strcpy(name, current_absolute_path);
    return strlen(name);

}

//  检测是否在白名单中
//  在白名单中返回真true， 不在白名单中返回假false
bool is_black_list(std::vector<std::string>& black_list, char *name)
{
    for (const auto& str : black_list)
    {
        //  找到了相匹配的---存在于白名单中
        if (str.compare(name) == 0)
        {
            return true;
        }
    }
    //  不存在于黑名单中
    return false;
}

/*
//  重定义open函数
int open(const char *pathname, int flags)
{
    std::cout << "成功拦截" << std::endl;
    void *handle = nullptr;
    new_open old_open = nullptr;

    //  获得libc.so的句柄
    handle = dlopen("libc.so.6", RTLD_LAZY);

    //  找到open函数的加载地址
    old_open = (new_open)dlsym(handle, "open");

    struct stat ststat;
    if (stat(pathname, &ststat) == 0)
    {
        //  判断是否是文件
        if (S_ISREG(ststat.st_mode))
        {
            //  全路径文件名buf
            char pathbuf[STRMAXLEN] = {0};
            //  判断是否为相对路径而，如不为/就是相对路径，转变为绝对路径
            if (pathname[0] != '/')
            {
                //  获取当前的工作路径
                getcwd(pathbuf, STRMAXLEN);
                //  如果结尾不为‘/’那就加上'/'
                if (pathbuf[strlen(pathbuf) - 1] != '/')
                {
                    strcat(pathbuf, "/");
                }
                strcat(pathbuf, pathname);
            }
            else
            {
                strcat(pathbuf, pathname);
            }
            
            //  这里函数判断是否为保护文件
            std::vector<std::string> protection_list = is_protection_file(pathbuf);

            //  获取当前进程的进程名 --> 配置文件中会有白名单的进程名，只要放行进程名即可
            char processName[1024] = {0};
            int ret = getSelfProcessName(processName, strlen(processName));
            //  等于0获取失败
            if (ret == 0)
            {
                LOG_ERROR("获取进程名失败！");
                exit(0);
            }

            //  检测当前进程名是否在白名单中
            bool is_black = is_black_list(protection_list, processName);

            //  如果为true也就是不在白名单中
            if (!is_black)
            {
                //  LOG_ERROR("进程%s正在打开%s, 操作已被拦截", processName, pathbuf);
                std::cout << "进程" << processName << "正在打开" << pathbuf << ",操作已经拦截" << std::endl;
                exit(0);
            }
        }
    }

    //  LOG_OK("进程%d正在打开%s,已放行", getpid(), pathname);
    std::cout << "进程" << getpid() << "正在打开"  << pathname << ",已放行" << std::endl;
    return old_open(pathname, flags);
}
*/

typedef int (*new_read)(int __fd, void *__buf, size_t __nbytes);

void get_file_path(int fd, char *file_path, size_t size)
{
    char buf[1024] = {'\0'};
    snprintf(buf, sizeof(buf), "/proc/self/fd/%d", fd);
    ssize_t len = readlink(buf, file_path, size - 1);
    if (len != -1)
    {
        file_path[len] = '\0';
    }
}

ssize_t read(int __fd, void *__buf, size_t __nbytes)
{
    //  std::cout << "成功拦截" << std::endl;

    void *handle = nullptr;
    new_read old_read = nullptr;

    //  获得libc.so的句柄
    handle = dlopen("libc.so.6", RTLD_LAZY);

    //  找到read函数的加载地址
    old_read = (new_read)dlsym(handle, "read");

    //  通过fd获取绝对路径
    char file_path[1024] = {0};
    get_file_path(__fd, file_path, sizeof(file_path));

    /*------------test测试----------------*/
    //  std::cout << file_path << std::endl;
    /*-----------------------------------*/


        //  这里函数判断是否为保护文件
        std::vector<std::string> protection_list = is_protection_file(file_path);

        /*
        for (auto& str : protection_list)
        {
            std::cout << "读取保护文件成功：" << str << std::endl;
        }
        */


        if (!protection_list.empty())
        {
            //  获取当前进程的进程名 --> 配置文件中会有白名单的进程名，只要放行进程名即可
            char processName[1024] = {0};
            int ret = getSelfProcessName(processName, strlen(processName));

            //  std::cout << "进程名： " << processName << std::endl;

            //  等于0获取失败
            if (ret == 0)
            {
                LOG_ERROR("获取进程名失败！");
                exit(0);
            }

            //  检测当前进程名是否在白名单中
            bool is_black = is_black_list(protection_list, processName);
            

            
            //  如果为true也就是不在白名单中
            if (!is_black)
            {
                LOG_ERROR("进程%s正在打开%s, 操作已被拦截", processName, file_path);
                std::cout << "进程" << processName << "正在打开" << file_path << ",操作已经拦截" << std::endl;
                return -1;
            }
            else
            {
                LOG_OK("进程%d正在打开%s,已放行", getpid(), file_path);
                return old_read(__fd, __buf, __nbytes);
            }
        }

    //  LOG_OK("进程%d正在打开%s,已放行", getpid(), file_path);
    //  std::cout << "进程" << getpid() << "正在打开"  << file_path << ",已放行" << std::endl;
    return old_read(__fd, __buf, __nbytes);
}