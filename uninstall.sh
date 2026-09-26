#!/bin/sh
set -eu
rm -f "${HOME}/.local/bin/featherblox"
rm -f "${HOME}/.local/share/applications/featherblox.desktop"
rm -f "${HOME}/.local/share/icons/hicolor/scalable/apps/featherblox.svg"
rm -rf "${HOME}/.local/share/featherblox"
echo "FeatherBlox removed."
