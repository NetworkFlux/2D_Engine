# 2D Engine

## Why I'm Building This

I'm building this 2D engine from scratch, in C, because I'm sick of not being able to display things on the screen using my C programs. C is my main language — I love the level of control it gives me — but everything always ends up in the terminal. I want a real UI. I want to draw, render, create. And I want to understand what's going on at the lowest level, without hiding behind massive libraries or abstract engines.

This project is both an act of learning and rebellion. I don't want to rely on big frameworks or rendering APIs like OpenGL, Vulkan, or Direct3D — at least not initially. I want to see what it really takes to push pixels to the screen. I want to struggle a bit and learn deeply. If that makes things harder, good. That’s part of the reason I chose C in the first place.

## Goals

- Make a 2D rendering engine from scratch
- Understand the basics of how rendering works at a low level
- Avoid using OpenGL/Direct3D/Vulkan unless absolutely necessary
- Learn how to interact with native windowing systems
- Build a minimal, clean, modular architecture
- Keep the code portable across Linux, Windows, and macOS

## Architecture Philosophy

The project is structured in modules. Each module does one thing and communicates with others through clean interfaces. Platform-dependent code will be isolated from the rest of the engine, so I can keep the core logic platform-independent.

The idea is to have:

- A **window management** module (platform-specific)
- A **renderer** module that draws into a memory buffer
- A **platform-specific presentation** layer (e.g. `XPutImage` on Linux) to blit the buffer to screen
- Later: input handling, texture management, scenes, etc.

Everything will be designed around **manual control** and **explicit data flow**. No magic, no giant engines. Just me, C, and the machine.

## Portability

For now, I'm starting on Linux using X11 and `XPutImage`. This is obviously not portable, so I’ll later write corresponding backend code for:

- Windows (likely using GDI)
- macOS (CoreGraphics, maybe Cocoa if needed)

The renderer will call a generic `present()` function, and the implementation will depend on the OS.

---

This is a learning project first and foremost. I don’t expect to reinvent the wheel, but I want to know exactly how the wheel turns.


