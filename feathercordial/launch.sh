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
exec flatpak run io.github.luohoa97.Cordial
