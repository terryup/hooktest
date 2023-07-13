#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

int main() {
    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket creation failed. Error: " << strerror(errno) << std::endl;
        return 1;
    }
    
    // 设置服务器地址和端口
    std::string serverIP = "127.0.0.1";
    int serverPort = 8080;
    
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverIP.c_str(), &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Invalid server IP address." << std::endl;
        return 1;
    }
    
    // 连接到服务器
    if (connect(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cout << "connect失败！" << std::endl;
        close(sockfd);
        return 1;
    }
    
    std::cout << "connect成功" << std::endl;
    
    // 在这里可以执行其他操作或发送/接收数据
    
    // 关闭套接字
    close(sockfd);
    
    return 0;
}