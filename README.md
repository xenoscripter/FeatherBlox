# FeatherBlox 2

A fresh FeatherBlox launcher for old Linux PCs.

## Target hardware

Designed around low-end x86-64 systems such as the eMachines E725:
- Intel Pentium Dual-Core T4400
- Intel GM45 integrated graphics
- OpenGL 2.1 host graphics
- No Vulkan dependency in FeatherBlox itself

## Roblox runtime

FeatherBlox 2 uses Cordial as the Roblox-compatible runtime. Cordial runs the official Roblox Android build natively on Linux and supports an OpenGL ES graphics path; Vulkan is an optional renderer path in Cordial, not a required renderer for the Roblox client.

FeatherBlox does not ship Roblox, bypass Roblox security, or modify Roblox. You must obtain the Roblox Android build through Cordial's normal setup.

## Install

On Ubuntu/Mint/Debian-based Linux:

```bash
sudo apt update
sudo apt install flatpak build-essential libsdl2-dev
```

Install Cordial:

```bash
flatpak remote-add --if-not-exists cordial https://luohoa97.github.io/cordial/cordial.flatpakrepo
flatpak install cordial io.github.luohoa97.Cordial
```

Build and install FeatherBlox:

```bash
chmod +x build.sh install.sh
./install.sh
```

Run:

```bash
featherblox
```

Press Enter to start Cordial.

## Important compatibility note

The GM45 is extremely old. Your Linux Mesa stack reports OpenGL 2.1. Roblox Android officially requires at least OpenGL ES 3.0 on supported Android devices. Cordial's GLES2/EGL path is therefore the experimental path worth testing on this hardware, but this project cannot guarantee that Roblox will be playable on the E725.

## Goals

- Lightweight launcher
- No Vulkan dependency in FeatherBlox
- No Wine dependency
- No Sober dependency
- No emulator dependency
- Installed Cordial backend
- Old-PC friendly
