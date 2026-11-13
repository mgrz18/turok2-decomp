#!/bin/sh
# mkapp.sh BUILD_DIR [APP] - a macOS app bundle of the port (round 14 c4, docs/port/QUICKSTART.md).
#
#   tools/port/mkapp.sh OUT/rel                 -> OUT/Turok2.app
#   tools/port/mkapp.sh OUT/rel ~/Applications/Turok2.app
#
# The bundle holds the executable and t2port.o2r (the port's shaders) only. The game data is
# yours: put turok2.o2r (tools/assets/mkarchive.py) and baserom.us.z64 in
# ~/Library/Application Support/turok2 (or in the bundle's Contents/Resources). A Finder launch
# has no arguments: the game starts in play mode (a window, sound, real time); the config, the
# logs and the Controller Pak are in ~/Library/Application Support/turok2.
#
# The executable links Homebrew's SDL3, libzip, GLEW, libpng and tinyxml2 dynamically: the
# bundle runs on Macs that have them (`brew install sdl3 libzip glew libpng tinyxml2`).
set -e
B=${1:?usage: mkapp.sh BUILD_DIR [APP]}
APP=${2:-$(dirname "$B")/Turok2.app}
[ -x "$B/app/turok2" ] || { echo "mkapp.sh: $B/app/turok2 not built" >&2; exit 1; }
[ -f "$B/t2port.o2r" ] || { echo "mkapp.sh: $B/t2port.o2r not built" >&2; exit 1; }
rm -rf "$APP"
mkdir -p "$APP/Contents/MacOS" "$APP/Contents/Resources"
cp "$B/app/turok2" "$APP/Contents/MacOS/turok2"
cp "$B/t2port.o2r" "$APP/Contents/Resources/t2port.o2r"
cat > "$APP/Contents/Info.plist" <<'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleDevelopmentRegion</key><string>en</string>
    <key>CFBundleExecutable</key><string>turok2</string>
    <key>CFBundleIdentifier</key><string>io.github.turok2-port</string>
    <key>CFBundleInfoDictionaryVersion</key><string>6.0</string>
    <key>CFBundleName</key><string>Turok 2</string>
    <key>CFBundleDisplayName</key><string>Turok 2: Seeds of Evil</string>
    <key>CFBundlePackageType</key><string>APPL</string>
    <key>CFBundleShortVersionString</key><string>0.14</string>
    <key>CFBundleVersion</key><string>14</string>
    <key>LSMinimumSystemVersion</key><string>13.0</string>
    <key>LSApplicationCategoryType</key><string>public.app-category.action-games</string>
    <key>NSHighResolutionCapable</key><true/>
    <key>GCSupportsControllerUserInteraction</key><true/>
</dict>
</plist>
EOF
# an ad-hoc signature over the whole bundle (arm64 needs one; nothing is notarized)
codesign --force --sign - "$APP" >/dev/null 2>&1 || echo "mkapp.sh: codesign failed (the bundle may still run)" >&2
echo "$APP"
