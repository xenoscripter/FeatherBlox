#!/bin/sh
set -eu
PREFIX="${HOME}/.local"
APP_DIR="${PREFIX}/share/featherblox"
BIN_DIR="${PREFIX}/bin"
APP_MENU="${PREFIX}/share/applications"
ICON_DIR="${PREFIX}/share/icons/hicolor/scalable/apps"

if ! command -v cc >/dev/null 2>&1 || ! command -v sdl2-config >/dev/null 2>&1; then
  echo "Missing build dependencies."
  echo "Run: sudo apt install build-essential libsdl2-dev libsdl2-ttf-dev"
  exit 1
fi

./build.sh
mkdir -p "${APP_DIR}/bin" "${BIN_DIR}" "${APP_MENU}" "${ICON_DIR}"
cp build/featherblox "${APP_DIR}/bin/featherblox"
cp featherblox.desktop "${APP_MENU}/featherblox.desktop"
cp featherblox.svg "${ICON_DIR}/featherblox.svg"

cat > "${BIN_DIR}/featherblox" <<EOF
#!/bin/sh
exec "${APP_DIR}/bin/featherblox" "$@"
EOF
chmod +x "${BIN_DIR}/featherblox"

echo "FeatherBlox 2 installed."
echo "Run: featherblox"
