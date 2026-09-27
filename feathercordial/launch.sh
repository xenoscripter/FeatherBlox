#!/bin/sh
set -eu

APP_ID="io.github.luohoa97.Cordial"
QEMU_DIR="${HOME}/.local/share/FeatherBlox"
QEMU_BIN="${QEMU_DIR}/qemu-x86_64-static"
CORDIAL_CACHE="${HOME}/.var/app/${APP_ID}/cache/cordial"
APK="${CORDIAL_CACHE}/build/x86_64/base.apk"
LIBDIR="${CORDIAL_CACHE}/lib/x86_64"

if ! command -v flatpak >/dev/null 2>&1; then
  echo "Flatpak is required."
  exit 1
fi

if ! flatpak info "${APP_ID}" >/dev/null 2>&1; then
  echo "Cordial is not installed."
  echo "Install it from the FeatherBlox UI."
  exit 1
fi

prepare_qemu() {
  if [ -x "${QEMU_BIN}" ]; then
    return 0
  fi

  QEMU_SOURCE=""
  if [ -x /usr/bin/qemu-x86_64-static ]; then
    QEMU_SOURCE=/usr/bin/qemu-x86_64-static
  elif [ -x /usr/bin/qemu-x86_64 ]; then
    QEMU_SOURCE=/usr/bin/qemu-x86_64
  elif command -v qemu-x86_64-static >/dev/null 2>&1; then
    QEMU_SOURCE="$(command -v qemu-x86_64-static)"
  elif command -v qemu-x86_64 >/dev/null 2>&1; then
    QEMU_SOURCE="$(command -v qemu-x86_64)"
  fi

  if [ -z "${QEMU_SOURCE}" ]; then
    echo "QEMU x86-64 is not installed."
    echo "Install it with: sudo apt install qemu-user qemu-user-static"
    exit 1
  fi

  mkdir -p "${QEMU_DIR}"
  cp "${QEMU_SOURCE}" "${QEMU_BIN}"
  chmod 0755 "${QEMU_BIN}"
}

launch_qemu() {
  prepare_qemu

  if [ ! -f "${APK}" ]; then
    echo "Cordial Roblox APK cache not found:"
    echo "  ${APK}"
    echo "Launch Cordial normally once so it can install/extract the Roblox build."
    exit 1
  fi

  if [ ! -f "${LIBDIR}/libroblox.so" ]; then
    echo "Cordial Roblox engine not found:"
    echo "  ${LIBDIR}/libroblox.so"
    echo "Launch Cordial normally once so it can prepare the Roblox build."
    exit 1
  fi

  echo "Starting FeatherBlox CPU Compatibility Mode..."
  echo "QEMU x86-64 TCG: enabled"
  echo "CPU: virtual x86-64 with modern instruction support"
  echo "Graphics: Mesa software rendering (llvmpipe)"
  echo "Warning: this will be extremely slow on a Pentium T4400."
  echo
  echo "Launching Cordial's loader under QEMU..."

  exec flatpak run \
    --filesystem="${QEMU_DIR}:ro" \
    --env=LIBGL_ALWAYS_SOFTWARE=true \
    --env=GALLIUM_DRIVER=llvmpipe \
    --env=CORDIAL_X11=1 \
    --command="${QEMU_BIN}" \
    "${APP_ID}" \
    -cpu max \
    -L / \
    /app/bin/cordial-run \
    --lib-dir "${LIBDIR}" \
    --apk "${APK}" \
    --host-libc \
    --game-activity \
    --run 0
}

case "${1:-normal}" in
  compatibility|qemu|legacy)
    launch_qemu
    ;;
  *)
    exec flatpak run "${APP_ID}"
    ;;
esac
