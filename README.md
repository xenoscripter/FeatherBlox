# FeatherBlox

A lightweight independent Roblox-style sandbox client designed around very old hardware.

## Target
- OpenGL 2.1
- Fixed-function OpenGL
- SDL2
- Linux x86_64
- Intel GM45-class hardware

This project does not connect to or load official Roblox experiences.

## Build

```bash
sudo apt update
sudo apt install build-essential libsdl2-dev
./build.sh
./build/featherblox
```

## Controls
- WASD: move
- Mouse: look
- Space: move up
- Ctrl: move down
- Shift: move faster
- Esc: quit

## Roadmap
- Chunked voxel renderer
- Local maps
- Lua scripting
- Lightweight multiplayer client/server
- Custom asset format
- Low-end graphics presets
