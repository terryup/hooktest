  #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

#include <iostream>

#include <unistd.h>
#include <string.h>

int main ()
{
    int fd = open("/home/zixuanhuang/hooktest/test.txt", 00);
    if (fd == -1)
    {
      std::cout << "打开失败" << std::endl;
    }
    std::cout << "open成功" << std::endl;

    char buf[1024] = {0};

    int idx = read(fd, buf, strlen(buf));
    if (idx >= 0)
    {
      std::cout << "read成功！" << std::endl;
    }
    else
    {
      std::cout << "read失败!!" << std::endl;
    }
    return 0;
}