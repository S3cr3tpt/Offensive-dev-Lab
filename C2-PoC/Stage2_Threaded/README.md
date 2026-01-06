# Stage 2.5: Multi-Threaded C2 with Persistent Shell

## 📖 Overview
Stage 2 evolves the C2 infrastructure from a simple connection into a **Concurrent Multi-Client System** with a stable, persistent shell environment. 
It utilizes a "Lobby" architecture where the listener runs in a background thread, while the main thread manages active sessions.

* **Architecture:** Asynchronous / Multi-Threaded
* **Protocol:** Custom TCP with `<END>` delimiter for stability.
* **Shell Type:** Persistent (Supports `cd` and state retention).

## ⚡ Key Features
* **Background Listener:** A daemon thread that perpetually accepts new connections without blocking the operator.
* **Persistent Shell:** * Implemented **Internal Command Handling** for `cd`. The implant modifies its own working directory using the C `chdir()` API, allowing users to navigate the file system permanently.
* **Stable Data Exfiltration:**
    * Uses a custom **`<END>` marker protocol** to ensure large outputs (like `ls -la` or `netstat`) are received completely before the prompt returns.
    * Solves the "fragmentation lag" issue found in raw socket implementations.
* **Session Management:**
    * **List:** View all active agents in the "Lobby".
    * **Interact:** Switch focus to a specific agent (`use <id>`).
    * **Auto-Pruning:** Automatically cleans up dead sessions.

---

## 🛠️ Usage

### 1. Start the Server (Python)
```bash
# Usage: python3 server.py <IP> <PORT>
python3 server.py

```

### 2. Connect Agents (C Implant)

Compile and run the implant on the target:

```bash
# Windows (Mingw)
x86_64-w64-mingw32-gcc implant.c -o implant.exe -lws2_32

# Linux (GCC)
gcc implant.c -o implant

```

### 3. Operator Commands

| Command | Description |
| --- | --- |
| `list` | Displays table of all active connections. |
| `use <id>` | Enter interactive shell with a specific Bot. |
| `exit` | Shutdown the server. |

### 4. Interactive Shell Commands

| Command | Description |
| --- | --- |
| `cd <path>` | **New:** Changes the directory on the remote machine persistently. |
| `back` | Return to main lobby (Session stays alive). |
| `exit` | Kill this specific bot. |
| `*` | Any other command is executed via `popen`. |

---

## 🧠 Technical Implementation

### 1. The "Interceptor" Logic (Implant)

The C implant uses an interceptor pattern to handle state-changing commands:

* **Incoming Command:** Checked against internal keywords (e.g., `cd`).
* **If `cd`:** Executed via `_chdir()`/`chdir()` to modify the process environment.
* **If other:** Passed to `popen()` for system execution.

### 2. The "Over" Protocol (Server)

To prevent TCP fragmentation issues, the server implements a dynamic receive loop:

* The server listens for data chunks continuously until it detects the specific `<END>` marker sent by the implant.
* This ensures that long outputs are fully captured before the operator is prompted for the next command.

```
