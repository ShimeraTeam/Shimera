#!/usr/bin/env python3
# .githooks/validate-commit-msg.py

import re
import sys

commit_msg_file = sys.argv[1]

with open(commit_msg_file) as f:
    commit_msg = f.read().strip()

pattern = r"^(feat|fix|docs|style|refactor|perf|test|build|ci|chore)(\(.+\))?: .{1,50}"

if not re.match(pattern, commit_msg):
    print(f"""
Invalid commit format!

Expected format: <type>(<scope>): <description>

Allowed types:

    feat:      Adding new functionality
    fix:       Squashing bugs
    refactor:  Improving code without changing behavior
    docs:      Documentation updates
    style:     Formatting and code style fixes
    test:      Adding or updating tests
    chore:     Maintenance tasks like dependency updates
    perf:      Performance optimizations
    ci:        Continuous integration tweaks

Examples:

    feat(auth): add user login endpoint
    fix(dashboard): correct chart rendering issue
    docs(readme): update project setup instructions
    refactor(utils): extract date formatter function
    chore: update dependencies to latest versions

Your message: "{commit_msg}"
""")
    sys.exit(1)

sys.exit(0)