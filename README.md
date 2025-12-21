# Offensive Development Lab

## Mission Statement
This repository serves as a personal research environment for understanding Offensive Security architecture, Windows Internals, and Command & Control (C2) design. 

The goal is to move beyond "scripting" and achieve **Deep Understanding** of the underlying technologies (Memory Management, Win32 API, Sockets).

## Directory Structure

### 1. /native-agent (Active)
**Status:** In Development  
**Language:** C (Win32 API)  
**Objective:** Building a custom C2 implant from scratch without external libraries. Focus on manual memory management, raw socket handling via WinSock, and stealth techniques.
- No abstraction layers.
- Pure interaction with the Kernel.

### 2. /legacy-c2 (Archived)
**Status:** Frozen / Reference Only  
**Language:** C++ (with Libraries)  
**Objective:** Initial prototype generated to understand the basic flow of a C2 connection. Kept for historical reference and logic comparison.

---
*Warning: This code is for educational and research purposes only. Authorized use only.*