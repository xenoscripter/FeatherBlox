# FeatherBlox

A lightweight independent Roblox-style sandbox client designed for very old Linux hardware.

## What it is

FeatherBlox is its **own** client and game project. It does not connect to, load, or impersonate official Roblox experiences, and it does not include Roblox's proprietary engine, authentication, assets, or networking.

## Target hardware

- OpenGL 2.1
- Fixed-function OpenGL
- SDL2
- Linux x86_64
- Intel GM45-class integrated graphics
- Low CPU/RAM overhead

## Build on Debian/Ubuntu/MX Linux

Install dependencies:

```bash
sudo apt update
sudo apt install build-essential libsdl2-dev libglu1-mesa-dev
```

Build and run:

```bash
./build.sh
./build/featherblox
```

Or install it into your user application menu:

```sh
sh install.sh
```

Then run:

```bash
featherblox
```

To remove the installed copy:

```sh
sh uninstall.sh
```

## Controls

- **WASD** — move
- **Mouse** — look
- **Space** — move up
- **Ctrl** — move down
- **Shift** — move faster
- **Esc** — quit

## Current client

The current prototype renders a small low-poly sandbox using legacy OpenGL calls so it can target older GPUs such as Intel GM45.

## Roadmap

- Chunked voxel renderer
- Local maps
- Lua scripting
- Lightweight multiplayer client/server
- Custom asset format
- Low-end graphics presets
- Configurable resolution and FPS cap
