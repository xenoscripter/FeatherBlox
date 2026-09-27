#!/bin/sh
set -eu

APP_ID="io.github.luohoa97.Cordial"

if ! command -v flatpak >/dev/null 2>&1; then
  echo "Flatpak is required."
  exit 1
fi

if ! flatpak info "$APP_ID" >/dev/null 2>&1; then
  echo "Cordial is not installed."
  echo "Install it from the FeatherBlox UI."
  exit 1
fi

case "${1:-normal}" in
  legacy)
    echo "FeatherBlox Legacy T4400 Mode"
    echo "Using Cordial's X11 fallback."
    echo "Note: X11 alone does not emulate missing CPU instructions."
    exec flatpak run --env=CORDIAL_X11=1 "$APP_ID"
    ;;

  qemu|compat|t4400)
    echo "FeatherBlox T4400 CPU Compatibility Mode"
    echo "Using QEMU x86-64 TCG CPU emulation."
    echo "Using Mesa software rendering (llvmpipe)."
    echo "This may be extremely slow on a Pentium T4400."
    echo

    if ! command -v qemu-x86_64 >/dev/null 2>&1; then
      echo "qemu-x86_64 is missing."
      echo "Install it with:"
      echo "  sudo apt install qemu-user qemu-user-static"
      exit 1
    fi

    # Flatpak reserves /usr, so the host QEMU binary cannot be exposed there.
    # host-os mounts the host's /usr read-only under /run/host/usr.
    if ! flatpak run --filesystem=host-os:ro --command=/bin/sh "$APP_ID"         -c 'test -x /run/host/usr/bin/qemu-x86_64'; then
      echo "Flatpak could not access the host QEMU binary."
      echo "Your Flatpak version must support --filesystem=host-os."
      exit 1
    fi

    # QEMU user-mode TCG translates the guest CPU instructions instead of
    # executing unsupported SSE4.x instructions directly on the T4400.
    # The Cordial shell and its child cordial-run stay inside the same
    # emulated process tree.
    exec flatpak run       --filesystem=host-os:ro       --env=CORDIAL_X11=1       --env=LIBGL_ALWAYS_SOFTWARE=1       --env=GALLIUM_DRIVER=llvmpipe       --command=/bin/sh       "$APP_ID"       -c 'exec /run/host/usr/bin/qemu-x86_64 -cpu max /app/bin/cordial-shell'
    ;;

  *)
    exec flatpak run "$APP_ID"
    ;;
esac
