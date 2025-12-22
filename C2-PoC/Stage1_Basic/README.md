# C2 PoC - Stage 1: Basic Synchronous Infrastructure

### ⚠️ Disclaimer
**This project is for educational and research purposes only.**
The code demonstrates the foundational architecture of Command and Control (C2) systems, specifically focusing on socket programming and process management in C and Python. Usage of this code against any system without authorized permission is illegal.

---

## 📖 Overview
**Stage 1** represents the simplest form of a C2 infrastructure: a synchronous, 1-on-1 connection model.
It consists of a Python "Server" (Listener) and a C "Implant" (Agent).

* **Architecture:** Client-Server (Reverse Shell Model)
* **Protocol:** Raw TCP
* **Interaction:** Synchronous (Blocking I/O)

### Features
* **Cross-Platform Implant (C):**
    * Compiles on **Linux** (GCC) and **Windows** (MinGW/MSVC).
    * Auto-detects OS and reports to server upon connection.
    * **Stealth Mode:** Silent execution on the target (no local stdout/printf).
    * **Traffic Redirection:** Merges `stderr` into `stdout` so errors are visible to the operator.
* **Basic Listener (Python):**
    * Handles incoming TCP connections.
    * Sends shell commands to the target.
    * Receives and displays output.

---

## 🛠️ Usage

### 1. Start the Server (The Brain)
The server must be running first to listen for incoming connections.
```bash
# Usage: python3 server.py <IP> <PORT>
python3 server.py 0.0.0.0 8000

```

* `0.0.0.0`: Listens on all available network interfaces.
* `8000`: The port to open.

### 2. Compile the Implant (The Muscle)

**Option A: Linux (Native)**

```bash
gcc implant.c -o implant

```

**Option B: Windows (Cross-Compile from Linux)**
Requires `mingw-w64`.

```bash
x86_64-w64-mingw32-gcc implant.c -o implant.exe -lws2_32

```

### 3. Execution

Run the implant on the target machine (or a second terminal for testing).

```bash
./implant

```

* **On Target:** The terminal will remain silent (Stealth Mode).
* **On Server:** You will see a notification: `[*] New Bot Online: Target is LINUX`

---

## 📝 Technical Details

* **Socket Creation:** Uses `AF_INET` (IPv4) and `SOCK_STREAM` (TCP).
* **Command Execution:** The implant uses `popen()` to open a process stream, execute the command, and read the output line-by-line.
* **Synchronization:** The server performs a handshake read (`recv`) immediately upon connection to clear the "Hello" message buffer before entering the command loop.
