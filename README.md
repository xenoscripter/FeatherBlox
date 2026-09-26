# FeatherBlox

FeatherBlox is a lightweight launcher for Roblox on Linux.

## Roblox runtime

FeatherBlox uses **Cordial** as its Roblox-compatible runtime instead of Sober. Cordial is an independent, open-source Linux Roblox runtime and launcher. It requires the Roblox Android build and does not ship Roblox itself.

Cordial currently targets x86-64 Linux and Wayland; X11 is supported through Flatpak's fallback socket but is not its primary development target.

## Install the runtime

Install Flatpak if needed:

```bash
sudo apt update
sudo apt install flatpak
```

Add Cordial's Flatpak remote:

```bash
flatpak remote-add --if-not-exists cordial https://luohoa97.github.io/cordial/cordial.flatpakrepo
flatpak install cordial io.github.luohoa97.Cordial
```

Then build FeatherBlox:

```bash
chmod +x build.sh install.sh
./install.sh
```

Run:

```bash
export PATH="$HOME/.local/bin:$PATH"
featherblox
```

Press **Enter** in the launcher to start Cordial.

## Important hardware note

Your eMachines E725 has Intel GM45 graphics with OpenGL 2.1. Current Roblox Linux runtimes have substantially higher graphics/runtime requirements than the old FeatherBlox OpenGL 2.1 prototype. Cordial/Sober compatibility therefore cannot be guaranteed on this laptop.

FeatherBlox does not bypass Roblox security or Hyperion. It simply provides a lightweight launcher around the installed runtime.

## Controls

- **Enter** — launch Roblox through Cordial
- **C** — check runtime installation
- **I** — show installation commands
- **Esc** — quit
