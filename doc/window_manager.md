# Window Manager — Design and Responsibilities

## Why This Document Exists

This file exists to clarify the role of the Window Manager within this engine and help guide its design, implementation, and boundaries. It ensures that we build something cohesive and maintainable, especially with platform independence in mind.

---

## Overview

The Window Manager is the engine’s **interface to the operating system’s graphical window system**. Its job is to:
- Create and manage the window(s)
- Handle OS-level event polling (including input)
- Provide a platform-agnostic interface to the rest of the engine

It acts as the **bridge between the platform and the engine core**, and must isolate all platform-specific behavior (e.g. X11, Win32, Cocoa).

---

## Core Responsibilities

✅ **1. Window Creation & Destruction**
- Initialize a native window using the target platform’s API (X11, etc.)
- Store references to system resources (Display*, Window, etc.)
- Allow proper cleanup (destroy window, close connection)

✅ **2. Event Polling**
- Read OS events from the system event queue (e.g. XPending/XNextEvent on X11)
- Forward events to the engine via an internal queue or callback
- Events include:
  - Keyboard input
  - Mouse movement/clicks
  - Window close requests
  - Window resize, expose, focus, etc.

✅ **3. Frame Presentation**
- Provide a function that can present a final image buffer to the window (e.g., using `XPutImage` for X11)
- This abstracts away platform-specific presentation details from the renderer

✅ **4. Input Collection (Low-level)**
- Collect raw input events from the platform
- These events should be passed to the Input Manager (which interprets them)
- The Input Manager should **not** talk directly to the platform

---

## Design Principles

📦 **Modularity**  
- All platform-specific code stays in the window manager.
- Other systems (input, renderer) only interact with clean, abstracted interfaces.

🌐 **Platform Independence**  
- The goal is to support multiple platforms in the future (Linux, Windows, macOS).
- Each backend implementation should follow the same interface contract.

🔁 **Event Forwarding**  
- Window Manager forwards raw, generic events (e.g., `KEY_DOWN`, `WINDOW_RESIZE`) into the engine.
- Other subsystems subscribe or query for these events.

---
