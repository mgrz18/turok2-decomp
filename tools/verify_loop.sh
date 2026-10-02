#!/bin/bash
# Build and verify the ROM, splitting the work by image:
#
#   turok2-native (arm64)  make setup, then make verify: splat, GNU as, ld,
#                          objcopy, n64crc. Native on Apple Silicon.
#   turok2-build  (amd64)  make build: the objects, since cc1 is an i386
#                          binary. Under qemu, which segfaults now and then,
#                          so it is retried; make is incremental.
#
# Under qemu the link of a ROM with ~1,600 C files outlasted a 30-minute
# timeout; natively it takes a few minutes. Without the native image
# (docker build --platform linux/arm64 -f Dockerfile.native -t turok2-native .)
# everything runs in turok2-build as before.
#
# usage: tools/verify_loop.sh <tag>   ->  build/verify<tag>.log, build/verify<tag>.out
cd "$(dirname "$0")/.."
T=$1
amd() { timeout 1800 docker run --platform=linux/amd64 --rm -v "$PWD:/work" -w /work turok2-build "$@"; }
if docker image inspect turok2-native >/dev/null 2>&1; then
  nat() { timeout 1800 docker run --rm -v "$PWD:/work" -w /work turok2-native "$@"; }
else
  nat() { amd "$@"; }
fi

for i in 1 2 3; do nat bash -c "make setup >/dev/null 2>&1" && break; done
for i in $(seq 1 25); do
  amd make -j4 build > build/verify$T.log 2>&1 && break
  # a real compile error: stop, don't retry
  grep -qE "Error: |error: " build/verify$T.log && ! grep -q "Segmentation fault" build/verify$T.log && break
  docker ps -q --filter ancestor=turok2-build | xargs -r docker kill >/dev/null 2>&1
done
nat make verify >> build/verify$T.log 2>&1
{ echo "build tries=$i"; tail -1 build/verify$T.log; ./.venv/bin/python tools/progress.py | grep -E "engine|virtual"; } > build/verify$T.out 2>&1
