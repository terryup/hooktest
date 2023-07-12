#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>

#define STRMAXLEN 301

const char path[] = "/home/changun/test"; // 受监控的目录
const char word[] = "popensss";       // 敏感字，在受监控的目录中存在敏感字的文件受到保护

typedef int (*new_open)(const char *pathname, int flags);

int open(const char *pathname, int flags)
{
    printf("拦截成功");
    void *handle = NULL;
    new_open old_open = NULL; // 保存成old_open以备调用

    /*
    // 获得libc.so.6的句柄
    handle = dlopen("libc.so.6", RTLD_LAZY);

    // 返回open函数在libc.so.6中的加载时的地址
    old_open = (new_open)dlsym(handle, "open");
    */

   old_open = (new_open)dlsym(RTLD_NEXT, "open");
    

    struct stat ststat;
    if (stat(pathname, &ststat)==0)
    {
        // 断是否为文件
        if (S_ISREG(ststat.st_mode))
        {
            // 判断是否为全路径，不为则补全当前路径
            char pathbuf[STRMAXLEN]; // 全路径文件名buf
            if (pathname[0] != '/')
            {
                memset(pathbuf, 0, STRMAXLEN);
                getcwd(pathbuf, STRMAXLEN);
                if (pathbuf[strlen(pathbuf)-1] != '/')
                    strcat(pathbuf, "/");
                strcat(pathbuf, pathname);
            }
            else
            {
                strcpy(pathbuf, pathname);
            }
            // 判断文件是否在监控的目录中
            if (strstr(pathbuf, path) == &pathbuf[0])
            {// 表明该文件受到监控
                FILE *fp = NULL;
                char buffer[1024]; memset(buffer, 0, sizeof(buffer));

                char strcmd[128]; memset(strcmd, 0, sizeof(strcmd));
                snprintf(strcmd, sizeof(strcmd), "grep %s %s", word, pathbuf);
                fp = popen(strcmd, "r");

                // buffer不为空则存在关键字，拦截
                if (fgets(buffer, sizeof(buffer), fp) != NULL)
                {
                    printf("进程%d正在打开%s, 该操作已被拦截\n", getpid(), pathbuf);
                    pclose(fp);
                    exit(0);
                }
                pclose(fp);
            }
        }
    }

    printf("进程%d正在打开%s, 已放行\n", getpid(), pathname);

    return old_open(pathname, flags);
}
