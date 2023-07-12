  #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>


int main ()
{
    int fd = open("/home/zixuanhuang/hooktest/test.txt", O_RDONLY);
    return 0;
}