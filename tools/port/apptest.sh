#!/bin/bash
# apptest.sh APP ARCHIVE ROM WORK - check that a bundle from mkapp.sh is self-contained (round 15 d4,
# MACOS.md section 17), headless: the bundle is copied under WORK/"My Games"/"Turok 2.app" (a path
# with spaces), /opt/homebrew and /usr/local are hidden by a sandbox profile, the environment is
# cleared (no DYLD_*), HOME is WORK/home with the archive and the ROM in its Application Support
# folder, and play mode starts a new game from the title with the virtual gamepad (--pad-test,
# --offscreen-gl). Prints the dylibs loaded from outside macOS (all must be the bundle's) and the
# last engine modes (11 = in level 51).
set -u
APP=${1:?usage: apptest.sh APP ARCHIVE ROM WORK}; ARC=${2:?}; ROM=${3:?}; W=${4:?}
mkdir -p "$W" && W=$(cd "$W" && pwd)
T="$W/My Games"; H="$W/home"; S="$H/Library/Application Support/turok2"
rm -rf "$T" "$H"; mkdir -p "$T" "$S"
cp -R "$APP" "$T/Turok 2.app"
cp "$ARC" "$S/turok2.o2r"; cp "$ROM" "$S/baserom.us.z64"
printf '%s\n' "4400 btn START down" "4404 btn START up" "4500 btn SOUTH down" "4504 btn SOUTH up" \
    "4700 btn SOUTH down" "4704 btn SOUTH up" "4800 btn SOUTH down" "4804 btn SOUTH up" > "$H/pad.txt"
EXE="$T/Turok 2.app/Contents/MacOS/turok2"
SB='(version 1)(allow default)(deny file-read* file-read-metadata (subpath "/opt/homebrew") (subpath "/usr/local"))'
codesign --verify --deep --strict "$T/Turok 2.app" && echo "codesign: ok"
echo "otool: $(otool -L "$EXE" "$T/Turok 2.app/Contents/Frameworks/"*.dylib | grep -c '/opt/homebrew\|/usr/local') references to /opt/homebrew or /usr/local"
cd "$H"
env -i HOME="$H" sandbox-exec -p "$SB" /usr/bin/env DYLD_PRINT_LIBRARIES=1 "$EXE" --help 2>&1 |
    awk '/^dyld\[[0-9]+\]: </ && !/ \/System\// && !/ \/usr\/lib\// {sub(/^dyld\[[0-9]+\]: <[^>]*> /, ""); print "loaded: " $0}'
start=$(date +%s)
env -i HOME="$H" PATH=/usr/bin:/bin sandbox-exec -p "$SB" "$EXE" --play --offscreen-gl --no-pace --frames 6000 \
    --pad-test "$H/pad.txt" > "$H/out.txt" 2> "$H/err.txt"
rc=$?
echo "play mode: exit $rc, $(($(date +%s) - start)) s, stdout $(wc -c < "$H/out.txt") bytes"
grep -h "engine mode" "$S/logs/turok2-stdout.log" | tail -2
exit $rc
