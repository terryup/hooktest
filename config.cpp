#include "config.h"
#include "logger.h"

#include <iostream>

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

        //  实例：/home/zixuanhuang=1,2,3,4,5,6,7\n
        std::string key;
        std::vector<std::string> value;
        std::string course_id;
        key = read_buf.substr(0, idx);
        Trim(key);
        
        int endidx = read_buf.find('\n', idx);
        int startidx = idx + 1;
        ++idx;
        while (startidx != endidx)
        {
            if (read_buf[idx] == ',' || startidx == endidx)
            {
                course_id = read_buf.substr(idx, endidx - startidx - 1);
                Trim(course_id);
                idx = startidx;
                value.push_back(course_id);
                course_id.clear();
            }
            ++startidx;
        }
        m_configMap.insert({key, value});
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
