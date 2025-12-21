# Universal Native Agent

## Overview
A cross-platform C implant capable of running on both Windows and Linux.
It uses **Preprocessor Directives** to detect the Operating System at compile time and select the correct API (WinSock for Windows, BSD Sockets for Linux).

## Configuration
- **Target IP:** 127.0.0.1 (Localhost)
- **Target Port:** 8000
- **Protocol:** TCP

## Compilation Instructions

### Option A: Compiling for Windows
**Requirement:** MinGW or Visual Studio.
**Command:**
```bash
gcc src/main.c -o build/agent.exe -lws2_32

```

*Note: The `-lws2_32` flag links the Windows Socket library.*

### Option B: Compiling for Linux

**Requirement:** GCC.
**Command:**

```bash
gcc src/main.c -o build/agent_linux

```

*Note: No special linker flags needed for Linux sockets.*

## How it Works

1. **#ifdef _WIN32:** Checks if the compiler is targeting Windows.
2. **Windows Path:** Initializes `WSAStartup`, uses `closesocket()`.
3. **Linux Path:** Skips initialization, uses standard `close()`.

