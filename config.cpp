#include "config.h"
#include "logger.h"

#include <iostream>
#include <sstream>


//  负责解析加载配置文件
void Config::LoadConfigFile(const char *config_file)
{
    //  打开文件读取
    FILE *pf = fopen(config_file, "r");
    if (nullptr == pf)
    {
        //  后面修改为写入日志
        std::cout << config_file << "is note exist!" << std::endl;
        exit(EXIT_FAILURE);
    }

    char ip_name[3] = "ip";

    //  如火没读完就进入循环
    while (!feof(pf))
    {
        char buf[512] = {0};
        fgets(buf, 512, pf);


        std::string read_buf(buf);
        //  去掉字符串前后的空格
        Trim(read_buf);

        //  判断#的注释
        if (read_buf[0] == '#' || read_buf.empty())
        {
            continue;
        }

        //  解析配置项
        int idx = read_buf.find('=');
        if (idx == -1)
        {
            //  配置信息不合法
            LOG_ERROR("配置文件不合法！");
            continue;
        }

        /*
        
        //  实例：/home/zixuanhuang/hooktest/test.txt=/home/zixuanhuang/hooktest/main
        std::string key;
        std::vector<std::string> value;
        std::string course_id;
        key = read_buf.substr(0, idx);
        Trim(key);
        
        int endidx = read_buf.find('\n');
        int startidx = idx + 1;
        ++idx;
        while (startidx != endidx)
        {
            ++startidx;
            //  实例：/home/zixuanhuang=1,2,3,4,5,6,7\n -> 30
            //  = -> 17   1 -> 18 , -> 19 
            if (read_buf[idx] == ',' || startidx == endidx)
            {
                course_id = read_buf.substr(idx, startidx);
                Trim(course_id);
                value.push_back(course_id);
                course_id.clear();
                if (startidx == endidx)
                {
                    break;
                }
                idx = ++startidx;
            }
            
        }
        std::cout << "配置文件加载：" << key << std::endl;
        for (auto &str : value)
        {
            std::cout << "白名单：" << str << std::endl;
        }
        std::cout << value.size() << std::endl;

        */


        std::string key;
        key = read_buf.substr(0, idx);
        Trim(key);
        ++idx;
        if (key.compare(ip_name) == 0)
        {
            //  这个判断出来这后面是IP行
            int end = read_buf.find('\n');
            std::string ip_port = read_buf.substr(idx, end - idx);
            Trim(ip_port);
            int ip_index = ip_port.find(':');
            std::string ip = ip_port.substr(0, ip_index);
            std::string port_str = ip_port.substr(ip_index + 1, end - ip_index + 1);
            int port = std::stoi(port_str);

            m_configMap_ip_port.insert({ip, port});
        }
        else
        {
            int end = read_buf.find('\n');
        int start_idx = idx;
        std::vector<std::string> value;

        while (start_idx != end)
        {
            ++start_idx;
            if (read_buf[start_idx] == ',')
            {
                std::string course_id = read_buf.substr(idx, start_idx - idx);
                ++start_idx;
                idx = start_idx;
                value.push_back(course_id);
            }
            else if (read_buf[start_idx] == '\n')
            {
                std::string course_id = read_buf.substr(idx, start_idx - idx);
                value.push_back(course_id);
                break;
            }
        }


        /*----------------------测试代码------------------*/
        /*
            std::cout << "配置文件加载：" << key << std::endl;
            for (const auto& str : value)
            {
                std::cout << "白名单：" << str << std::endl;
            }
        */
        

        m_configMap.insert({key, value});
        } 
    }
}

//  查询保护文件列表
std::vector<std::string> Config::Load (const std::string&key)
{
    auto it = m_configMap.find(key);
    if (it == m_configMap.end())
    {
        return {};
    }
    return it->second;
}

//  去掉字符串前后的空格
void Config::Trim (std::string& src_buf)
{
    int idx = src_buf.find_first_not_of(' ');
    if (idx != -1)
    {
        src_buf = src_buf.substr(idx, src_buf.size());
    }
    idx = src_buf.find_last_not_of(' ');
    if (idx != -1)
    {
        src_buf = src_buf.substr(0, idx + 1);
    }
}


//  查询IP:PORT是否在文件中
int Config::Load_ip_port (const std::string&ip)
{
    auto it = m_configMap_ip_port.find(ip);
    if (it == m_configMap_ip_port.end())
    {
        return -1;
    }
    return it->second;
}