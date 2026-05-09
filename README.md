# SDL2 Cross-Platform App

A small C++/SDL2 rendering experiment that builds as a native desktop app or as a WebAssembly app with Emscripten.

The project code lives in `src/`. SDL2 is vendored under `ext/SDL2-2.30.2/`.

## What It Does

The app opens an SDL window or browser canvas and renders an animated abstract scene:

- rotating color-cycling radial lines
- pulsing rings
- orbiting blocks
- close-window and `Esc` quit handling

## Requirements

For native macOS builds:

- CMake
- a C++ compiler, such as Apple Clang from Xcode Command Line Tools

For WebAssembly builds:

- CMake
- Emscripten, with `emcmake` and `emrun` available on `PATH`

## Build

Use the Python helper from the repo root.

Native macOS debug build:

```sh
python3 builder.py macos
```

WebAssembly debug build:

```sh
python3 builder.py web
```

Release builds:

```sh
python3 builder.py macos --type release
python3 builder.py web --type release
```

## Run

Run the web build with Emscripten's local server:

```sh
python3 builder.py web --run
```

To force a specific browser:

```sh
python3 builder.py web --run --browser "/path/to/browser"
```

For macOS, run the installed binary after building:

```sh
./out/bin/app
```

You can quit the running app by closing the window or pressing `Esc`.

## Project Layout

```text
.
├── CMakeLists.txt              # Top-level platform configuration
├── builder.py                  # Small build helper for macOS and web targets
├── simple_shell.html           # Emscripten HTML shell
├── src/
│   ├── CMakeLists.txt          # App executable and dependency wiring
│   ├── main.cpp                # Platform-aware main loop entry point
│   └── renderloop/
│       ├── CMakeLists.txt      # Render library target
│       ├── renderloop.hpp      # Render class interface
│       └── renderloop.cpp      # SDL setup, events, drawing, shutdown
└── ext/
    └── SDL2-2.30.2/            # Vendored SDL2 source
```
