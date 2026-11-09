#!/usr/bin/env python3
# .githooks/check-license-headers.py
#
# Usage: check-license-headers.py <file> [<file> ...]
# Files whose extension is not listed in EXTENSIONS are ignored.

import sys
from pathlib import Path

EXTENSIONS = {".cpp", ".hpp", ".h", ".inl", ".slang", ".frag", ".vert"}

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
""".splitlines()

def find_error(path):
    """Return a description of the header problem in `path`, or None if valid."""
    try:
        # utf-8-sig drops an optional BOM; splitlines() handles LF and CRLF.
        text = path.read_text(encoding="utf-8-sig", errors="replace")
    except OSError as error:
        return f"cannot read file ({error.strerror})"

    lines = text.splitlines()
    for number, expected in enumerate(HEADER, start=1):
        if number > len(lines):
            return f"line {number}: file ends before the end of the header"
        if lines[number - 1] != expected:
            return f"line {number}: expected \"{expected}\""
    return None

def main():
    failed = False
    for name in sys.argv[1:]:
        path = Path(name)
        if path.suffix not in EXTENSIONS:
            continue
        error = find_error(path)
        if error:
            failed = True
            print(f"{name}: {error}")

    if failed:
        print("\nMissing or invalid license header.")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
