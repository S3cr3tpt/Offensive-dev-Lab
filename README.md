# Offensive Development & C2 Engineering Lab

### 🛡️ Project Goal
This repository documents my journey into low-level systems programming and network security. The goal is to reverse-engineer standard malware architectures to understand how Command & Control (C2) infrastructures function, how they are detected, and how to defend against them.

**Current Focus:** Custom C2 protocols, Socket Programming (C/Python), and Concurrency.

---

## 📂 Project Structure

### [Stage 1: Synchronous C2 Infrastructure](./Stage1_Basic)
* **Status:** ✅ Complete
* **Architecture:** Single-Client / Blocking I/O
* **Description:** A foundational implementation of a C2 listener and implant.
    * **Implant (C):** Cross-platform (Linux/Windows), stealth execution, output redirection.
    * **Server (Python):** Basic command handling and response parsing.
* **Key Concept:** Understanding the TCP 3-Way Handshake and the `send()`/`recv()` synchronization loop.

### [Stage 2: Threaded "Lobby" Architecture](./Stage2_Threaded)
* **Status:** 🚧 In Progress
* **Architecture:** Multi-Threaded / Concurrent Handling
* **Description:** upgrading the "Brain" to handle multiple simultaneous connections.
    * **Session Management:** Using Python `threading` to manage a "Lobby" of active bots.
    * **UX:** Dynamic switching between targets (`use <id>`, `background`).
* **Key Concept:** Concurrency, Race Conditions, and Shared Memory management.

---

## 🧠 Skills Matrix

| Skill | Usage in Project |
| :--- | :--- |
| **C Programming** | Memory management, Process execution (`popen`), Socket API |
| **Python** | Rapid prototyping, Threading, String manipulation |
| **Networking** | TCP/IP flow control, Port handling, Endianness (`htons`) |
| **OS Internals** | Standard I/O streams (`stdout`/`stderr`), Process pipes |
| **Cross-Compilation** | Building Windows Executables (`.exe`) from Linux (MinGW) |

---

## ⚠️ Disclaimer
**Educational Use Only.**
This code is written for research and security training. It is designed to be run in isolated lab environments. I am not responsible for misuse of this code.