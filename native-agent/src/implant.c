#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//if in windows then
#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #pragma comment(lib, "ws2_32.lib")
#else //if linux then
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

//default configuration for the port and the localhost ip
#define DefaultIp "127.0.0.1"
#define DefaultPort 8000

int main (int argc, char *argv[]){
    
    char *targetIp = DefaultIp;
    int targetPort = DefaultPort;

    if (argc == 3){
        targetIp = argv[1];
        targetPort = atoi(argv[2]);
        printf("[*] Custom Configuration: %s:%d\n", targetIp, targetPort);
    }
    else{
        printf("[*] Using Default Configuration: %s:%d\n", targetIp, targetPort);
    }
    #ifdef _WIN32
        WSADATA wsa;
        printf("[*] Machine detected: WINDOWS\n");
        printf("[*] Initializing WinSocket...\n");
        if (WSAStartup(MAKEWORD(2,2), &wsa) != 0){
            printf("[!] Failed to initialized, ERROR: %d\n", WSAGetLastError());
            return 1;
        }
    #else
        printf("[*] Machine detected: LINUX\n");
        printf("[*] No WinSocket initialization needed. \n");
    #endif

    SOCKET soc = socket(AF_INET, SOCK_STREAM, 0);
    
    if (soc == INVALID_SOCKET){
        printf("[!] Socket creation failed.\n");
        return 1;
    }
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(targetIp);
    server.sin_family = AF_INET;
    server.sin_port = htons(targetPort);

    if (connect(soc, (struct sockaddr *)&server, sizeof(server)) < 0 ){
        printf("[!] Connection Failed to %s:%d\n", targetIp, targetPort);
        return 1;
    }
    printf("[+] Connected to C2! Waiting for orders...\n");

    while (1){
        #ifdef _WIN32
            Sleep(1000);
        #else  
            sleep(1);
        #endif
        char *heartbeat = "."; 
        send(soc, heartbeat, 1, 0);
    }

    #ifdef _WIN32
        closesocket(soc);
        WSACleanup();
    #else
        close(soc);
    #endif
    return 0;
}   