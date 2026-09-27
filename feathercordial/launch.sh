#!/bin/sh
set -eu

if ! command -v flatpak >/dev/null 2>&1; then
  echo "Flatpak is required."
  exit 1
fi

if ! flatpak info io.github.luohoa97.Cordial >/dev/null 2>&1; then
  echo "Cordial is not installed."
  echo "Install it from the FeatherBlox UI."
  exit 1
fi

case "${1:-normal}" in
  legacy)
    echo "FeatherBlox Legacy T4400 Mode"
    echo "Using Cordial's X11 fallback."
    echo "Note: this does not add SSE4.1/AVX instructions to the CPU."
    exec flatpak run --env=CORDIAL_X11=1 io.github.luohoa97.Cordial
    ;;
  *)
    exec flatpak run io.github.luohoa97.Cordial
    ;;
esac
