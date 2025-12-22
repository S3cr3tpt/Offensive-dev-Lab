# Stage 2: Multi-Threaded C2 "The Lobby"

## 📖 Overview
Stage 2 evolves the C2 infrastructure from a synchronous, single-client model to a **Concurrent Multi-Client System**. 
It utilizes a "Lobby" architecture where the listener runs in a background thread, allowing the main operator thread to manage multiple active sessions simultaneously without blocking.

* **Architecture:** Asynchronous / Multi-Threaded
* **Handling:** Concurrent connections (Python `threading`)
* **State Management:** Global Shared Dictionary (`ALL_CLIENTS`)

## ⚡ Key Features
* **Background Listener:** A daemon thread that perpetually accepts new connections and performs the initial handshake (OS detection) without interrupting the operator.
* **Session Management:**
    * **List:** View all active agents in the "Lobby".
    * **Interact:** Switch focus to a specific agent (`use <id>`) to send commands.
    * **Background:** Return to the lobby (`back`) while keeping the agent connection alive.
* **Auto-Pruning:** Automatically cleans up dead sessions if a connection error occurs.

---

## 🛠️ Usage

### 1. Start the Multi-Threaded Server
```bash
# Usage: python3 server.py <IP> <PORT>
# Default: 0.0.0.0 8000
python3 server.py

```

* **Note:** The server prompt `C2_Lobby >` will remain active even while new bots connect in the background.

### 2. Connect Agents (Implants)

Run the implant on multiple target machines (or multiple terminal tabs).

```bash
./implant

```

* The server will notify: `[+] New Bot Connected: ID 0 | IP: 127.0.0.1 | OS: LINUX`

### 3. Operator Commands (The Lobby)

| Command | Description |
| --- | --- |
| `list` | Displays table of all active connections (ID, IP, OS). |
| `use <id>` | Enter interactive shell with specific Bot ID (e.g., `use 0`). |
| `exit` | Shutdown the server and all threads. |

### 4. Session Commands (Inside a Bot)

| Command | Description |
| --- | --- |
| `back` | Return to main lobby (Session stays alive). |
| `exit` | Kill this specific bot and remove from list. |
| `*` | Any other command is sent to the target shell (e.g., `ls`, `whoami`). |

---

## 🧠 Technical Implementation

### The Concurrency Model

The server uses Python's `threading` library to split duties:

1. **Thread A (Daemon):** Runs `listener_handler()`. It loops on `server.accept()`. When a client connects, it performs a blocking `recv()` to get the OS info, registers the client in the `ALL_CLIENTS` dictionary, and immediately goes back to listening.
2. **Thread B (Main):** Runs `main_menu()`. It handles user input. Since Python's `input()` is blocking, this thread does nothing until the operator types.

### Data Structure

Active sessions are stored in a global dictionary for O(1) access:

```python
ALL_CLIENTS = {
    0: [socket_obj, ("127.0.0.1", 4444), "LINUX"],
    1: [socket_obj, ("192.168.1.5", 5555), "WINDOWS"]
}

```