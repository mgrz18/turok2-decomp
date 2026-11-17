#!/bin/sh
# mkapp.sh BUILD_DIR [APP] - a self-contained macOS app bundle of the port
# (round 14 c4, round 15 d4; docs/port/QUICKSTART.md, MACOS.md section 17).
#
#   tools/port/mkapp.sh OUT/rel                 -> OUT/Turok2.app
#   tools/port/mkapp.sh OUT/rel ~/Applications/Turok2.app
#
# The bundle holds the executable, t2port.o2r (the port's shaders) and every dylib the
# executable needs that is not part of macOS (Homebrew's SDL3, GLEW, libzip, tinyxml2 and what
# they link: liblzma, libzstd ...), copied into Contents/Frameworks and relinked to
# @rpath (= @executable_path/../Frameworks), so the app runs on a Mac without Homebrew. The
# whole bundle gets an ad-hoc signature (nothing is notarized: see QUICKSTART.md for the first
# launch). The script fails if anything in the bundle still points outside macOS.
#
# The game data is yours: put turok2.o2r (tools/assets/mkarchive.py) and baserom.us.z64 in
# ~/Library/Application Support/turok2 (or in the bundle's Contents/Resources). A Finder launch
# has no arguments: the game starts in play mode (a window, sound, real time); the config, the
# logs and the Controller Pak are in ~/Library/Application Support/turok2.
#
# Env: T2_APP_VERSION (CFBundleShortVersionString, default 0.15), T2_APP_BUILD (CFBundleVersion,
# default 15), T2_APP_ZIP=1 (also Turok2-VERSION-macos-arm64.zip next to the app, made with
# ditto so the signature survives).
set -e
B=${1:?usage: mkapp.sh BUILD_DIR [APP]}
APP=${2:-$(dirname "$B")/Turok2.app}
VER=${T2_APP_VERSION:-0.15}
BUILD=${T2_APP_BUILD:-15}
[ -x "$B/app/turok2" ] || { echo "mkapp.sh: $B/app/turok2 not built" >&2; exit 1; }
[ -f "$B/t2port.o2r" ] || { echo "mkapp.sh: $B/t2port.o2r not built" >&2; exit 1; }
rm -rf "$APP"
mkdir -p "$APP/Contents/MacOS" "$APP/Contents/Resources" "$APP/Contents/Frameworks"
EXE="$APP/Contents/MacOS/turok2"
FW="$APP/Contents/Frameworks"
cp "$B/app/turok2" "$EXE"
cp "$B/t2port.o2r" "$APP/Contents/Resources/t2port.o2r"
chmod u+w "$EXE"

# system = shipped with macOS (or the bundle's own references)
is_system() {
    case "$1" in
    /System/* | /usr/lib/* | @rpath/* | @executable_path/* | @loader_path/*) return 0 ;;
    esac
    return 1
}
# the non-system install names a Mach-O links (not its own id)
deps() {
    id=$(otool -D "$1" | sed -n 2p)
    otool -L "$1" | tail -n +2 | awk '{print $1}' | while read -r d; do
        [ "$d" = "$id" ] && continue
        is_system "$d" || echo "$d"
    done
}

# copy the dependency closure into Frameworks, relinking each reference to @rpath/NAME
todo="$EXE"
done_list=""
while [ -n "$todo" ]; do
    f=${todo%%"
"*}
    [ "$f" = "$todo" ] && todo="" || todo=${todo#*"
"}
    for d in $(deps "$f"); do
        n=$(basename "$d")
        if [ ! -f "$FW/$n" ]; then
            src=$d
            case "$d" in /*) ;; *) echo "mkapp.sh: $f links $d (relative): not handled" >&2; exit 1 ;; esac
            [ -f "$src" ] || { echo "mkapp.sh: $d (needed by $f) not found" >&2; exit 1; }
            cp -L "$src" "$FW/$n"
            chmod u+w "$FW/$n"
            install_name_tool -id "@rpath/$n" "$FW/$n" 2>/dev/null
            todo="$todo${todo:+
}$FW/$n"
            done_list="$done_list $n"
        fi
        install_name_tool -change "$d" "@rpath/$n" "$f" 2>/dev/null
    done
done
# the executable finds them through one rpath; drop the build's rpaths (/opt/homebrew/lib ...)
for r in $(otool -l "$EXE" | awk '/LC_RPATH/{f=1} f&&/ path /{print $2; f=0}'); do
    install_name_tool -delete_rpath "$r" "$EXE" 2>/dev/null
done
install_name_tool -add_rpath "@executable_path/../Frameworks" "$EXE" 2>/dev/null
# dylibs find their siblings through the executable's rpath; drop their own build rpaths
for l in "$FW"/*.dylib; do
    [ -f "$l" ] || continue
    for r in $(otool -l "$l" | awk '/LC_RPATH/{f=1} f&&/ path /{print $2; f=0}'); do
        install_name_tool -delete_rpath "$r" "$l" 2>/dev/null
    done
done

# nothing may point outside macOS any more
bad=0
for m in "$EXE" "$FW"/*.dylib; do
    [ -f "$m" ] || continue
    for d in $(deps "$m"); do
        echo "mkapp.sh: $m still links $d" >&2
        bad=1
    done
    if otool -l "$m" | awk '/LC_RPATH/{f=1} f&&/ path /{print $2; f=0}' | grep -qv '^@'; then
        echo "mkapp.sh: $m keeps a non-bundle rpath" >&2
        bad=1
    fi
done
[ $bad = 0 ] || exit 1

# the oldest macOS every Mach-O in the bundle accepts (Homebrew builds for the host's release)
minos=$(for m in "$EXE" "$FW"/*.dylib; do
    [ -f "$m" ] && otool -l "$m" | awk '/LC_BUILD_VERSION/{f=1} f&&/minos/{print $2; f=0}'
done | sort -t. -k1,1n -k2,2n -k3,3n | tail -1)
minos=${minos:-13.0}

cat > "$APP/Contents/Info.plist" <<EOF
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
    <key>CFBundleShortVersionString</key><string>$VER</string>
    <key>CFBundleVersion</key><string>$BUILD</string>
    <key>CFBundleSupportedPlatforms</key><array><string>MacOSX</string></array>
    <key>LSMinimumSystemVersion</key><string>$minos</string>
    <key>LSArchitecturePriority</key><array><string>arm64</string></array>
    <key>LSApplicationCategoryType</key><string>public.app-category.action-games</string>
    <key>NSHighResolutionCapable</key><true/>
    <key>NSSupportsAutomaticGraphicsSwitching</key><true/>
    <key>GCSupportsControllerUserInteraction</key><true/>
    <key>GCSupportedGameControllers</key>
    <array>
        <dict><key>ProfileName</key><string>ExtendedGamepad</string></dict>
    </array>
    <key>NSHumanReadableCopyright</key><string>A fan source port. Turok 2 is a trademark of its owners; bring your own ROM.</string>
</dict>
</plist>
EOF
plutil -lint "$APP/Contents/Info.plist" >/dev/null

# an ad-hoc signature: each dylib, then the bundle (arm64 needs one; nothing is notarized)
for l in "$FW"/*.dylib; do
    [ -f "$l" ] && codesign --force --sign - "$l" >/dev/null 2>&1
done
codesign --force --deep --sign - "$APP" >/dev/null 2>&1 || { echo "mkapp.sh: codesign failed" >&2; exit 1; }
codesign --verify --deep --strict "$APP" || { echo "mkapp.sh: codesign --verify failed" >&2; exit 1; }
echo "$APP (bundled:$done_list; LSMinimumSystemVersion $minos)"
if [ "${T2_APP_ZIP:-0}" = 1 ]; then
    Z="$(dirname "$APP")/Turok2-$VER-macos-arm64.zip"
    rm -f "$Z"
    ditto -c -k --sequesterRsrc --keepParent "$APP" "$Z"
    echo "$Z"
fi
