#!/usr/bin/env python3
# .githooks/run-tests.py

import subprocess
import sys

if sys.platform == "linux":
    if subprocess.call("./run-tests.sh") != 0:
        sys.exit(1)
elif sys.platform == "win32":
    if subprocess.call(["run-tests.bat"], shell=True) != 0:
        sys.exit(1)
sys.exit(0)