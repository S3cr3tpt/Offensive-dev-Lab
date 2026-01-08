# Stage 3: Persistence & Auto-Start Mechanisms

## Overview
This stage introduces **persistence** to the C2 implant. The goal is to ensure the implant automatically re-executes when the victim machine is rebooted or the user logs in. This moves the project from a simple "Reverse Shell" to a proper "Post-Exploitation Implant."

The implementation logic is **OS-agnostic**, automatically detecting the underlying platform (Windows or Linux) and applying the appropriate persistence technique.

## Features Added
* **Windows Persistence:** Uses the Windows API to modify the **User Registry Hive (HKCU)**.
* **Linux Persistence:** Modifies the user's shell configuration (`.bashrc`) to trigger execution on terminal launch.
* **Dynamic Path Resolution:** The implant identifies its own location on the disk at runtime (using `GetModuleFileName` or `readlink`), preventing errors if the file is moved.
* **Background Execution:** Linux payload automatically detaches from the terminal to remain stealthy.

---

## Technical Implementation

### Windows (`RegOpenKeyEx`)
On Windows systems, the implant targets the `Run` key in the Registry. This key controls programs that start when a specific user logs in.
* **Target Key:** `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`
* **Value Name:** `WindowsHealthCheck` (Disguised service name)
* **Permissions:** Uses `HKCU` to avoid requiring Administrator privileges.

### Linux (`.bashrc`)
On Linux systems, the implant appends a command to the user's local `.bashrc` file.
* **Target File:** `~/.bashrc`
* **Method:** Appends `/path/to/implant &`.
* **Stealth:** The ampersand (`&`) ensures the process runs in the background, preventing the terminal from hanging during startup.

---

## Usage & Compilation

### 1. Start the C2 Server
The server must be running to catch the incoming connection once the persistence triggers.
```bash
python3 server.py <IP> <PORT>

```

### 2. Compile the Implant

**For Linux (Testing):**

```bash
gcc implant.c -o implant

```

**For Windows (Cross-Compile from Linux):**

```bash
i686-w64-mingw32-gcc implant.c -o implant.exe -lws2_32

```

### 3. Execution

* **First Run:** Execute the binary manually once (`./implant` or `implant.exe`).
* **Persistence Installation:** The `persistence()` function runs immediately, installing the hooks.
* **Verification:** Reboot the machine or restart the terminal. The implant should automatically connect back to the C2 server.

---

## Disclaimer

**For Educational Purposes Only.**
This project is created for **Offensive Development research** and **Malware Analysis** training. It is designed to simulate post-exploitation techniques in a controlled, isolated laboratory environment. Misuse of this software is illegal.

