#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-only
#
# Shimera: a simple way to add visual effects without any GPU knowledge
# Copyright (C) 2025-2026 The Shimera Authors
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

"""
Adds the Shimera license header to every tracked source file that lacks it.

Usage:
    python3 scripts/add_license_header.py          # add the header where missing
    python3 scripts/add_license_header.py --check  # only list missing files, exit 1 if any (CI)

See the "License header" section of CONTRIBUTING.md.
"""

import argparse
import subprocess
import sys
from pathlib import Path

EXTENSIONS = {".cpp", ".hpp", ".h", ".inl", ".slang", ".frag", ".vert"}
MARKER = "SPDX-License-Identifier"

HEADER = """\
// SPDX-License-Identifier: GPL-3.0-only
//
// Shimera: a simple way to add visual effects without any GPU knowledge
// Copyright (C) 2025-2026 The Shimera Authors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
"""

# Only the first lines are searched, so a file merely mentioning the marker further down still gets a header.
HEADER_SEARCH_LINES = 5


def repo_root() -> Path:
    out = subprocess.run(["git", "rev-parse", "--show-toplevel"], capture_output=True, text=True, check=True)
    return Path(out.stdout.strip())


def tracked_sources(root: Path) -> list[Path]:
    out = subprocess.run(["git", "ls-files", "-z"], cwd=root, capture_output=True, text=True, check=True)
    return [root / f for f in out.stdout.split("\0") if f and Path(f).suffix in EXTENSIONS]


def has_header(text: str) -> bool:
    return any(MARKER in line for line in text.splitlines()[:HEADER_SEARCH_LINES])


def add_header(path: Path) -> None:
    raw = path.read_bytes()
    bom = b"\xef\xbb\xbf" if raw.startswith(b"\xef\xbb\xbf") else b""
    text = raw[len(bom):].decode("utf-8")
    # keeping the file's own line endings so the diff only shows the header.
    newline = "\r\n" if "\r\n" in text else "\n"
    header = HEADER.replace("\n", newline) + newline
    path.write_bytes(bom + (header + text.lstrip("\r\n")).encode("utf-8"))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--check", action="store_true", help="do not modify files, exit 1 if a header is missing")
    args = parser.parse_args()

    root = repo_root()
    missing = [p for p in tracked_sources(root) if not has_header(p.read_text(encoding="utf-8-sig"))]

    for path in missing:
        rel = path.relative_to(root)
        if args.check:
            print(f"missing license header: {rel}")
        else:
            add_header(path)
            print(f"added license header: {rel}")

    if args.check and missing:
        print(f"\n{len(missing)} file(s) without license header, run scripts/add_license_header.py", file=sys.stderr)
        return 1
    if not missing:
        print("all source files already have the license header")
    return 0


if __name__ == "__main__":
    sys.exit(main())
