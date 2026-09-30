#!/bin/bash
# Permute many functions in one run, for a fixed time.
#
#   tools/permuter/batch.sh <seconds> <func> [<func>...]
#
# The permuter shares its -j4 workers across the directories it is given
# (set up first with tools/permuter/setup.py --dir). A function it matches
# stops there (--stop-on-zero); collect them with tools/permuter/harvest.py.
#
# Under qemu the permuter's own cpp segfaults now and then while it loads,
# which ends the whole run, so it is restarted until the time is up; the
# candidates it saved (output-*/) stay. A function already matched is left
# out of the next start.
set -uo pipefail
cd "$(dirname "$0")/../.."
# One batch at a time: two share the CPUs for nothing.
LOCK=build/permuter/.batch.lock
if ! mkdir "$LOCK" 2>/dev/null; then
  if kill -0 "$(cat "$LOCK/pid" 2>/dev/null)" 2>/dev/null; then
    echo "another batch is running (pid $(cat "$LOCK/pid"))"; exit 1
  fi
  rm -rf "$LOCK"; mkdir "$LOCK"   # stale: its owner is gone
fi
echo $$ > "$LOCK/pid"
trap 'rm -rf "$LOCK"' EXIT
NAME=turok2-permuter-$$
SECS=$1
shift
END=$(( $(date +%s) + SECS ))
while :; do
  LEFT=$(( END - $(date +%s) ))
  [ "$LEFT" -gt 60 ] || break
  DIRS=()
  for f in "$@"; do
    ls -d "build/permuter/$f"/output-0-* >/dev/null 2>&1 || DIRS+=("build/permuter/$f")
  done
  [ ${#DIRS[@]} -gt 0 ] || break
  echo "== start: ${#DIRS[@]} functions, ${LEFT}s left"
  T0=$(date +%s)
  timeout "$LEFT" docker run --platform=linux/amd64 --rm --name "$NAME" \
      -v "$PWD:/work" -w /work turok2-permuter \
      python3 references/decomp-permuter/permuter.py -j4 --stop-on-zero "${DIRS[@]}" </dev/null
  docker kill "$NAME" >/dev/null 2>&1 || true
  # A run that dies at once is not qemu's cpp: stop instead of spinning.
  if [ $(( $(date +%s) - T0 )) -lt 15 ]; then
    FAST=$(( ${FAST:-0} + 1 ))
    [ "$FAST" -lt 5 ] || { echo "== five runs in a row died at once; giving up"; exit 1; }
  else
    FAST=0
  fi
done
