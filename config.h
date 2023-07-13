#pragma once

#include <vector>
#include <string>
#include <unordered_map>


//  读取配置文件类
class Config
{
public:
    //  负责解析加载配置文件
    void LoadConfigFile (const char *config_file);

    //  查询保护文件列表
    //  输入参数key（文件路径）---- 返回参数vector<string> 白名单进程ID列表
    std::vector<std::string> Load (const std::string&key);


    //  查询IP:PORT是否在文件中
    int Load_ip_port (const std::string&key);

private:
    
    //  保护文件路径到他的白名单进程ID的映射
    //  first: 保护文件路径， second: 白名单进程ID
    std::unordered_map<std::string, std::vector<std::string>> m_configMap;

    //  放IP：port的
    //  first:ip, second:port
    std::unordered_map<std::string, int> m_configMap_ip_port;

    //  去掉字符串前后的空格
    void Trim (std::string& src_buf);

};
