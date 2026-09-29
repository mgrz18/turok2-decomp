#!/bin/bash
# Run decomp-permuter on a directory made by tools/permuter/setup.py.
#
#   tools/permuter/run.sh <func> [permuter flags, e.g. --stop-on-zero]
#
# Runs in the turok2-permuter image with every core; candidates that score
# better land in build/permuter/<func>/output-<score>-<n>/.
set -euo pipefail
cd "$(dirname "$0")/../.."
FUNC=$1
shift
exec docker run --platform=linux/amd64 --rm -v "$PWD:/work" -w /work turok2-permuter \
    python3 references/decomp-permuter/permuter.py "build/permuter/$FUNC" -j4 "$@"
