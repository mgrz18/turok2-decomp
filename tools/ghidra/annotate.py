#!/usr/bin/env python3
"""Apply everything the repo knows to the running Ghidra project.

Ghidra is a working copy, not the record: a session that ends badly loses
what it held (a headless server killed with SIGTERM does not save). So
every name, prototype, local variable type, data label and comment that
matters lives in the repo, and this puts it back:

  versions/names.us.txt          function names
  tools/ghidra/annotations.json  names Ghidra only (sqrtf), prototypes,
                                 typed locals, data labels, comments

It talks to tools/ghidra/serve.sh through GhidraMCP's HTTP API; the port
is read from the server's log (or give it with --url). Structs must be in
the program first: tools/ghidra/load_types.py.

Usage:
    tools/ghidra/annotate.py [--url http://127.0.0.1:PORT] [--log FILE]
"""

import argparse
import json
import re
import sys
import urllib.parse
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent


def find_url(log):
    text = Path(log).read_text(errors="replace") if log and Path(log).exists() else ""
    ports = re.findall(r"Serving \S+ on (http://127\.0\.0\.1:\d+)/", text)
    return ports[-1] if ports else None


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--url")
    ap.add_argument("--log", default=str(ROOT / "build" / "ghidra_serve.log"))
    args = ap.parse_args()
    url = args.url or find_url(args.log)
    if not url:
        sys.exit("no server URL: start tools/ghidra/serve.sh (logging to build/ghidra_serve.log) or pass --url")

    def post(ep, **kw):
        req = urllib.request.Request(f"{url}/{ep}", data=urllib.parse.urlencode(kw).encode())
        try:
            return json.load(urllib.request.urlopen(req, timeout=120)).get("success", False)
        except Exception:
            return False

    ann = json.loads((ROOT / "tools" / "ghidra" / "annotations.json").read_text())
    names = {}
    for line in (ROOT / "versions" / "names.us.txt").read_text().splitlines():
        m = re.match(r"^(\w+)\s*=\s*0x([0-9A-Fa-f]+);", line)
        if m:
            names[m.group(2).upper().zfill(8)] = m.group(1)
    names.update({a.upper(): n for a, n in ann.get("names", {}).items()})

    ok = {k: 0 for k in ("names", "prototypes", "locals", "data", "comments")}
    for addr, name in names.items():
        ok["names"] += post("rename_function_by_address", function_address=addr.lower(), new_name=name)
    for addr, proto in ann.get("prototypes", {}).items():
        ok["prototypes"] += post("set_function_prototype", function_address=addr.lower(), prototype=proto)
    for addr, var, typ, new in ann.get("locals", []):
        if post("set_local_variable_type", function_address=addr.lower(), variable_name=var, new_type=typ):
            ok["locals"] += 1
            if new:
                post("rename_variable", function_address=addr.lower(), old_name=var, new_name=new)
        elif new and post("set_local_variable_type", function_address=addr.lower(),
                          variable_name=new, new_type=typ):
            ok["locals"] += 1                   # already renamed by an earlier run
    for addr, name in ann.get("data_names", {}).items():
        ok["data"] += post("rename_data", address=addr.lower(), new_name=name)
    for addr, text in ann.get("comments", {}).items():
        ok["comments"] += post("set_decompiler_comment", address=addr.lower(), comment=text)
    total = {"names": len(names), "prototypes": len(ann.get("prototypes", {})),
             "locals": len(ann.get("locals", [])), "data": len(ann.get("data_names", {})),
             "comments": len(ann.get("comments", {}))}
    print("  ".join(f"{k} {ok[k]}/{total[k]}" for k in ok))
    return 0


if __name__ == "__main__":
    sys.exit(main())
