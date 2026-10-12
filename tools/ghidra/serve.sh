#!/bin/bash
# Serve the Ghidra project headless, through GhidraMCP (LaurieWired, 2.0).
#
#   tools/ghidra/serve.sh            # foreground; Ctrl-C saves and quits
#
# Opens the already-analysed program in the project, without the GUI, and
# serves GhidraMCP's HTTP API (decompile, set_function_prototype,
# rename_function_by_address, set_decompiler_comment...) on a free port,
# printed as "Serving turok2.us.elf on http://127.0.0.1:PORT/", and an MCP
# endpoint on 127.0.0.1:8081/mcp. Changes are saved on exit (--save).
#
# The GUI cannot open the project while this runs: Ghidra locks it.
#
# Paths default to this machine's setup; override with the variables below.
set -euo pipefail
GHIDRA_PROJECT_DIR=${GHIDRA_PROJECT_DIR:-$HOME/Documents/Proyectos/turok2_ghidra}
GHIDRA_INSTALL_DIR=${GHIDRA_INSTALL_DIR:-/opt/homebrew/Cellar/ghidra/12.1.4/libexec}
GHIDRA_MCP_WHEEL=${GHIDRA_MCP_WHEEL:-$HOME/Downloads/GhidraMCP/GhidraMCP-release-2-0/ghidra_mcp-2.0.0-py3-none-any.whl}
export JAVA_HOME=${JAVA_HOME:-/opt/homebrew/opt/openjdk@21/libexec/openjdk.jdk/Contents/Home}
exec uvx --from "$GHIDRA_MCP_WHEEL" --with pyghidra ghidra-mcp --headless \
    --binary "$GHIDRA_PROJECT_DIR/turok2.us.elf" \
    --project "$GHIDRA_PROJECT_DIR" --project-name turok2 \
    --no-analyze --save --ghidra-install-dir "$GHIDRA_INSTALL_DIR" \
    --transport streamable-http --mcp-port 8081
