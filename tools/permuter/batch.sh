#!/bin/bash
# Permute many functions in one run, for a fixed time.
#
#   tools/permuter/batch.sh <seconds> <func> [<func>...]
#
# The permuter shares its -j4 workers across the directories it is given
# (set up first with tools/permuter/setup.py --dir). A function it matches
# stops there (--stop-on-zero); collect them with tools/permuter/harvest.py.
set -uo pipefail
cd "$(dirname "$0")/../.."
SECS=$1
shift
DIRS=()
for f in "$@"; do DIRS+=("build/permuter/$f"); done
timeout "$SECS" docker run --platform=linux/amd64 --rm --name turok2-permuter-batch \
    -v "$PWD:/work" -w /work turok2-permuter \
    python3 references/decomp-permuter/permuter.py -j4 --stop-on-zero "${DIRS[@]}"
docker kill turok2-permuter-batch >/dev/null 2>&1 || true
