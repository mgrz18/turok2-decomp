#!/bin/bash
# Serve the Ghidra project headless, through GhidraMCP (LaurieWired, 2.0).
#
#   tools/ghidra/serve.sh            # foreground; logs to build/ghidra_serve.log
#
# Opens the already-analysed program in the project, without the GUI, and
# serves GhidraMCP's HTTP API (decompile, set_function_prototype,
# rename_function_by_address, set_decompiler_comment...) on a free port,
# logged as "Serving turok2.us.elf on http://127.0.0.1:PORT/", and an MCP
# endpoint on 127.0.0.1:8081/mcp.
#
# Changes made through the API do not survive the server: stopping it (by
# SIGTERM or SIGINT) does not save them. So the repo keeps them
# (versions/names.us.txt, tools/ghidra/annotations.json) and this applies
# them every time the server comes up, with tools/ghidra/annotate.py. Add
# what you learn there, not only in Ghidra. Structs are loaded into the
# project itself, with the server stopped: tools/ghidra/load_types.py.
#
# The GUI cannot open the project while this runs: Ghidra locks it.
#
# Paths default to this machine's setup; override with the variables below.
set -uo pipefail
cd "$(dirname "$0")/../.."
GHIDRA_PROJECT_DIR=${GHIDRA_PROJECT_DIR:-$HOME/Documents/Proyectos/turok2_ghidra}
GHIDRA_INSTALL_DIR=${GHIDRA_INSTALL_DIR:-/opt/homebrew/Cellar/ghidra/12.1.4/libexec}
GHIDRA_MCP_WHEEL=${GHIDRA_MCP_WHEEL:-$HOME/Downloads/GhidraMCP/GhidraMCP-release-2-0/ghidra_mcp-2.0.0-py3-none-any.whl}
export JAVA_HOME=${JAVA_HOME:-/opt/homebrew/opt/openjdk@21/libexec/openjdk.jdk/Contents/Home}
LOG=build/ghidra_serve.log
mkdir -p build
uvx --from "$GHIDRA_MCP_WHEEL" --with pyghidra ghidra-mcp --headless \
    --binary "$GHIDRA_PROJECT_DIR/turok2.us.elf" \
    --project "$GHIDRA_PROJECT_DIR" --project-name turok2 \
    --no-analyze --save --ghidra-install-dir "$GHIDRA_INSTALL_DIR" \
    --transport streamable-http --mcp-port 8081 > "$LOG" 2>&1 &
SERVER=$!
trap 'kill -INT $SERVER 2>/dev/null; wait $SERVER' INT TERM
for _ in $(seq 1 120); do
    grep -q "Serving .* on http" "$LOG" 2>/dev/null && break
    kill -0 $SERVER 2>/dev/null || { cat "$LOG"; exit 1; }
    sleep 2
done
grep -o "Serving .* on http://127.0.0.1:[0-9]*/" "$LOG" | tail -1
./.venv/bin/python tools/ghidra/annotate.py --log "$LOG"
wait $SERVER
