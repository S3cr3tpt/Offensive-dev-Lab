# Legacy C2 Prototype

## Status: ARCHIVED

## Origin
This codebase was the initial proof-of-concept (PoC) for the offensive lab. It was generated with AI assistance to visualize the logic flow of a Command and Control connection.

## Technical Details
- **Language:** C/C++
- **Dependencies:** Relies on external libraries / high-level wrappers.
- **Limitation:** The use of abstraction layers obscured the actual mechanics of how Windows handles connections and memory.

## Reason for Archival
Development on this branch has ceased. The project has moved to `native-agent` to prioritize:
1.  **Full Code Ownership:** Writing every line manually to ensure 100% understanding.
2.  **Native Implementation:** Using the Win32 API directly instead of libraries.
3.  **Optimization:** Reducing binary size and dependency bloat.

*This folder is preserved solely for reference.*