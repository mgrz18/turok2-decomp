#!/bin/bash
# setup once, then retry the incremental verify until it links (qemu segfaults)
# usage: tools/verify_loop.sh <tag>
cd "$(dirname "$0")/.."
T=$1
run() { timeout 1800 docker run --platform=linux/amd64 --rm -v "$PWD:/work" -w /work turok2-build "$@"; }
for i in 1 2 3; do run bash -c "make setup >/dev/null 2>&1" && break; done
for i in $(seq 1 25); do
  run make verify > build/verify$T.log 2>&1 && break
  # a real failure: stop, don't retry
  grep -qE "expected:|undefined reference|Error: |error: " build/verify$T.log && ! grep -q "Segmentation fault" build/verify$T.log && break
  docker ps -q --filter ancestor=turok2-build | xargs -r docker kill >/dev/null 2>&1
  # a killed link leaves an empty ELF that make would take as up to date
  [ -s build/turok2.us.elf ] || rm -f build/turok2.us.elf build/turok2.us.z64
done
{ echo "tries=$i"; tail -1 build/verify$T.log; ./.venv/bin/python tools/progress.py | grep -E "engine|virtual"; } > build/verify$T.out 2>&1
